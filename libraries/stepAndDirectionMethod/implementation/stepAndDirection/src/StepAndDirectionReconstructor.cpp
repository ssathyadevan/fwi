#include "StepAndDirectionReconstructor.h"
#include "progressBar.h"

using fwi::core::operator-;

namespace fwi
{
    namespace inversionMethods
    {
        StepAndDirectionReconstructor::StepAndDirectionReconstructor(StepSizeCalculator *desiredStep, DirectionCalculator *desiredDirection,
            forwardModels::ForwardModelInterface *forwardModel, const ReconstructorParameters &directionInput)
            : _desiredStep(desiredStep)
            , _desiredDirection(desiredDirection)
            , _forwardModel(forwardModel)
            , _directionInput(directionInput)
            , _grid(forwardModel->getGrid())
        {
        }

        core::dataGrid2D StepAndDirectionReconstructor::reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput)
        {
            io::progressBar bar(_directionInput.maxIterationsNumber);

            std::ofstream file(gInput.outputLocation + gInput.runName + "Residual.log");

            double step = 0.0;

            double eta = _desiredDirection->getErrorFunctionalScalingFactor();

            core::dataGrid2D chiEstimateCurrent(_grid);
            chiEstimateCurrent = _directionInput.startingChi;

            std::vector<std::complex<double>> pDataEst(pData.size());
            _forwardModel->calculatePData(chiEstimateCurrent, pDataEst);
            std::vector<std::complex<double>> residualVector = pData - pDataEst;
            double residualValue = calculateResidualNorm(residualVector, eta);

            _forwardModel->calculateKappa();

            core::dataGrid2D const *directionCurrent;

            int kappaTimesDirectionSize = _forwardModel->getFreq().count * _forwardModel->getSource().count * _forwardModel->getReceiver().count;
            std::vector<std::complex<double>> kappaTimesDirection(kappaTimesDirectionSize);

            for(int it = 0; it < _directionInput.maxIterationsNumber; ++it)
            {
                directionCurrent = &_desiredDirection->calculateDirection(chiEstimateCurrent, residualVector);

                // here we compute kappaTimesDirection, which is used only in ConjugateGradientStepSize.
                _forwardModel->mapDomainToSignal(*directionCurrent, kappaTimesDirection);

                _desiredStep->updateVariables(
                    chiEstimateCurrent, *directionCurrent, it, kappaTimesDirection, residualVector);   // update all StepSizeCalculator with last two items

                step = _desiredStep->calculateStepSize();

                chiEstimateCurrent = calculateNextMove(chiEstimateCurrent, *directionCurrent, step);

                _forwardModel->calculatePData(chiEstimateCurrent, pDataEst);
                residualVector = pData - pDataEst;
                residualValue = calculateResidualNorm(residualVector, eta);
                file << std::setprecision(17) << residualValue << "," << it + 1 << std::endl;

                if(residualValue < _directionInput.tolerance)
                {
                    file << "Reconstruction terminated at iteration " << it + 1 << " because error is smaller than input tolerance " << std::setprecision(17)
                         << _directionInput.tolerance << std::endl;
                    break;
                }

                bar++;
            }

            return chiEstimateCurrent;
        }

        core::dataGrid2D StepAndDirectionReconstructor::calculateNextMove(
            const core::dataGrid2D &chiEstimate, const core::dataGrid2D &direction, double step) const
        {
            core::dataGrid2D chiTemp = chiEstimate;
            const int nGridPoints = chiEstimate.getNumberOfGridPoints();
            std::vector<double> descentVector(nGridPoints, 0.0);
            const std::vector<double> &directionData = direction.getData();

            for(int i = 0; i < nGridPoints; ++i)
            {   // make sure the DirectionCalculators have the -1 integrated
                descentVector[i] = step * directionData[i];
            }
            chiTemp += descentVector;

            return chiTemp;
        }

        double StepAndDirectionReconstructor::calculateResidualNorm(const std::vector<std::complex<double>> &residualVector, double eta) const
        {
            return eta * core::l2NormSq(residualVector);
        }
    }   // namespace inversionMethods
}   // namespace fwi
