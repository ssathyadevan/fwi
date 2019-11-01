#pragma once

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
    ForwardModelInterface(const Grid2D &grid, const Sources &src, const Receivers &recv,
                          const FrequenciesGroup &freq);

    virtual ~ForwardModelInterface();

    const Grid2D& getGrid();
    const Sources& getSrc();
    const Receivers& getRecv();
    const FrequenciesGroup& getFreq();

    virtual void calculatePData(const PressureFieldSerial &chiEst, std::complex<double> *pData) = 0;
    virtual void calculatePTot(const PressureFieldSerial &chiEst) = 0;
    virtual void mapDomainToSignal(const PressureFieldSerial &CurrentPressureFieldSerial, std::complex<double> *kOperator) = 0;

    virtual void calculateKappa() {std::cout << "This ForwardModel is not compatible with the Inversion model" << std::endl; exit(EXIT_FAILURE);}
    virtual void getUpdateDirectionInformation(std::complex<double>* res, PressureFieldComplexSerial &kRes) { std::cout << "This ForwardModel is not compatible with the Inversion model" << std::endl; exit(EXIT_FAILURE); }

    std::complex<double>* calculateResidual(const PressureFieldSerial &chiEst, const std::complex<double> *pDataRef);
    double calculateResidualNormSq(std::complex<double> *residual);

private:

    std::complex<double> *_residual;

protected:
    const Grid2D            &_grid;
    const Sources           &_src;
    const Receivers         &_recv;
    const FrequenciesGroup  &_freq;

};

