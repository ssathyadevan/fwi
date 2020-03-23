#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include <string>

class ForwardModelContainer
{
    public:
    ForwardModelContainer(const GenericInput &gInput, const std::string &desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv,
        const FrequenciesGroup &freq);
    ~ForwardModelContainer();

    void createForwardModels(
        const GenericInput &gInput, const std::string &desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv);
    void calculateKappaParallel();
    std::vector<std::complex<double>> &calculateResidualParallel(const PressureFieldSerial &chiEstimate, const std::vector<std::complex<double>> &pDataRef);
    double calculateResidualNormSqParallel(const std::vector<std::complex<double>> &residual);

    const FrequenciesGroup &getFrequencies();
    const Grid2D &getGrid();
    const Sources &getSources();
    const Receivers &getReceivers();

    private:
    std::vector<ForwardModelInterface *> _forwardmodels;
    const int _numberOfThreads;
    const int _numberOfSources;
    const int _numberOfReceivers;
    std::vector<int> _numberOfFrequencies;
    std::vector<std::complex<double>> _residuals;
    std::vector<FrequenciesGroup> _frequenciesVector;
    FrequenciesGroup _allFrequencies;

    void devideFrequencies();
    int ComputeThreadOffset();
};
