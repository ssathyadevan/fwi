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


    pressureFieldSerial chiEst(_grid), chiOld(_grid), g(_grid), gOld(_grid), zeta(_grid);
    pressureFieldComplexSerial tmp(_grid); // eq: integrandForDiscreteK, tmp is the argument of Re()
    std::vector<double>dfdxi(_grid.GetNumberOfGridPoints(),0);
    chiEst.Zero();
    chiOld.Zero();
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
        std::cout << "Iteration number: " << it1+1 << std::endl;
        vecResFirstIter.push_back(res);

        std::vector<double> dfdxiOld = dfdxi;
        dfdxi= differential(pData,chiEst,_gdInput.dx,eta);
        if( it1 > 0){
            //gamma = getGamma(dfdxiOld,dfdxi,chiOld,chiEst);
        }
        else{
            gamma = _gdInput.gamma;
        }
        std::cout << "Gamma:" << gamma << std::endl;
        chiOld = chiEst;
        chiEst = gradientDescent(pData, chiEst, dfdxi, gamma, eta);

    }
    return chiEst;
}
std::vector<double> gradientDescentInversion::differential(const std::complex<double> *const pData, pressureFieldSerial xi, double dxi,double eta){

    std::vector<double> dFdx(xi.GetNumberOfGridPoints());
    double Fx = functionF(xi, pData, eta);
    pressureFieldSerial xidxi = xi;
    std::cout << "The residual is :" << Fx << std::endl;
    for (int i = 0; i < xi.GetNumberOfGridPoints(); i++){



        xidxi.GetDataPtr()[i] += dxi;

        double Fxdx = functionF(xidxi, pData, eta);
        dFdx[i] = (Fxdx - Fx)/dxi;
        xidxi.GetDataPtr()[i] = xi.GetDataPtr()[i];
    }
    return dFdx;
}
double gradientDescentInversion::functionF(pressureFieldSerial xi, const std::complex<double> *const pData, double eta){
    return eta *_forwardModel->calculateResidualNormSq(_forwardModel->calculateResidual(xi,pData));
}

pressureFieldSerial gradientDescentInversion::gradientDescent(const std::complex<double> *const pData, pressureFieldSerial xi, std::vector<double> nablaFxi , double gamma, double eta){
    for(int j = 0; j < xi.GetNumberOfGridPoints(); j++){
        xi.GetDataPtr()[j] -= gamma * nablaFxi[j];
    }
    return xi;
}

double gradientDescentInversion::getGamma(std::vector<double> dFdxiOld, std::vector<double> dFdxi, pressureFieldSerial xiOld, pressureFieldSerial xi){

    double deltaxiDeltadFdxiSum = 0;
    double deltadFdxiSquaredSum = 0;

    for ( int i = 0; i < xi.GetNumberOfGridPoints(); i++){
        double deltadFdXi = dFdxi[i] - dFdxiOld[i];
        double deltaxi = xi.GetDataPtr()[i]-xiOld.GetDataPtr()[i];
        deltaxiDeltadFdxiSum+= deltaxi * deltadFdXi;
        deltadFdxiSquaredSum += pow((deltadFdXi * deltaxi),2);
    }
    return fabs(deltaxiDeltadFdxiSum)/deltadFdxiSquaredSum;
}
