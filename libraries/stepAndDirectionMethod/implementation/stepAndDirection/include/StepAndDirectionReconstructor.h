#pragma once
#include "CommonVectorOperations.h"
#include "CostFunctionCalculator.h"
#include "DirectionCalculator.h"
#include "ForwardModelInterface.h"
#include "ReconstructorParameters.h"
#include "StepSizeCalculator.h"
#include "genericInput.h"

namespace fwi
{
    namespace inversionMethods
    {
        class StepAndDirectionReconstructor
        {
        private:
            StepSizeCalculator *_desiredStep;
            DirectionCalculator *_desiredDirection;
            forwardModels::ForwardModelInterface *_forwardModel;
            const core::CostFunctionCalculator &_costCalculator;
            const ReconstructorParameters &_directionInput;
            /**  _directionInput contains the specific values for the particular method adopted through the choice a Direction Calculator */
            const core::grid2D &_grid;

            core::dataGrid2D calculateNextMove(const core::dataGrid2D &chiEstimate, const core::dataGrid2D &direction, double step) const;
            double calculateResidualNorm(const std::vector<std::complex<double>> &pData, double eta) const;

        public:
            StepAndDirectionReconstructor(StepSizeCalculator *desiredStep, DirectionCalculator *desiredDirection,
                const core::CostFunctionCalculator &costCalculator, forwardModels::ForwardModelInterface *forwardModel,
                const ReconstructorParameters &directionInput);

            ~StepAndDirectionReconstructor(){}

            /**
             * @brief reconstruct The main method of the class, where the desired steps and directions are combined and the reconstruction happens
             * @param pData is the data coming from the readings
             * @param gInput is the data input for the problem
             * @return
             */
            core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput);
        };
    }   // namespace inversionMethods
}   // namespace fwi
