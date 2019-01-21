#include "inversion.h"

double inversion::findRealRootFromCubic(double a, double b, double c, double d)
{
    // assuming ax^3 + bx^2 +cx + d and assuming only one real root, which is expected in this algorithm
    // uses Cardano's formula
    double f = ( (double(3.0) * c / a) - (std::pow(b, 2) / std::pow(a, 2) ) ) / double(3.0);
    double g = ( (double(2.0) * std::pow(b, 3) / std::pow(a, 3) ) -
                 (double(9.0) * b * c / std::pow(a, 2) ) +
                 (double(27.0) * d / a)) / double(27.0);
    double h = ( std::pow(g, 2) / double(4.0) ) + ( std::pow(f, 3) / double(27.0) );
    double r = - (g / double(2.0) ) + std::sqrt(h);
    double s = std::cbrt(r);
    double t = - (g / double(2.0) ) - std::sqrt(h);
    double u = std::cbrt(t);

    double realroot = s + u - (b / (double(3.0) * a) );
    return realroot;
}


pressureFieldSerial inversion::Reconstruct(const std::complex<double> *const pData, Input input)
{
    double eta = 1.0/(normSq(pData,forwardModel_->getInput().freq.nTotal*
                             forwardModel_->getInput().nSourcesReceivers.rec*
                             forwardModel_->getInput().nSourcesReceivers.src));//scaling factor eq 2.10 in thesis
    double gamma, alpha, res;

    std::array<double,2> alphaDiv;

    const int nTotal = forwardModel_->getInput().freq.nTotal*
            forwardModel_->getInput().nSourcesReceivers.rec*
            forwardModel_->getInput().nSourcesReceivers.src;

    pressureFieldSerial chiEst(forwardModel_->getGrid() ),
            g(forwardModel_->getGrid() ), gOld(forwardModel_->getGrid() ),
            zeta(forwardModel_->getGrid() ); // stays here
    pressureFieldComplexSerial tmp(forwardModel_->getGrid() ); // eq: integrandForDiscreteK, tmp is the argument of Re()
    chiEst.Zero();


    pressureFieldSerial **gradientChiOld = new pressureFieldSerial*[2];
    pressureFieldSerial **gradientGregTmp = new pressureFieldSerial*[2];
    pressureFieldSerial **gradientZetaTmp = new pressureFieldSerial*[2];

    for (int i = 0; i < 2; i++)
    {
        gradientChiOld[i] = new pressureFieldSerial(forwardModel_->getGrid() );
        gradientGregTmp[i] = new pressureFieldSerial(forwardModel_->getGrid() );
        gradientZetaTmp[i] = new pressureFieldSerial(forwardModel_->getGrid() );
    }

    // open the file to store the residual log
    std::ofstream file;
    file.open (input.outputLocation+input.cardName+"Residual.log", std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout<< "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    int counter = 1;
    //main loop//
    for(int it=0; it < input.n_max; it++)
    {
        std::vector<std::complex<double>> vecResFirstIter;
        if (input.doReg == 0)
        {
            forwardModel_->intermediateForwardModelStep1();
            for (int it1=0; it1 < input.iter1.n; it1++)
            {
                forwardModel_->calculateResidual(chiEst, pData);
                res = forwardModel_->calculateResidualNormSq(eta);
                {
                    if (it1 > 0)
                    {
                        std::cout << "inner loop residual = " << std::setprecision(17) << res << "     " <<
                                     std::abs(vecResFirstIter[it1-1] - res) << "    " << it1+1 << '/' << input.iter1.n << std::endl;
                    }
                    else
                    {
                        std::cout << "inner loop residual = " << std::setprecision(17) << res << std::endl;
                    }
                }

                vecResFirstIter.push_back(res);

                if ( (it1 > 0) && ( (res < double(input.iter1.tolerance)) ||
                                    ( std::abs(vecResFirstIter[it1-1] - res) < double(input.iter1.tolerance) ) ) )
                {
                    break;
                }

                forwardModel_->calculateKRes(tmp);
                g = (2*eta) * tmp.GetRealPart();

                if (it1==0)
                {
                    zeta = g;
                }
                else
                {
                    gamma = g.InnerProduct(g-gOld) / gOld.InnerProduct(gOld);
                    zeta = g + gamma*zeta;
                }

                alphaDiv[0] = 0.0;
                alphaDiv[1] = 0.0;
                auto zetaTemp = forwardModel_->intermediateForwardModelStep2(zeta);
                for (int i = 0; i < nTotal; i++)
                {
                    alphaDiv[0] += std::real( conj(forwardModel_->getResidual()[i]) * zetaTemp[i] );
                    alphaDiv[1] += std::real( conj(zetaTemp[i]) * zetaTemp[i] );
                }
                alpha = alphaDiv[0] / alphaDiv[1];
                chiEst += alpha*zeta; // the step size of the parameter in Eq: ContrastUpdate in the user manual.
                gOld = g;
            }
        }
        else if (input.doReg == 1)
        {
            double deltasquaredOld = double(0.0);
            pressureFieldSerial bsquaredOld(forwardModel_->getGrid());
            bsquaredOld.Zero();
            double fRegOld = double(1.0);
            double fDataOld = double(0.0);
            double deltaAmplification = input.deltaAmplification.start / (input.deltaAmplification.slope * it + double(1.0) );

            forwardModel_->intermediateForwardModelStep1();
            forwardModel_->calculateResidual(chiEst, pData);

            //start the inner loop
            for (int it1=0; it1 < input.iter1.n; it1++)
            {
                if (it1 == 0)
                {
                    forwardModel_->calculateKRes(tmp);
                    g = eta * tmp.GetRealPart(); //eq: gradientRunc
                    zeta = g;
                    alphaDiv[0] = double(0.0);
                    alphaDiv[1] =double(0.0);
                    auto zetaTemp = forwardModel_->intermediateForwardModelStep2(zeta);

                    for (int i = 0; i < nTotal; i++)
                    {
                        alphaDiv[0] += std::real( conj(forwardModel_->getResidual()[i]) * zetaTemp[i] );
                        alphaDiv[1] += std::real( conj(zetaTemp[i]) * zetaTemp[i] );
                    }
                    alpha = alphaDiv[0] / alphaDiv[1]; //eq:optimalStepSizeCG in the readme pdf
                    chiEst += alpha*zeta;
                    gOld = g;

                    forwardModel_->calculateResidual(chiEst, pData);
                    res = forwardModel_->calculateResidualNormSq(eta);
                    std::cout << it1+1 << "/" << input.iter1.n << "\t (" << it+1 << "/" << input.n_max << ")\t res: " << std::setprecision(17) << res << std::endl;

                    file << std::setprecision(17) << res << "," << counter << std::endl;
                    counter++;// store the residual value in the residual log

                    fDataOld = res;
                    vecResFirstIter.push_back(res);
                }
                else
                {
                    chiEst.Gradient(gradientChiOld);
                    pressureFieldSerial gradientChiOldNormsquared(forwardModel_->getGrid());
                    gradientChiOldNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) + (*gradientChiOld[1] * *gradientChiOld[1]);

                    pressureFieldSerial bsquared = ( gradientChiOldNormsquared + deltasquaredOld );// eq: errorFuncRegulWeighting
                    bsquared.Reciprocal();
                    bsquared *= (double(1.0) / (forwardModel_->getGrid().GetDomainArea()) );// # eq. 2.22
                    pressureFieldSerial b = bsquared;
                    b.Sqrt();

                    pressureFieldSerial tmpVolField = b * *gradientChiOld[0];
                    tmpVolField.Square();
                    pressureFieldSerial tmpVolField2 = b * *gradientChiOld[1];
                    tmpVolField2.Square();
                    tmpVolField += tmpVolField2;
                    double deltasquared = deltaAmplification * double(0.5) * tmpVolField.Summation() / bsquared.Summation(); // # eq. 2.23

                    tmpVolField = bsquaredOld * *gradientChiOld[0];
                    tmpVolField.Gradient(gradientGregTmp);
                    tmpVolField = *gradientGregTmp[0];
                    tmpVolField2 = bsquaredOld * *gradientChiOld[1];
                    tmpVolField2.Gradient(gradientGregTmp);
                    tmpVolField2 = *gradientGregTmp[1];

                    pressureFieldSerial gReg = tmpVolField + tmpVolField2;//# eq. 2.24
                    tmp.Zero();
                    forwardModel_->calculateKRes(tmp);
                    g = eta * fRegOld * tmp.GetRealPart() + fDataOld * gReg; // # eq: integrandForDiscreteK

                    gamma = g.InnerProduct(g - gOld) / gOld.InnerProduct(gOld);// # eq: PolakRibiereDirection
                    zeta = g + gamma*zeta;

                    std::array<double,2> A = {0.0, 0.0};
                    auto zetaTemp = forwardModel_->intermediateForwardModelStep2(zeta);
                    for (int i = 0; i < nTotal; i++)
                    {
                        A[1] += eta * std::real( conj(zetaTemp[i]) * zetaTemp[i]);
                        A[0] += double(-2.0) * eta * std::real( conj(forwardModel_->getResidual()[i]) * zetaTemp[i]);
                    }
                    double A0 = fDataOld;
                    zeta.Gradient(gradientZetaTmp);
                    tmpVolField = b * *gradientZetaTmp[0];
                    tmpVolField2 = b * *gradientZetaTmp[1];
                    tmpVolField.Square();
                    tmpVolField2.Square();
                    double B2 = (tmpVolField.Summation() + tmpVolField2.Summation()) * forwardModel_->getGrid().GetCellVolume();

                    tmpVolField = (b * *gradientZetaTmp[0]) * (b * *gradientChiOld[0]);
                    tmpVolField2 = (b * *gradientZetaTmp[1]) * (b * *gradientChiOld[1]);
                    double B1 = double(2.0) * (tmpVolField.Summation() + tmpVolField2.Summation()) *
                            forwardModel_->getGrid().GetCellVolume();

                    tmpVolField = (b * *gradientChiOld[0]) * (b * *gradientChiOld[0]);
                    tmpVolField2 = (b * *gradientChiOld[1]) * (b * *gradientChiOld[1]);
                    double B0 = ((tmpVolField.Summation() + tmpVolField2.Summation()) + deltasquaredOld *
                                 bsquared.Summation()) * forwardModel_->getGrid().GetCellVolume();

                    double derA = double(4.0) * A[1] * B2;
                    double derB = double(3.0) * (A[1] * B1 + A[0] * B2);
                    double derC = double(2.0) * (A[1] * B0 + A[0] * B1 + A0 * B2);
                    double derD = A[0] * B0 + A0 * B1;

                    alpha = this->findRealRootFromCubic(derA, derB, derC, derD);

                    chiEst += alpha*zeta;

                    forwardModel_->calculateResidual(chiEst, pData);

                    res = forwardModel_->calculateResidualNormSq(eta);

                    std::cout << it1+1 << "/" << input.iter1.n << "\t (" << it+1 << "/" << input.n_max << ")\t res: "
                              << std::setprecision(17) << res << std::endl;

                    file << std::setprecision(17) << res << "," << counter << std::endl; // store the residual value in the residual log
                    counter++;
                    fDataOld = res;
                    vecResFirstIter.push_back(res);

                    //breakout check
                    if ( (it1 > 0) && ( (res < double(input.iter1.tolerance)) ||
                                        ( std::abs(vecResFirstIter[it1-1] - res) < double(input.iter1.tolerance) ) ) )
                        break;
//                    std::cout << "Relative Tol: " << res/std::abs(vecResFirstIter[0]) << std::endl;
//                    if ( (it1 > 0) && ( res/std::abs(vecResFirstIter[0]) < 0.15 )  )
//                        break;

                    chiEst.Gradient(gradientChiOld);
                    pressureFieldSerial gradientChiNormsquared(forwardModel_->getGrid());
                    gradientChiNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) +
                            (*gradientChiOld[1] * *gradientChiOld[1]);

                    tmpVolField = (gradientChiNormsquared + deltasquaredOld) / (gradientChiOldNormsquared + deltasquaredOld);
                    fRegOld = (double(1.0) / (forwardModel_->getGrid().GetDomainArea()) ) *
                            tmpVolField.Summation() * forwardModel_->getGrid().GetCellVolume();

                    deltasquaredOld = deltasquared;
                    gOld = g;
                    bsquaredOld = bsquared;
                }
            } // end regularisation loop
        }

        forwardModel_->createTotalField1D(input.iter2, chiEst); // estimate p_tot from the newly estimated chi (chi_est)
    }
    file.close(); // close the residual.log file

    for (int i = 0; i < 2; i++)
    {
        delete gradientChiOld[i];
        delete gradientGregTmp[i];
        delete gradientZetaTmp[i];
    }
    delete[] gradientChiOld;
    delete[] gradientGregTmp;
    delete[] gradientZetaTmp;

    pressureFieldSerial result(forwardModel_->getGrid());
    chiEst.CopyTo(result);
    return result;
}
