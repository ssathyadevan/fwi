#include "MPIConjugateGradientInversion.h"
#include "cpuClockMPI.h"
#include "log.h"
#include "progressBar.h"
#include <iostream>
#include <memory>
#include <mpi.h>

#define COMMAND_GETUPDATEDIRECTION 1
#define COMMAND_EXIT -99
#define TAG_RESARRAY 2
#define TAG_RESULT 3

int once = 0;
MPIConjugateGradientInversion::MPIConjugateGradientInversion(ForwardModelInterface *forwardModel, const MPIConjugateGradientInversionInput &cgInput) :
    _forwardModel(), _cgInput(cgInput), _grid(forwardModel->getGrid()), _source(forwardModel->getSource()), _receiver(forwardModel->getReceiver()),
    _freq(forwardModel->getFreq())
{
    _forwardModel = forwardModel;
}

double MPIConjugateGradientInversion::findRealRootFromCubic(double a, double b, double c, double d)
{
    // assuming ax^3 + bx^2 +cx + d and assuming only one real root, which is expected in this algorithm
    // uses Cardano's formula
    double f = ((double(3.0) * c / a) - (std::pow(b, 2) / std::pow(a, 2))) / double(3.0);
    double g = ((double(2.0) * std::pow(b, 3) / std::pow(a, 3)) - (double(9.0) * b * c / std::pow(a, 2)) + (double(27.0) * d / a)) / double(27.0);
    double h = (std::pow(g, 2) / double(4.0)) + (std::pow(f, 3) / double(27.0));
    double r = -(g / double(2.0)) + std::sqrt(h);
    double s = std::cbrt(r);
    double t = -(g / double(2.0)) - std::sqrt(h);
    double u = std::cbrt(t);

    double realroot = s + u - (b / (double(3.0) * a));
    return realroot;
}

double MPIConjugateGradientInversion::calculateAlpha(PressureFieldSerial &zeta, std::vector<std::complex<double>> &residuals)
{
    double alphaDiv[2] = {0.0, 0.0};
    int nSignals = _freq.count * _source.count * _receiver.count;

    std::vector<std::complex<double>> zetaTemp(_freq.count * _source.count * _receiver.count);
    _forwardModel->mapDomainToSignal(zeta, zetaTemp);

    for(int i = 0; i < nSignals; i++)
    {
        alphaDiv[0] += std::real(conj(residuals[i]) * zetaTemp[i]);
        alphaDiv[1] += std::real(conj(residuals[i]) * zetaTemp[i]);
    }

    return alphaDiv[0] / alphaDiv[1];
}

void MPIConjugateGradientInversion::ReconstructSlave()
{
    int mpi_command, mpi_size, mpi_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    _forwardModel->calculateKappa();
    PressureFieldComplexSerial tmp(_grid);
    PressureFieldComplexSerial tmp2(_grid);

    mpi_command = 0;
    while(mpi_command != COMMAND_EXIT)
    {
        MPI_Bcast(&mpi_command, 1, MPI_INT, 0, MPI_COMM_WORLD);   // receive command
        if(mpi_command == COMMAND_GETUPDATEDIRECTION)
        {
            int block_size = (_source.count * _receiver.count * _freq.count) / mpi_size;
            int array_size = block_size * 2;
            int offset = block_size * (mpi_rank - 1);
            double *deconstructedResArray = new double[array_size];
            L_(lwarning) << "In thread " << mpi_rank << " array_size: " << array_size << " block_size " << block_size;
            MPI_Recv(deconstructedResArray, array_size, MPI_DOUBLE, 0, TAG_RESARRAY, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   // receive resArray
            std::vector<std::complex<double>> resArray;
            for(int i = 0; i < block_size; i++)
            {   // rebuild resArray
                std::complex<double> temp = {deconstructedResArray[i * 2], deconstructedResArray[i * 2 + 1]};
                resArray.push_back(temp);
            }
            if(!once)
            {
                for(unsigned i = 0; i < resArray.size(); i++)
                {
                    L_(io::lerror) << i << "  " << resArray[i].real() << "  " << resArray[i].imag();
                }
                once++;
            }
            L_(lwarning) << "In thread " << mpi_rank << " offset: " << offset << " block_size " << block_size;
            _forwardModel->getUpdateDirectionInformationMPI(resArray, tmp, offset, block_size);   // calculate result
            PressureFieldSerial result(_grid);
            result = tmp.GetRealPart();
            double *result_data = result.GetDataPtr();
            MPI_Request send_req;
            MPI_Isend(result_data, result.GetNumberOfGridPoints(), MPI_DOUBLE, 0, TAG_RESULT, MPI_COMM_WORLD, &send_req);   // send ResArray
            MPI_Waitall(1, &send_req, MPI_STATUSES_IGNORE);
            delete deconstructedResArray;
            // std::cerr<< "Get Update Direction" << std::endl;
        }
        // loop
    }
    return;
}

PressureFieldSerial *MPIConjugateGradientInversion::getUpdateDirectionInformation(std::vector<std::complex<double>> resArray, const int mpi_size)
{
    // MPI GetUpdateDirection
    int mpi_command = COMMAND_GETUPDATEDIRECTION;
    MPI_Bcast(&mpi_command, 1, MPI_INT, 0, MPI_COMM_WORLD);   // Send command

    int deconstructed_array_size = resArray.size() * 2;
    double *deconstructedResArray = new double[deconstructed_array_size];
    int block_size = (_source.count * _receiver.count * _freq.count) / mpi_size;
    int partial_array_size = block_size * 2;
    L_(lwarning) << "In thread " << 0 << " array_size: " << partial_array_size << " block_size " << block_size;
    L_(lwarning) << "Original resArray size " << resArray.size();
    // MPI_Bcast(&partial_array_size, 1, MPI_INT, 0, MPI_COMM_WORLD); //Send size

    for(unsigned i = 0; i < resArray.size(); i++)
    {   // Serialize resArray into chain of doubles
        deconstructedResArray[i * 2] = resArray[i].real();
        deconstructedResArray[i * 2 + 1] = resArray[i].imag();
    }

    int offset = 0;
    for(int r = 1; r < mpi_size; r++)
    {
        MPI_Send(deconstructedResArray + offset, partial_array_size, MPI_DOUBLE, r, TAG_RESARRAY, MPI_COMM_WORLD);   // Send serialized array
        offset += partial_array_size;
    }

    PressureFieldComplexSerial tmp(_grid);
    tmp.Zero();
    offset = block_size * (mpi_size - 1);
    if(!once)
    {
        for(unsigned i = 0; i < resArray.size(); i++)
        {
            L_(io::lerror) << i << "  " << resArray[i].real() << "  " << resArray[i].imag();
        }
        once++;
    }
    L_(lwarning) << "In thread " << 0 << " offset: " << offset << " block_size " << resArray.size() - offset;
    std::vector<std::complex<double>> sliced_resArray = std::vector<std::complex<double>>(resArray.begin() + offset, resArray.end());
    _forwardModel->getUpdateDirectionInformationMPI(sliced_resArray, tmp, offset, resArray.size() - offset);
    PressureFieldSerial *result = new PressureFieldSerial(_grid);
    *result = tmp.GetRealPart();
    double *result_data = result->GetDataPtr();

    double **partialResult = new double *[mpi_size - 1];
    MPI_Request *receiver_req = new MPI_Request[mpi_size - 1];
    for(int i = 0; i < mpi_size - 1; i++)
    {
        partialResult[i] = new double[result->GetNumberOfGridPoints()];
        MPI_Ireceiver(partialResult[i], result->GetNumberOfGridPoints(), MPI_DOUBLE, i + 1, TAG_RESULT, MPI_COMM_WORLD, receiver_req + i);   // receive resArray
    }
    MPI_Waitall(mpi_size - 1, receiver_req, MPI_STATUSES_IGNORE);

    for(int j = 0; j < mpi_size - 1; j++)
    {
        for(int i = 0; i < result->GetNumberOfGridPoints(); i++)
        {   // Rebuild result
            result_data[i] += partialResult[j][i];
        }
    }
    delete deconstructedResArray;
    for(int i = 0; i < mpi_size - 1; i++)
    {
        delete partialResult[i];
    }
    delete partialResult;
    return result;
}

PressureFieldSerial MPIConjugateGradientInversion::Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput)
{
    int mpi_size, mpi_command;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    // ProgressBar bar(_cgInput.n_max * _cgInput.iteration1.n);
    // bar.setTerminalWidth(80);
    const int nTotal = _freq.count * _source.count * _receiver.count;

    double eta = 1.0 / (forwardModels::normSq(pData, nTotal));   // scaling factor eq 2.10 in thesis
    double gamma, alpha, resSq, res = 0;

    std::array<double, 2> alphaDiv;

    PressureFieldSerial chiEst(_grid), g(_grid), gOld(_grid), zeta(_grid);
    PressureFieldComplexSerial tmp(_grid);   // eq: integrandForDiscreteK, tmp is the argument of Re()

    chiEst.Zero();

    std::vector<PressureFieldSerial> gradientChiOld(2, PressureFieldSerial(_grid));
    std::vector<PressureFieldSerial> gradientGregTmp(2, PressureFieldSerial(_grid));
    std::vector<PressureFieldSerial> gradientZetaTmp(2, PressureFieldSerial(_grid));

    std::ofstream residualLogFile = openResidualLogFile(gInput);

    // main loop//
    int counter = 1;
    for(int it = 0; it < _cgInput.n_max; it++)
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

        // start the inner loop
        for(int it1 = 0; it1 < _cgInput.iteration1.n; it1++)
        {
            if(it1 == 0)
            {
                PressureFieldSerial *result = getUpdateDirectionInformation(resArray, mpi_size);

                g = eta * *result;   // eq: gradientRunc
                zeta = g;

                alphaDiv[0] = double(0.0);
                alphaDiv[1] = double(0.0);

                std::vector<std::complex<double>> zetaTemp(_freq.count * _source.count * _receiver.count);
                _forwardModel->mapDomainToSignal(zeta, zetaTemp);

                for(int i = 0; i < nTotal; i++)
                {
                    alphaDiv[0] += std::real(conj(resArray[i]) * zetaTemp[i]);
                    alphaDiv[1] += std::real(conj(zetaTemp[i]) * zetaTemp[i]);
                }

                if(alphaDiv[1] == 0.0)
                {
                    throw std::overflow_error("MPIConjugateGradient: the computation of alpha devides by zero.");
                }

                alpha = alphaDiv[0] / alphaDiv[1];   // eq:optimalStepSizeCG in the readme pdf
                chiEst += alpha * zeta;
                gOld = g;

                resArray = _forwardModel->calculateResidual(chiEst, pData);
                resSq = _forwardModel->calculateResidualNormSq(resArray);

                res = eta * resSq;

                L_(io::linfo) << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: " << std::setprecision(17)
                          << res;

                residualLogFile << std::setprecision(17) << res << "," << counter << std::endl;
                counter++;   // store the residual value in the residual log

                fDataOld = res;
                vecResFirstIter.push_back(res);
            }
            else
            {
                chiEst.Gradient(gradientChiOld);
                PressureFieldSerial gradientChiOldNormsquared(_grid);

                gradientChiOldNormsquared = (gradientChiOld[0] * gradientChiOld[0]) + (gradientChiOld[1] * gradientChiOld[1]);

                PressureFieldSerial bsquared = (gradientChiOldNormsquared + deltasquaredOld);   // eq: errorFuncRegulWeighting
                bsquared.Reciprocal();
                bsquared *= (double(1.0) / (_grid.GetDomainArea()));   // # eq. 2.22
                PressureFieldSerial b = bsquared;
                b.Sqrt();

                PressureFieldSerial tmpVolField = b * gradientChiOld[0];
                tmpVolField.Square();
                PressureFieldSerial tmpVolField2 = b * gradientChiOld[1];
                tmpVolField2.Square();
                tmpVolField += tmpVolField2;

                double bsquared_sum = bsquared.Summation();
                if(bsquared_sum == 0.0)
                {
                    throw std::overflow_error("MPIConjugateGradient: the computation of deltasquared devides by zero.");
                }
                double deltasquared = deltaAmplification * double(0.5) * tmpVolField.Summation() / bsquared_sum;   // # eq. 2.23

                tmpVolField = bsquaredOld * gradientChiOld[0];
                tmpVolField.Gradient(gradientGregTmp);
                tmpVolField = gradientGregTmp[0];
                tmpVolField2 = bsquaredOld * gradientChiOld[1];
                tmpVolField2.Gradient(gradientGregTmp);
                tmpVolField2 = gradientGregTmp[1];

                PressureFieldSerial gReg = tmpVolField + tmpVolField2;   //# eq. 2.24
                tmp.Zero();

                PressureFieldSerial *result = this->getUpdateDirectionInformation(resArray, mpi_size);

                g = eta * fRegOld * *result + fDataOld * gReg;   // # eq: integrandForDiscreteK

                double inproduct_gOld = gOld.InnerProduct(gOld);
                if(inproduct_gOld == 0.0)
                {
                    throw std::overflow_error("MPIConjugateGradient: the computation of gamma devides by zero.");
                }

                gamma = g.InnerProduct(g - gOld) / inproduct_gOld;   // # eq: PolakRibiereDirection
                zeta = g + gamma * zeta;

                std::array<double, 2> A = {0.0, 0.0};

                std::vector<std::complex<double>> zetaTemp(_freq.count * _source.count * _receiver.count);
                _forwardModel->mapDomainToSignal(zeta, zetaTemp);

                for(int i = 0; i < nTotal; i++)
                {
                    A[1] += eta * std::real(conj(zetaTemp[i]) * zetaTemp[i]);
                    A[0] += double(-2.0) * eta * std::real(conj(resArray[i]) * zetaTemp[i]);
                }

                double A0 = fDataOld;
                zeta.Gradient(gradientZetaTmp);
                tmpVolField = b * gradientZetaTmp[0];
                tmpVolField2 = b * gradientZetaTmp[1];
                tmpVolField.Square();
                tmpVolField2.Square();
                double B2 = (tmpVolField.Summation() + tmpVolField2.Summation()) * _grid.GetCellVolume();

                tmpVolField = (b * gradientZetaTmp[0]) * (b * gradientChiOld[0]);
                tmpVolField2 = (b * gradientZetaTmp[1]) * (b * gradientChiOld[1]);
                double B1 = double(2.0) * (tmpVolField.Summation() + tmpVolField2.Summation()) * _grid.GetCellVolume();

                tmpVolField = (b * gradientChiOld[0]) * (b * gradientChiOld[0]);
                tmpVolField2 = (b * gradientChiOld[1]) * (b * gradientChiOld[1]);
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

                L_(io::linfo) << it1 + 1 << "/" << _cgInput.iteration1.n << "\t (" << it + 1 << "/" << _cgInput.n_max << ")\t res: " << std::setprecision(17)
                          << res;

                residualLogFile << std::setprecision(17) << res << "," << counter << std::endl;   // store the residual value in the residual log
                counter++;
                fDataOld = res;
                vecResFirstIter.push_back(res);

                // breakout check
                if((it1 > 0) &&
                    ((res < double(_cgInput.iteration1.tolerance)) || (std::abs(vecResFirstIter[it1 - 1] - res) < double(_cgInput.iteration1.tolerance))))
                {
                    // bar.setCounter(_cgInput.iteration1.n + bar.getCounter() - (bar.getCounter() % _cgInput.iteration1.n));
                    break;
                }

                chiEst.Gradient(gradientChiOld);
                PressureFieldSerial gradientChiNormsquared(_grid);
                gradientChiNormsquared = (gradientChiOld[0] * gradientChiOld[0]) + (gradientChiOld[1] * gradientChiOld[1]);

                tmpVolField = (gradientChiNormsquared + deltasquaredOld) / (gradientChiOldNormsquared + deltasquaredOld);
                fRegOld = (double(1.0) / (_grid.GetDomainArea())) * tmpVolField.Summation() * _grid.GetCellVolume();

                deltasquaredOld = deltasquared;
                gOld = g;
                bsquaredOld = bsquared;
            }
            // bar++;
        }   // end regularisation loop
    }
    residualLogFile.close();   // close the residual.log file
    mpi_command = COMMAND_EXIT;
    MPI_Bcast(&mpi_command, 1, MPI_INT, 0, MPI_COMM_WORLD);

    PressureFieldSerial result(_grid);
    chiEst.CopyTo(result);
    return result;
}

std::ofstream ConjugateGradientInversion::openResidualLogFile(io::genericInput &gInput)
{
    std::string filePath = gInput.outputLocation + gInput.runName + "Residual" + ".log";

    std::ofstream residualLogFile;
    residualLogFile.open(filePath, std::ios::out | std::ios::trunc);
    if(!residualLogFile)
    {
        throw std::invalid_argument("Unable to store residuals from file : " + filePath);
    }

    return residualLogFile;
}
