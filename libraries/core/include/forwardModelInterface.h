#ifndef FORWARDMODELINTERFACE_H
#define FORWARDMODELINTERFACE_H

#include "pressureFieldSerial.h"
#include "pressureFieldComplexSerial.h"
#include "grid2D.h"
#include "frequenciesGroup.h"
#include "sources.h"
#include "receivers.h"

#include <complex>

inline double normSq(const std::complex<double> *data, int n)
{
    double result = double(0.0);
    for(int i=0; i<n; i++)
    {
        result += std::norm(data[i]);
    }

    return result;
}

inline double normSq(std::complex<double> *data, int n)
{
    double result = double(0.0);
    for(int i=0; i<n; i++)
    {
        result += std::norm(data[i]);
    }

    return result;
}

class ForwardModelInterface
{

public:
    ForwardModelInterface(const grid2D &grid, const sources &src, const receivers &recv,
                          const frequenciesGroup &freq);

    virtual ~ForwardModelInterface();

    const grid2D& getGrid();
    const sources& getSrc();
    const receivers& getRecv();
    const frequenciesGroup& getFreq();

    virtual void calculatePData(const pressureFieldSerial &chiEst, std::complex<double> *pData) = 0;
    virtual void calculatePTot(const pressureFieldSerial &chiEst) = 0;
    virtual void mapDomainToSignal(const pressureFieldSerial &CurrentPressureFieldSerial, std::complex<double> *kOperator) = 0;

    virtual void calculateKappa() {std::cout << "This ForwardModel is not compatible with the Inversion model" << std::endl; exit(EXIT_FAILURE);}
    virtual void getUpdateDirectionInformation(std::complex<double>* res, pressureFieldComplexSerial &kRes) { std::cout << "This ForwardModel is not compatible with the Inversion model" << std::endl; exit(EXIT_FAILURE); }

    std::complex<double>* calculateResidual(const pressureFieldSerial &chiEst, const std::complex<double> *pDataRef);
    double calculateResidualNormSq(std::complex<double> *residual);

private:

    std::complex<double> *_residual;

protected:
    const grid2D            &_grid;
    const sources           &_src;
    const receivers         &_recv;
    const frequenciesGroup  &_freq;

};

#endif

