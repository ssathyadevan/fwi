#include <memory>
#include "MPIConjugateGradientInversion.h"
#include "progressBar.h"
#include <mpi.h>
#include <iostream>

#define TAG_COMMAND     0
    #define COMMAND_GETUPDATEDIRECTION  1
    #define COMMAND_EXIT                -99
#define TAG_SIZE        1 
#define TAG_RESARRAY    2 
#define TAG_RESULT      3


MPIConjugateGradientInversion::MPIConjugateGradientInversion(ForwardModelInterface *forwardModel, const GenericInput &gInput)
    : _forwardModel(), _cgInput(), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()), _recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    MPIConjugateGradientInversionInputCardReader reader(gInput.caseFolder);
    _forwardModel = forwardModel;
    _cgInput = reader.getInput();
}

double MPIConjugateGradientInversion::findRealRootFromCubic(double a, double b, double c, double d)
{
    // assuming ax^3 + bx^2 +cx + d and assuming only one real root, which is expected in this algorithm
    // uses Cardano's formula
    double f = ((double(3.0) * c / a) - (std::pow(b, 2) / std::pow(a, 2))) / double(3.0);
    double g = ((double(2.0) * std::pow(b, 3) / std::pow(a, 3)) -
                (double(9.0) * b * c / std::pow(a, 2)) +
                (double(27.0) * d / a)) /
               double(27.0);
    double h = (std::pow(g, 2) / double(4.0)) + (std::pow(f, 3) / double(27.0));
    double r = -(g / double(2.0)) + std::sqrt(h);
    double s = std::cbrt(r);
    double t = -(g / double(2.0)) - std::sqrt(h);
    double u = std::cbrt(t);

    double realroot = s + u - (b / (double(3.0) * a));
    return realroot;
}

double MPIConjugateGradientInversion::calculateAlpha(PressureFieldSerial& zeta, std::vector<std::complex<double>>& residuals)
{
    double alphaDiv [2] = {0.0, 0.0};
    int nSignals = _freq.nFreq * _src.nSrc * _recv.nRecv;

    std::vector<std::complex<double>> zetaTemp(_freq.nFreq * _src.nSrc * _recv.nRecv);
    _forwardModel->mapDomainToSignal(zeta, zetaTemp);

    for (int i = 0; i < nSignals; i++)
    {
        alphaDiv[0] += std::real(conj(residuals[i]) * zetaTemp[i]);
        alphaDiv[1] += std::real(conj(residuals[i]) * zetaTemp[i]);
    }

    return alphaDiv[0] / alphaDiv[1];
}

void MPIConjugateGradientInversion::ReconstructSlave(GenericInput gInput){
    int mpi_command, mpi_size, mpi_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    _forwardModel->calculateKappa();
    PressureFieldComplexSerial tmp(_grid);
    tmp.Zero();
    PressureFieldComplexSerial tmp2(_grid);
    tmp2.Zero();
    while(1){
        MPI_Bcast(&mpi_command, 1, MPI_INT, 0, MPI_COMM_WORLD);//receive command
        if (mpi_command == COMMAND_GETUPDATEDIRECTION){
            int array_size;
            MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);//receive array size
            double* deconstructedResArray = new double[array_size];
            MPI_Bcast(deconstructedResArray, array_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);//receive resArray
            std::vector<std::complex<double>> resArray;
            for (int i = 0; i < array_size; i++){                                                       //rebuild resArray
                std::complex<double> temp = {deconstructedResArray[i*2], deconstructedResArray[i*2+1]};
                resArray.push_back(temp);
            }
            int block_size = (_src.nSrc * _recv.nRecv * _freq.nFreq) / mpi_size;
            int offset = block_size * mpi_rank;
            _forwardModel->getUpdateDirectionInformationMPI(resArray, tmp, offset, block_size);//calculate result
            PressureFieldSerial result(_grid);
            result = tmp.GetRealPart();
            double* result_data = result.GetDataPtr();
            MPI_Send(result_data , result.GetNumberOfGridPoints(), MPI_DOUBLE, 0, TAG_RESULT, MPI_COMM_WORLD);

            
            //return result
            //std::cerr<< "Get Update Direction" << std::endl;
        }
        if (mpi_command == COMMAND_EXIT){
            //std::cerr<< "Exit" << std::endl;

            break;
        }
        //loop
    }
    return;
}

PressureFieldSerial MPIConjugateGradientInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput ){
    int mpi_size, mpi_command;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    //ProgressBar bar(_cgInput.n_max * _cgInput.iteration1.n);

    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;

    double eta = 1.0 / (normSq(pData, nTotal)); //scaling factor eq 2.10 in thesis
    double gamma, alpha, resSq, res = 0;

    std::array<double, 2> alphaDiv;

    PressureFieldSerial chiEst(_grid), g(_grid), gOld(_grid), zeta(_grid);
    PressureFieldComplexSerial tmp(_grid); // eq: integrandForDiscreteK, tmp is the argument of Re()

    chiEst.Zero();

    PressureFieldSerial **gradientChiOld = new PressureFieldSerial *[2];
    PressureFieldSerial **gradientGregTmp = new PressureFieldSerial *[2];
    PressureFieldSerial **gradientZetaTmp = new PressureFieldSerial *[2];

    for (int i = 0; i < 2; i++)
    {
        gradientChiOld[i] = new PressureFieldSerial(_grid);
        gradientGregTmp[i] = new PressureFieldSerial(_grid);
        gradientZetaTmp[i] = new PressureFieldSerial(_grid);
    }

    // open the file to store the residual log
    std::ofstream file;
    file.open(gInput.outputLocation + gInput.runName + "Residual.log", std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout << "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    int counter = 1;
    
    //main loop//
    for (int it = 0; it < _cgInput.n_max; it++)
    {
        std::vector<std::complex<double>> vecResFirstIter;

        double deltasquaredOld = double(0.0);
        double fRegOld = double(1.0);
        double fDataOld = double(0.0);
        double deltaAmplification = _cgInput.dAmplification.start / (_cgInput.dAmplification.slope * it + double(1.0));

        PressureFieldSerial bsquaredOld(_grid);
        bsquaredOld.Zero();

        _forwardModel->calculateKappa();

        std::vector<std::complex<double>> &resArray = _forwardModel->calculateResidual(chiEst, pData);

        //start the inner loop
        for (int it1 = 0; it1 < _cgInput.iteration1.n; it1++)
        {
            if (it1 == 0)
            {
                PressureFieldSerial* result = this->getUpdateDirectionInformation(resArray, mpi_size);               

                g = eta * *result; //eq: gradientRunc
                zeta = g;

                alphaDiv[0] = double(0.0);
                alphaDiv[1] = double(0.0);

                std::vector<std::complex<double>> zetaTemp(_freq.nFreq * _src.nSrc * _recv.nRecv);
                _forwardModel->mapDomainToSignal(zeta, zetaTemp);

                for (int i = 0; i < nTotal; i++)
                {
                    alphaDiv[0] += std::real(conj(resArray[i]) * zetaTemp[i]);
                    alphaDiv[1] += std::real(conj(zetaTemp[i]) * zetaTemp[i]);
                }

                alpha = alphaDiv[0] / alphaDiv[1]; //eq:optimalStepSizeCG in the readme pdf
                chiEst += alpha * zeta;
                gOld = g;

                resArray = _forwardModel->calculateResidual(chiEst, pData);
                resSq = _forwardModel->calculateResidualNormSq(resArray);

                res = eta * resSq;

                std::cout << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: " << std::setprecision(17) << res << std::endl;

                file << std::setprecision(17) << res << "," << counter << std::endl;
                counter++; // store the residual value in the residual log

                fDataOld = res;
                vecResFirstIter.push_back(res);
            }
            else
            {
                chiEst.Gradient(gradientChiOld);
                PressureFieldSerial gradientChiOldNormsquared(_grid);

                gradientChiOldNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) + (*gradientChiOld[1] * *gradientChiOld[1]);

                PressureFieldSerial bsquared = (gradientChiOldNormsquared + deltasquaredOld); // eq: errorFuncRegulWeighting
                bsquared.Reciprocal();
                bsquared *= (double(1.0) / (_grid.GetDomainArea())); // # eq. 2.22
                PressureFieldSerial b = bsquared;
                b.Sqrt();

                PressureFieldSerial tmpVolField = b * *gradientChiOld[0];
                tmpVolField.Square();
                PressureFieldSerial tmpVolField2 = b * *gradientChiOld[1];
                tmpVolField2.Square();
                tmpVolField += tmpVolField2;
                double deltasquared = deltaAmplification * double(0.5) * tmpVolField.Summation() / bsquared.Summation(); // # eq. 2.23

                tmpVolField = bsquaredOld * *gradientChiOld[0];
                tmpVolField.Gradient(gradientGregTmp);
                tmpVolField = *gradientGregTmp[0];
                tmpVolField2 = bsquaredOld * *gradientChiOld[1];
                tmpVolField2.Gradient(gradientGregTmp);
                tmpVolField2 = *gradientGregTmp[1];

                PressureFieldSerial gReg = tmpVolField + tmpVolField2; //# eq. 2.24
                tmp.Zero();

                PressureFieldSerial* result = this->getUpdateDirectionInformation(resArray, mpi_size);

                g = eta * fRegOld * *result + fDataOld * gReg; // # eq: integrandForDiscreteK

                gamma = g.InnerProduct(g - gOld) / gOld.InnerProduct(gOld); // # eq: PolakRibiereDirection
                zeta = g + gamma * zeta;

                std::array<double, 2> A = {0.0, 0.0};

                std::vector<std::complex<double>> zetaTemp(_freq.nFreq * _src.nSrc * _recv.nRecv);
                _forwardModel->mapDomainToSignal(zeta, zetaTemp);

                for (int i = 0; i < nTotal; i++)
                {
                    A[1] += eta * std::real(conj(zetaTemp[i]) * zetaTemp[i]);
                    A[0] += double(-2.0) * eta * std::real(conj(resArray[i]) * zetaTemp[i]);
                }

                double A0 = fDataOld;
                zeta.Gradient(gradientZetaTmp);
                tmpVolField = b * *gradientZetaTmp[0];
                tmpVolField2 = b * *gradientZetaTmp[1];
                tmpVolField.Square();
                tmpVolField2.Square();
                double B2 = (tmpVolField.Summation() + tmpVolField2.Summation()) * _grid.GetCellVolume();

                tmpVolField = (b * *gradientZetaTmp[0]) * (b * *gradientChiOld[0]);
                tmpVolField2 = (b * *gradientZetaTmp[1]) * (b * *gradientChiOld[1]);
                double B1 = double(2.0) * (tmpVolField.Summation() + tmpVolField2.Summation()) * _grid.GetCellVolume();

                tmpVolField = (b * *gradientChiOld[0]) * (b * *gradientChiOld[0]);
                tmpVolField2 = (b * *gradientChiOld[1]) * (b * *gradientChiOld[1]);
                double B0 = ((tmpVolField.Summation() + tmpVolField2.Summation()) + deltasquaredOld * bsquared.Summation()) * _grid.GetCellVolume();

                double derA = double(4.0) * A[1] * B2;
                double derB = double(3.0) * (A[1] * B1 + A[0] * B2);
                double derC = double(2.0) * (A[1] * B0 + A[0] * B1 + A0 * B2);
                double derD = A[0] * B0 + A0 * B1;

                alpha = this->findRealRootFromCubic(derA, derB, derC, derD);

                chiEst += alpha * zeta;

                resArray = _forwardModel->calculateResidual(chiEst, pData);
                resSq = _forwardModel->calculateResidualNormSq(resArray);
                res = eta * resSq;

                std::cout << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: "
                            << std::setprecision(17) << res << std::endl;

                file << std::setprecision(17) << res << "," << counter << std::endl; // store the residual value in the residual log
                counter++;
                fDataOld = res;
                vecResFirstIter.push_back(res);

                //breakout check
                if ((it1 > 0) && ((res < double(_cgInput.iteration1.tolerance)) || (std::abs(vecResFirstIter[it1 - 1] - res) < double(_cgInput.iteration1.tolerance)))) {
                    //bar.setCounter(_cgInput.iteration1.n + bar.getCounter() - (bar.getCounter() % _cgInput.iteration1.n));
                    break; }        

                chiEst.Gradient(gradientChiOld);
                PressureFieldSerial gradientChiNormsquared(_grid);
                gradientChiNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) +
                                            (*gradientChiOld[1] * *gradientChiOld[1]);

                tmpVolField = (gradientChiNormsquared + deltasquaredOld) / (gradientChiOldNormsquared + deltasquaredOld);
                fRegOld = (double(1.0) / (_grid.GetDomainArea())) *
                            tmpVolField.Summation() * _grid.GetCellVolume();

                deltasquaredOld = deltasquared;
                gOld = g;
                bsquaredOld = bsquared;
            }
            //bar++;
        } // end regularisation loop
    }
    file.close(); // close the residual.log file
    mpi_command = COMMAND_EXIT;
    MPI_Bcast(&mpi_command, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < 2; i++)
    {
        delete gradientChiOld[i];
        delete gradientGregTmp[i];
        delete gradientZetaTmp[i];
    }
    delete[] gradientChiOld;
    delete[] gradientGregTmp;
    delete[] gradientZetaTmp;

    PressureFieldSerial result(_grid);
    chiEst.CopyTo(result);
    return result;
}

PressureFieldSerial* MPIConjugateGradientInversion::getUpdateDirectionInformation(std::vector<std::complex<double>> resArray, const int mpi_size){
    // MPI GetUpdateDirection
    int mpi_command = COMMAND_GETUPDATEDIRECTION;
    MPI_Bcast(&mpi_command, 1, MPI_INT, 0, MPI_COMM_WORLD); //Send command

    int array_size = resArray.size()*2;
    double* deconstructedResArray = new double[ array_size ];

    MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD); //Send size

    for (unsigned i = 0; i < resArray.size() ; i++){                                   //Serialize resArray into chain of doubles
        deconstructedResArray[i * 2]     = resArray[i].real();
        deconstructedResArray[i * 2 + 1] = resArray[i].imag();
    }

    MPI_Bcast(deconstructedResArray, resArray.size() * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD); //Send serialized array

    int block_size = (_src.nSrc * _recv.nRecv * _freq.nFreq) / mpi_size;
    PressureFieldComplexSerial tmp(_grid);
    tmp.Zero();
    _forwardModel->getUpdateDirectionInformationMPI(resArray, tmp, 0, block_size);

    PressureFieldSerial* result = new PressureFieldSerial(_grid);
    *result = tmp.GetRealPart();
    double* result_data = result->GetDataPtr();

    //for (int i = 0 ; i < result.GetNumberOfGridPoints(); i++) std::cout << result_data[i] << std::endl;
    
    double** partialResult = new double*[mpi_size-1];
    for (int i = 0; i < mpi_size - 1; i++){
        partialResult[i] = new double[result->GetNumberOfGridPoints()];
        MPI_Recv(partialResult[i], result->GetNumberOfGridPoints(), MPI_DOUBLE, i+1, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//receive resArray
    }
    
    for (int j = 0; j < mpi_size -1; j++){
        for (int i = 0; i < result->GetNumberOfGridPoints(); i++){//Rebuild result
            result_data[i] += partialResult[j][i];
        }
    }
    return result;
}