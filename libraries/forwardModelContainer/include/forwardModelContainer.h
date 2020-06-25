#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include <string>

namespace fwi
{
    class ForwardModelContainer
    {
    public:
        ForwardModelContainer(const io::genericInput &genericInput, const std::string &desiredForwardModel, const core::grid2D &grid,
            const core::Sources &sources, const core::Receivers &receivers, const core::FrequenciesGroup &frequencies);
        ~ForwardModelContainer();

        void createForwardModels(const io::genericInput &gInput, const std::string &desiredForwardModel, const core::grid2D &grid, const core::Sources &sources,
            const core::Receivers &receivers);

        void calculateKappaParallel();
        std::vector<std::complex<double>> &calculateResidualParallel(const core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pDataRef);
        double calculateResidualNormSqParallel(const std::vector<std::complex<double>> &residual);

        const core::FrequenciesGroup &getFrequencies() const { return _allFrequencies; }
        const core::grid2D &getGrid() const { return _forwardmodels[0]->getGrid(); }
        const core::Sources &getSources() const { return _forwardmodels[0]->getSrc(); }
        const core::Receivers &getReceivers() const { return _forwardmodels[0]->getRecv(); }

    private:
        std::vector<forwardModels::forwardModelInterface *> _forwardmodels;
        const int _numberOfThreads;
        const int _numberOfSources;
        const int _numberOfReceivers;
        std::vector<int> _numberOfFrequenciesPerThread;
        std::vector<std::complex<double>> _residuals;
        std::vector<core::FrequenciesGroup> _frequenciesVector;
        core::FrequenciesGroup _allFrequencies;

        void divideFrequencies();
        int computeThreadOffset();
    };
}   // namespace fwi
