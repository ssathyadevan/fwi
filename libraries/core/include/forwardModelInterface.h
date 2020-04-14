#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"
#include "frequenciesGroup.h"
#include "grid2D.h"
#include "log.h"
#include "receivers.h"
#include "sources.h"

#include <complex>

inline double normSq(const std::vector<std::complex<double>> &data, int n)
{
    double result = double(0.0);
    for(int i = 0; i < n; i++)
    {
        result += std::norm(data[i]);
    }

    return result;
}

class forwardModelInterface
{
public:
    forwardModelInterface(const grid2D &grid, const sources &src, const receivers &recv, const frequenciesGroup &freq);

    virtual ~forwardModelInterface();

    const grid2D &getGrid();
    const sources &getSrc();
    const receivers &getRecv();
    const frequenciesGroup &getFreq();

    virtual void calculatePData(const dataGrid2D &chiEst, std::vector<std::complex<double>> &pData) = 0;
    virtual void calculatePTot(const dataGrid2D &chiEst) = 0;
    virtual void mapDomainToSignal(const dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator) = 0;

    virtual void calculateKappa()
    {
        L_(lerror) << "This ForwardModel is not compatible with the Inversion model";
        exit(EXIT_FAILURE);
    }
    virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, complexDataGrid2D &)
    {
        L_(lerror) << "This ForwardModel is not compatible with the Inversion model";
        exit(EXIT_FAILURE);
    }
    virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &, complexDataGrid2D &, const int, const int)
    {
        L_(lerror) << "This ForwardModel is not compatible with the Inversion model";
        exit(EXIT_FAILURE);
    }

    std::vector<std::complex<double>> &calculateResidual(const dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef);
    double calculateResidualNormSq(std::vector<std::complex<double>> &residual);

private:
    std::vector<std::complex<double>> _residual;

protected:
    const grid2D &_grid;
    const sources &_src;
    const receivers &_recv;
    const frequenciesGroup &_freq;
};
