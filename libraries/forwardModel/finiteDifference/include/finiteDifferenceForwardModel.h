#pragma once

#include "finiteDifferenceForwardModelInput.h"
#include "forwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"

namespace fwi
{
    namespace forwardModels
    {
        class finiteDifferenceForwardModel : public forwardModelInterface
        {
        public:
            finiteDifferenceForwardModel(const core::grid2D &grid, const core::sources &src, const core::receivers &recv, const core::frequenciesGroup &freq,
                const finiteDifferenceForwardModelInput &fmInput);

            ~finiteDifferenceForwardModel();

            virtual void calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &kOperator);

            void calculateKappa();
            virtual void calculatePTot(const core::dataGrid2D &chiEst);
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes);
            virtual void getUpdateDirectionInformationMPI(
                std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes, const int offset, const int block_size);
            virtual void getResidualGradient(std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes);
            virtual void mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator);

        private:
            core::greensRect2DCpu **_Greens;

            core::complexDataGrid2D ***_p0;
            core::complexDataGrid2D **_pTot;
            core::complexDataGrid2D **_kappa;
            finiteDifferenceForwardModelInput _fMInput;

            void createP0();
            void deleteP0();

            void createPTot(const core::frequenciesGroup &freq, const core::sources &src);

            void createGreens();
            void deleteGreens();

            void deletePtot();

            core::complexDataGrid2D calcTotalField(const core::greensRect2DCpu &G, const core::dataGrid2D &chiEst, const core::complexDataGrid2D &Pinit);

            void applyKappa(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &pData);
            void createKappa(const core::frequenciesGroup &freq, const core::sources &src, const core::receivers &recv);
            void deleteKappa();
        };

    }   // namespace forwardModels
}   // namespace fwi
