#include "gradientDescentInversion.h"
gradientDescentInversion::gradientDescentInversion(ForwardModelInterface *forwardModel, const gradientDescentInversionInput &gdInput)
    :_forwardModel(),_gdInput(), _grid(forwardModel->getGrid()), _src(forwardModel->getSrc()),_recv(forwardModel->getRecv()), _freq(forwardModel->getFreq())
{
    _forwardModel = forwardModel;
    _gdInput = gdInput;
}

pressureFieldSerial gradientDescentInversion::Reconstruct(const std::complex<double> *const pData, genericInput gInput)
{
    const int nTotal = _freq.nFreq * _src.nSrc * _recv.nRecv;

    double eta = 1.0 / (normSq(pData, nTotal)); //scaling factor eq 2.10 in thesis
    double gamma, alpha, res, resSq;

    std::array<double,2> alphaDiv;

    pressureFieldSerial chiEst(_grid), g(_grid), gOld(_grid), zeta(_grid);
    pressureFieldComplexSerial tmp(_grid); // eq: integrandForDiscreteK, tmp is the argument of Re()

    chiEst.Zero();
    // open the file to store the residual log
    std::ofstream file;
    file.open (gInput.outputLocation + gInput.runName + "Residual.log", std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout<< "Failed to open the file to store residuals" << std::endl;
        std::exit(EXIT_FAILURE);
    }
     std::vector<std::complex<double>> vecResFirstIter;
    _forwardModel->calculateKappa();




    std::complex<double>* resArray = _forwardModel->calculateResidual(chiEst, pData);// Fx to minimize;
    for (int it1 = 0; it1 < _gdInput.iter; it1++)
    {
        resSq = _forwardModel->calculateResidualNormSq(resArray);
        res = eta * resSq;
        if (it1 > 0)
        {
            std::cout << "inner loop residual = " << std::setprecision(17) << res << "     " <<
                         std::abs(vecResFirstIter[it1-1] - res) << "    " << it1+1 << '/' << _gdInput.iter << std::endl;
        }
        else
        {
            std::cout << "inner loop residual = " << std::setprecision(17) << res << std::endl;
        }
        vecResFirstIter.push_back(res);



        alphaDiv[0] = 0.0;
        alphaDiv[1] = 0.0;

        std::complex<double>* zetaTemp = new std::complex<double>[_freq.nFreq * _src.nSrc * _recv.nRecv];
        _forwardModel->mapDomainToSignal(zeta, zetaTemp);

        for (int i = 0; i < nTotal; i++)
        {
            alphaDiv[0] += std::real( conj(resArray[i]) * zetaTemp[i] );
            alphaDiv[1] += std::real( conj(zetaTemp[i]) * zetaTemp[i] );
        }
        alpha = alphaDiv[0] / alphaDiv[1];
        chiEst += alpha*zeta; // the step size of the parameter in Eq: ContrastUpdate in the user manual.
        gOld = g;

        delete[] zetaTemp;
    }

}
std::vector<double> gradientDescentInversion::differential(const std::complex<double> *const pData, pressureFieldSerial xi, double dxi){

    std::vector<double> dFdx(xi.GetNumberOfGridPoints());
    for (int i = 0; i < xi.GetNumberOfGridPoints(); i++){
        pressureFieldSerial xidxi = xi;
        xidxi.GetDataPtr()[i] += dxi;
        double Fx = functionF(xi, pData);
        double Fxdx = functionF(xidxi, pData);
        dFdx[i] = (Fxdx - Fx)/dxi;
    }
    return dFdx;
}
double gradientDescentInversion::functionF(pressureFieldSerial xi, const std::complex<double> *const pData){
    return _forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(xi,pData));
}

pressureFieldSerial gradientDescentInversion::gradientDescent(const std::complex<double> *const pData, pressureFieldSerial xi){
    double dxi = _gdInput.dx;
    int iter = _gdInput.iter;
    double gamma = _gdInput.gamma;
    for(int j = 0; j < xi.GetNumberOfGridPoints(); j++){
        std::vector<double> nablaFxi = differential(pData,xi,dxi);
        xi.GetDataPtr()[j] -= gamma * nablaFxi[j];
    }
    return xi;
}
