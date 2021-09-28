#pragma once

#include "FiniteDifferenceForwardModelInput.h"
#include "ForwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"

namespace fwi
{
    namespace forwardModels
    {
        class FiniteDifferenceForwardModel : public ForwardModelInterface
        {
        public:
            FiniteDifferenceForwardModel(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
                const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput);

            ~FiniteDifferenceForwardModel();

            virtual std::vector<std::complex<double>> calculatePressureField(const core::dataGrid2D<double> &chiEst);

            void calculateKappa();
            virtual void calculatePTot(const core::dataGrid2D<double> &chiEst);
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes);
            virtual void getUpdateDirectionInformationMPI(
                std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes, const int offset, const int block_size);
            virtual void getResidualGradient(std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes);

            const core::grid2D &getGrid() { return _grid; }

            const core::Sources &getSource() { return _source; }

            const core::Receivers &getReceiver() { return _receiver; }

            const core::FrequenciesGroup &getFreq() { return _freq; }

            /**
             * This vector contains the kernels for all combinations of frequencies, receivers, and sources.
             *
             * It is laid out using the following logic:
             *   - Frequency  x
             *     - Receiver y
             *       - Source z
             *
             * @example
             * E.g., for a problem with 5 frequencies, 4 receivers and 4 sources:
             * - index 0 contains the kernel for frequencies 0, receiver 0, source 0;
             * - index 1 contains the kernel for frequencies 0, receiver 0, source 1;
             * - index 4 contains the kernel for frequencies 0, receiver 1, source 0;
             * - index 16 contains the kernel for frequencies 1, receiver 0, source 0;
             * - etc...
             *
             * @brief the vector of all kernels, laid out from frequencies -> receiver -> source. See docs for more details.
             * @return a vector containing the kernels for all combinations of frequencies, receivers and sources.
             */
            const std::vector<core::dataGrid2D<std::complex<double>>> &getKernel() { return _vkappa; }

        private:
            void createPTot(const core::FrequenciesGroup &freq, const core::Sources &source);

            void createGreens();

            core::dataGrid2D<std::complex<double>> calcTotalField(
                const core::greensRect2DCpu &G, const core::dataGrid2D<double> &chiEst, const core::dataGrid2D<std::complex<double>> &Pinit);

            void applyKappa(const core::dataGrid2D<double> &CurrentPressureFieldSerial, std::vector<std::complex<double>> &pData);
            void createKappa(const core::FrequenciesGroup &freq, const core::Sources &source, const core::Receivers &receiver);

        private:
            std::vector<std::complex<double>> _residual;
            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;
            std::vector<core::greensRect2DCpu> _Greens;

            std::vector<core::dataGrid2D<std::complex<double>>> _vpTot;
            std::vector<core::dataGrid2D<std::complex<double>>> _vkappa;

            finiteDifferenceForwardModelInput _fMInput;
            friend class FiniteDifferenceForwardModelParallel;
        };

    }   // namespace forwardModels
}   // namespace fwi
