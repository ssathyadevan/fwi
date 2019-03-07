#include "conjugateGradientInversion.h"

conjugateGradientInversion::conjugateGradientInversion(forwardModelBasicOptimization *forwardModel, const conjugateGradientInput &cgInput)
    :m_forwardModel(), m_cgInput(), m_grid(forwardModel->getGrid()), m_src(forwardModel->getSrc()), m_recv(forwardModel->getRecv()), m_freq(forwardModel->getFreq())
{
    m_forwardModel = forwardModel;
    m_cgInput = cgInput;
}

double conjugateGradientInversion::findRealRootFromCubic(double a, double b, double c, double d)
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


pressureFieldSerial conjugateGradientInversion::Reconstruct(const std::complex<double> *const pData, genericInput gInput)
{
    const int nTotal = m_freq.nFreq * m_src.nSrc * m_recv.nRecv;

    double eta = 1.0 / (normSq(pData, nTotal)); //scaling factor eq 2.10 in thesis
    double gamma, alpha, resErrFunc, resSq;

    std::array<double,2> alphaDiv;

    pressureFieldSerial chiEst(m_grid), g(m_grid), gOld(m_grid), zeta(m_grid);
    pressureFieldComplexSerial tmp(m_grid); // eq: integrandForDiscreteK, tmp is the argument of Re()

    chiEst.Zero();

    pressureFieldSerial **gradientChiOld  = new pressureFieldSerial*[2];
    pressureFieldSerial **gradientGregTmp = new pressureFieldSerial*[2];
    pressureFieldSerial **gradientZetaTmp = new pressureFieldSerial*[2];

    for (int i = 0; i < 2; i++)
    {
         gradientChiOld[i] = new pressureFieldSerial(m_grid);
        gradientGregTmp[i] = new pressureFieldSerial(m_grid);
        gradientZetaTmp[i] = new pressureFieldSerial(m_grid);
    }

    // open the file to store the residual log
    std::ofstream file;
    file.open (gInput.outputLocation+gInput.runName+"Residual.log", std::ios::out | std::ios::trunc);

    if (!file)
    {
        std::cout<< "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int counter = 1;

    //main loop//
    for(int it = 0; it < m_cgInput.n_max; it++)
    {
        std::vector<std::complex<double>> vecResFirstIter;

        if (m_cgInput.doReg == 0)
        {
            m_forwardModel->initializeForwardModel(chiEst);

            std::complex<double>* resArray = m_forwardModel->calculateResidual(chiEst, pData);

            for (int it1 = 0; it1 < m_cgInput.iteration1.n; it1++)
            {  
                resSq = m_forwardModel->calculateResidualNormSq(resArray);
                resErrFunc = eta * resSq;

                {
                    if (it1 > 0)
                    {
                        std::cout << "inner loop residual = " << std::setprecision(17) << resErrFunc << "     " <<
                                     std::abs(vecResFirstIter[it1-1] - resErrFunc) << "    " << it1+1 << '/' << m_cgInput.iteration1.n << std::endl;
                    }
                    else
                    {
                        std::cout << "inner loop residual = " << std::setprecision(17) << resErrFunc << std::endl;
                    }
                }

                vecResFirstIter.push_back(resErrFunc);

                if ( (it1 > 0) && ( (resErrFunc < double(m_cgInput.iteration1.tolerance)) ||
                                    ( std::abs(vecResFirstIter[it1-1] - resErrFunc) < double(m_cgInput.iteration1.tolerance) ) ) )
                {
                    break;
                }

                m_forwardModel->calculateOperatorMapStoD(tmp, resArray);

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

                auto zetaTemp = m_forwardModel->createKappaOperator(zeta);

                for (int i = 0; i < nTotal; i++)
                {
                    alphaDiv[0] += std::real( conj(resArray[i]) * zetaTemp[i] );
                    alphaDiv[1] += std::real( conj(zetaTemp[i]) * zetaTemp[i] );
                }
                alpha = alphaDiv[0] / alphaDiv[1];
                chiEst += alpha*zeta; // the step size of the parameter in Eq: ContrastUpdate in the user manual.
                gOld = g;
            }
        }
        else if (m_cgInput.doReg == 1)
        {
            double deltasquaredOld      = double(0.0);
            double fRegOld              = double(1.0);
            double fDataOld             = double(0.0);
            double deltaAmplification   = m_cgInput.dAmplification.start / (m_cgInput.dAmplification.slope * it + double(1.0) );

            pressureFieldSerial bsquaredOld(m_grid);
            bsquaredOld.Zero();

            m_forwardModel->initializeForwardModel(chiEst);

            std::complex<double>* resArray = m_forwardModel->calculateResidual(chiEst, pData);

            //start the inner loop
            for (int it1 = 0; it1 < m_cgInput.iteration1.n; it1++)
            {
                if (it1 == 0)
                {
                    m_forwardModel->calculateOperatorMapStoD(tmp, resArray);

                    g = eta * tmp.GetRealPart(); //eq: gradientRunc
                    zeta = g;

                    alphaDiv[0] = double(0.0);
                    alphaDiv[1] = double(0.0);

                    auto *zetaTemp = m_forwardModel->createKappaOperator(zeta);

                    for (int i = 0; i < nTotal; i++)
                    {
                        alphaDiv[0] += std::real( conj(resArray[i]) * zetaTemp[i] );
                        alphaDiv[1] += std::real( conj(zetaTemp[i]) * zetaTemp[i] );
                    }

                    alpha   = alphaDiv[0] / alphaDiv[1]; //eq:optimalStepSizeCG in the readme pdf
                    chiEst += alpha * zeta;
                    gOld    = g;

                    resArray = m_forwardModel->calculateResidual(chiEst, pData);
                    resSq = m_forwardModel->calculateResidualNormSq(resArray);

                    resErrFunc = eta * resSq;

                    std::cout << it1+1 << "/" << m_cgInput.iteration1.n << "\t (" << it+1 << "/" <<m_cgInput.n_max << ")\t res: " << std::setprecision(17) << resErrFunc << std::endl;

                    file << std::setprecision(17) << resErrFunc << "," << counter << std::endl;
                    counter++;// store the residual value in the residual log

                    fDataOld = resErrFunc;
                    vecResFirstIter.push_back(resErrFunc);

                    }
                    else
                    {

                    chiEst.Gradient(gradientChiOld);
                    pressureFieldSerial gradientChiOldNormsquared(m_grid);

                    gradientChiOldNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) + (*gradientChiOld[1] * *gradientChiOld[1]);

                    pressureFieldSerial bsquared = ( gradientChiOldNormsquared + deltasquaredOld );// eq: errorFuncRegulWeighting
                    bsquared.Reciprocal();
                    bsquared *= (double(1.0) / (m_grid.GetDomainArea()) );// # eq. 2.22
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

                    m_forwardModel->calculateOperatorMapStoD(tmp, resArray);

                    g = eta * fRegOld * tmp.GetRealPart() + fDataOld * gReg; // # eq: integrandForDiscreteK

                    gamma = g.InnerProduct(g - gOld) / gOld.InnerProduct(gOld);// # eq: PolakRibiereDirection
                    zeta = g + gamma*zeta;

                    std::array<double,2> A = {0.0, 0.0};
                    auto zetaTemp = m_forwardModel->createKappaOperator(zeta);

                    for (int i = 0; i < nTotal; i++)
                    {
                        A[1] += eta * std::real( conj(zetaTemp[i]) * zetaTemp[i]);
                        A[0] += double(-2.0) * eta * std::real( conj(resArray[i]) * zetaTemp[i]);
                    }

                    double A0 = fDataOld;
                    zeta.Gradient(gradientZetaTmp);
                    tmpVolField = b * *gradientZetaTmp[0];
                    tmpVolField2 = b * *gradientZetaTmp[1];
                    tmpVolField.Square();
                    tmpVolField2.Square();
                    double B2 = (tmpVolField.Summation() + tmpVolField2.Summation()) * m_grid.GetCellVolume();

                    tmpVolField = (b * *gradientZetaTmp[0]) * (b * *gradientChiOld[0]);
                    tmpVolField2 = (b * *gradientZetaTmp[1]) * (b * *gradientChiOld[1]);
                    double B1 = double(2.0) * (tmpVolField.Summation() + tmpVolField2.Summation()) *
                            m_grid.GetCellVolume();

                    tmpVolField = (b * *gradientChiOld[0]) * (b * *gradientChiOld[0]);
                    tmpVolField2 = (b * *gradientChiOld[1]) * (b * *gradientChiOld[1]);
                    double B0 = ((tmpVolField.Summation() + tmpVolField2.Summation()) + deltasquaredOld *
                                 bsquared.Summation()) * m_forwardModel->getGrid().GetCellVolume();

                    double derA = double(4.0) * A[1] * B2;
                    double derB = double(3.0) * (A[1] * B1 + A[0] * B2);
                    double derC = double(2.0) * (A[1] * B0 + A[0] * B1 + A0 * B2);
                    double derD = A[0] * B0 + A0 * B1;

                    alpha = this->findRealRootFromCubic(derA, derB, derC, derD);

                    chiEst += alpha * zeta;

                    resSq = m_forwardModel->calculateResidualNormSq(resArray);
                    resErrFunc = eta * resSq;

                    std::cout << it1+1 << "/" << m_cgInput.iteration1.n << "\t (" << it+1 << "/" <<m_cgInput.n_max << ")\t res: "
                              << std::setprecision(17) << resErrFunc << std::endl;

                    file << std::setprecision(17) << resErrFunc << "," << counter << std::endl; // store the residual value in the residual log
                    counter++;
                    fDataOld = resErrFunc;
                    vecResFirstIter.push_back(resErrFunc);

                    //breakout check
                    if ( (it1 > 0) && ( (resErrFunc < double(m_cgInput.iteration1.tolerance)) ||
                                        ( std::abs(vecResFirstIter[it1-1] - resErrFunc) < double(m_cgInput.iteration1.tolerance) ) ) )
                        break;
//                    std::cout << "Relative Tol: " << res/std::abs(vecResFirstIter[0]) << std::endl;
//                    if ( (it1 > 0) && ( res/std::abs(vecResFirstIter[0]) < 0.15 )  )
//                        break;

                    chiEst.Gradient(gradientChiOld);
                    pressureFieldSerial gradientChiNormsquared(m_grid);
                    gradientChiNormsquared = (*gradientChiOld[0] * *gradientChiOld[0]) +
                            (*gradientChiOld[1] * *gradientChiOld[1]);

                    tmpVolField = (gradientChiNormsquared + deltasquaredOld) / (gradientChiOldNormsquared + deltasquaredOld);
                    fRegOld = (double(1.0) / (m_grid.GetDomainArea()) ) *
                            tmpVolField.Summation() * m_grid.GetCellVolume();

                    deltasquaredOld = deltasquared;
                    gOld = g;
                    bsquaredOld = bsquared;
                }
            } // end regularisation loop
        }

        //_forwardModel->createTotalField1D(chiEst); // estimate p_tot from the newly estimated chi (chi_est)
        m_forwardModel->postInitializeForwardModel(chiEst);
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

    pressureFieldSerial result(m_grid);
    chiEst.CopyTo(result);
    return result;
}
