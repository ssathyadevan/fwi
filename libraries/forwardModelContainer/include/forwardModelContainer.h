#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include <string>

class ForwardModelContainer
{
    public:
    ForwardModelContainer(const genericInput &genericInput, const std::string &desiredForwardModel, const grid2D &grid, const sources &sources,
        const receivers &receivers, const frequenciesGroup &frequencies);
    ~ForwardModelContainer();

    void createForwardModels(
        const genericInput &gInput, const std::string &desiredForwardModel, const grid2D &grid, const sources &sources, const receivers &receivers);

    void calculateKappaParallel();
    std::vector<std::complex<double>> &calculateResidualParallel(const pressureFieldSerial &chiEstimate, const std::vector<std::complex<double>> &pDataRef);
    double calculateResidualNormSqParallel(const std::vector<std::complex<double>> &residual);

    const frequenciesGroup &getFrequencies() const { return _allFrequencies; }
    const grid2D &getGrid() const { return _forwardmodels[0]->getGrid(); }
    const sources &getSources() const { return _forwardmodels[0]->getSrc(); }
    const receivers &getReceivers() const { return _forwardmodels[0]->getRecv(); }

    private:
    std::vector<forwardModelInterface *> _forwardmodels;
    const int _numberOfThreads;
    const int _numberOfSources;
    const int _numberOfReceivers;
    std::vector<int> _numberOfFrequenciesPerThread;
    std::vector<std::complex<double>> _residuals;
    std::vector<frequenciesGroup> _frequenciesVector;
    frequenciesGroup _allFrequencies;

    void divideFrequencies();
    int computeThreadOffset();
};
