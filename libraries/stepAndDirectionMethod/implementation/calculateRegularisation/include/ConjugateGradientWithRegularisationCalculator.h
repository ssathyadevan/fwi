#pragma once
#include "ConjugateGradientRegularisationParameters.h"
#include "ConjugateGradientWithRegularisationParametersInput.h"
#include "CostFunctionCalculator.h"
#include "StepAndDirectionReconstructor.h"

namespace fwi
{
    namespace inversionMethods
    {
        /**
         * @brief The ConjugateGradientWithRegularisationCalculator class inherits both from DirectionCalculator and StepSizeCalculator, so that it is possible
         * to update _directionCurrent while computing the step stepSize. Unless specifically mentioned, we refer to
         * doc/BackGroundInfo/phd-Peter-Haffinger-seismic-broadband-full-waveform-inversion.pdf for every equation
         */
        class ConjugateGradientWithRegularisationCalculator : public DirectionCalculator, public StepSizeCalculator
        {
        private:
            // StepSize part
            core::dataGrid2D _chiEstimatePrevious;
            core::dataGrid2D _chiEstimateCurrent;
            core::dataGrid2D _gradientPrevious;
            core::dataGrid2D _gradientCurrent;
            int _iterationNumber;

            core::CostFunctionCalculator _costCalculator;
            // Direction part
            const std::vector<std::complex<double>> &_pData;

            // Inversion part
            ConjugateGradientWithRegularisationParametersInput _cgParametersInput;
            const core::grid2D &_grid;
            int _nTotal;   // nFreq * source.count * receiver.count

            core::dataGrid2D _directionPrevious;   // in the documentation the direction is indicated with the greek letter Zeta
            core::dataGrid2D _directionCurrent;
            core::complexDataGrid2D _kappaTimesResidual;
            ConjugateGradientRegularisationParameters _regularisationPrevious;
            ConjugateGradientRegularisationParameters _regularisationCurrent;
            double _residualValueCurrent;
            double _residualValuePrevious;
            double _deltaAmplification;
            std::vector<std::complex<double>> _residualVector;

            /**
             * @brief calculateRegularisationParameters updates all of the parameters of _regularisationCurrent that can be computed from previous iteration
             */
            void calculateRegularisationParameters();

            /**
             * @brief updateResidual computes and updates _residualVector and _residualValueCurrent
             */
            void updateResidual();

            /**
             * @brief calculateRegularisationStep is the main function, where the inner loop acts. It is called by calculateStepSize()
             * @return the StepSize to be sent to StepAndDirectionReconstructor
             */
            double calculateRegularisationStep();

            /**
             * @brief calculateGammaPolakRibiere computes the optimal step size according to the Polak-Ribiere formula, see eq. 2.14.
             * @return the optimal stepsize without regularisation
             */
            double calculateGammaPolakRibiere();

            /**
             * @brief calculateWeightingFactor computes equation 2.22
             * @return the value for _regularisationCurrent.bSquared, which will be needed to approximate the regularisation cost Function in
             * calculateStepSizeInRegularisation(), see eq. 2.26-2.27
             */
            core::dataGrid2D calculateWeightingFactor();

            /**
             * @brief calculateStepSizeInRegularisation prepares the parameters for equation 2.26 (see eq. 2.27) and invokes findRealRootFromCubic(...) to find
             * the optimal StepSize
             * @return the output of findRealRootFromCubic(...), which is the final StepSize value to be sent to StepAndDirectionReconstructor
             */
            double calculateStepSizeInRegularisation();

            /**
             * @brief calculateDirectionInRegularisation updates _kappaTimesResidual and _gradientCurrent following equations 2.13 and 2.25
             * @return the new direction
             */
            core::dataGrid2D calculateDirectionInRegularisation();

            /**
             * @brief calculateSteeringFactor is a modified version of equation 2.23
             * @return the new value of _regularisationCurrent.deltaSquared
             */
            double calculateSteeringFactor();

            /**
             * @brief findRealRootFromCubic applies the Cardano formula to solve third order polynomial a*x^3 +b*x^2 +c*x +d =0
             * @param a the cubic coefficient
             * @param b the quadratic coefficient
             * @param c the linear coefficient
             * @param d the constant coefficient
             * @return  the (in this case) only real root
             */
            double findRealRootFromCubic(double a, double b, double c, double d);

            /**
             * @brief calculateRegularisationErrorFunctional updates _regularisationPrevious.errorFunctional after having computed
             * _regularisationCurrent.gradientChi
             */
            void calculateRegularisationErrorFunctional();

            /**
             * @brief calculateRegularisationGradient computes the derivative of the regularisation cost Function by only using _regularisationPrevious.bSquared
             * and .gradientChi, see equation 2.24
             * @return updates _regularisationCurrent.gradient
             */
            core::dataGrid2D calculateRegularisationGradient();

        public:
            ConjugateGradientWithRegularisationCalculator(double errorFunctionalScalingFactor, forwardModels::forwardModelInterface *forwardModel,
                ConjugateGradientWithRegularisationParametersInput cgParametersInput, const std::vector<std::complex<double>> &pData);   // constructor
            virtual ~ConjugateGradientWithRegularisationCalculator() {}                                                                  // destructor

            /**
             * @brief calculateStepSize Invokes the whole regularisation process, where also the direction is updated
             * @return the final optimized StepSize for the internally updated _directionCurrent
             */
            double calculateStepSize() override;

            /**
             * @brief calculateDirection performs the computation described in eq. 2.13
             * @param residualVector is saved into the member variable _residualVector
             * @return is_directionCurrent. We output a reference so that the value is updated as we perform at a later stage the regularisation process
             * during the call of calculateStepSize
             */
            core::dataGrid2D &calculateDirection(const core::dataGrid2D &, const std::vector<std::complex<double>> &residualVector) override;

            /**
             * @brief updateVariables updates _chiEstimateCur/Pr and _iterationNumber based on the parameters
             * @param chiEstimateCurrent
             * @param iterationNumber
             */
            void updateVariables(const core::dataGrid2D &chiEstimateCurrent, const core::dataGrid2D &, int iterationNumber,
                const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &) override;
        };
    }   // namespace inversionMethods
}   // namespace fwi
