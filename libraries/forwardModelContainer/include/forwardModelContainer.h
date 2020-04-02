#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include <string>

class ForwardModelContainer
{
    public:
    ForwardModelContainer(const GenericInput &genericInput, const std::string &desiredForwardModel, const Grid2D &grid, const Sources &sources,
        const Receivers &receivers, const FrequenciesGroup &frequencies);
    ~ForwardModelContainer();

    void createForwardModels(
        const GenericInput &gInput, const std::string &desiredForwardModel, const Grid2D &grid, const Sources &sources, const Receivers &receivers);

    void calculateKappaParallel();
    std::vector<std::complex<double>> &calculateResidualParallel(const PressureFieldSerial &chiEstimate, const std::vector<std::complex<double>> &pDataRef);
    double calculateResidualNormSqParallel(const std::vector<std::complex<double>> &residual);

    const FrequenciesGroup &getFrequencies() const { return _allFrequencies; }
    const Grid2D &getGrid() const { return _forwardmodels[0]->getGrid(); }
    const Sources &getSources() const { return _forwardmodels[0]->getSrc(); }
    const Receivers &getReceivers() const { return _forwardmodels[0]->getRecv(); }

    private:
    std::vector<ForwardModelInterface *> _forwardmodels;
    const int _numberOfThreads;
    const int _numberOfSources;
    const int _numberOfReceivers;
    std::vector<int> _numberOfFrequenciesPerThread;
    std::vector<std::complex<double>> _residuals;
    std::vector<FrequenciesGroup> _frequenciesVector;
    FrequenciesGroup _allFrequencies;

    void divideFrequencies();
    int computeThreadOffset();
};
