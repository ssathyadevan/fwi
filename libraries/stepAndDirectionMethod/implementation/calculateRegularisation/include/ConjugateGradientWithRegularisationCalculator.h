#pragma once
#include "ConjugateGradientRegularisationParameters.h"
#include "ConjugateGradientWithRegularisationParametersInput.h"
#include "StepAndDirectionReconstructor.h"

class ConjugateGradientWithRegularisationCalculator : public DirectionCalculator, public StepSizeCalculator
{
private:
    // StepSize part
    dataGrid2D _chiEstimatePrevious;
    dataGrid2D _chiEstimateCurrent;

    dataGrid2D _gradientPrevious;
    dataGrid2D _gradientCurrent;
    int _iterationNumber;

    // Direction part
    const std::vector<std::complex<double>> &_pData;

    // Inversion part
    ConjugateGradientWithRegularisationParametersInput _cgParametersInput;
    const grid2D &_grid;
    int _nTotal;   // nFreq * nSrc * nRecv

    dataGrid2D _zetaPrevious;   //_zeta IS the direction!
    dataGrid2D _zetaCurrent;
    complexDataGrid2D _kappaTimesResidual;
    double _gamma;
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
     * @brief updateResidual computes and updates _residualVector
     * @return residualValue to be put in _residualValueCurrent
     */
    void updateResidual();

    /**
     * @brief calculateRegularisationStep is the main function, where the inner loop acts. It is called by calculateStepSize(...)
     * @return the StepSize alpha
     */
    double calculateRegularisationStep();

    /**
     * @brief calculateGammaPolakRibiere computes the optimal step size according to the Polak-Ribiere formula.
     * @return the optimal stepsize without regularisation
     */
    double calculateGammaPolakRibiere();

    /**
     * @brief calculateWeightingFactor computes equation 2.22 of the thesis
     * @return the value for _regularisationCurrent.bSquared
     */
    dataGrid2D calculateWeightingFactor();

    /**
     * @brief calculateStepSizeRegularisation prepares the parameters for equation 2.26 (see eq. 2.27) and invokes findRealRootFromCubic
     * @return the output of findRealRootFromCubic(...), which is the final StepSize value, to be sent to StepAndDirectionReconstructor
     */
    double calculateStepSizeRegularisation();

    /**
     * @brief calculateDirectionRegularisation updates _kappaTimesResidual and _gradientCurrent following equations 2.13 and 2.25
     * @return the new value of _zetaCurrent
     */
    dataGrid2D calculateDirectionRegularisation();

    /**
     * @brief calculateSteeringFactor is a modified version of equation 2.23
     * @return the new value of _regularisationCurrent.deltaSquared
     */
    double calculateSteeringFactor();

    /**
     * @brief findRealRootFromCubic applies the Cardano formula to solve third order polynomial ax^3 +bx^2 +cx +d =0
     * @param a
     * @param b
     * @param c
     * @param d
     * @return  the (in this case) only real root
     */
    double findRealRootFromCubic(double a, double b, double c, double d);

    /**
     * @brief calculateRegularisationErrorFunctional updates _regularisationPrevious.errorFunctional after having computed _regularisationCurrent.gradientChi
     */
    void calculateRegularisationErrorFunctional();

    /**
     * @brief calculateRegularisationGradient by only using _regularisationPrevious.bSquared and .gradientChi, see equation 2.24
     * @return updates _regularisationCurrent.gradient
     */
    dataGrid2D calculateRegularisationGradient();

public:
    ConjugateGradientWithRegularisationCalculator(double errorFunctionalScalingFactor, forwardModelInterface *forwardModel,
        ConjugateGradientWithRegularisationParametersInput cgParametersInput, const std::vector<std::complex<double>> &pData);   // constructor
    virtual ~ConjugateGradientWithRegularisationCalculator();                                                                    // destructor

    /**
     * @brief calculateStepSize After computing the optimal step as described in Eq. PolakRibiereDirection of ReadMe/1_ProjectDescription, it will also invoke
     * the whole regularisation process, where _zetaCurrent, the output of calculateDirection(), is also updated
     * @return alpha, the final optimized StepSize for the updated _zetaCurrent
     */
    double calculateStepSize() override;

    /**
     * @brief calculateDirection performs the computation described in Eq. integrandForDiscreteK of ReadMe/1_ProjectDescription
     * @param residualVector is saved into the member variable _residualVector
     * @return is the direction = _zetaCurrent. We output a reference so that the value is updated as we perform at a later stage the regularisation process
     * during the call of calculateStepSize
     */
    dataGrid2D &calculateDirection(const dataGrid2D &, const std::vector<std::complex<double>> &residualVector) override;
    /**
     * @brief updateVariables only modifies _chiEstimateCur/Pr and _iterationNumber, since _gradientCur/Pr and _zetaCur/Pr are already updated in
     * CalculateDirection
     * @param chiEstimateCurrent
     * @param iterationNumber
     */
    void updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &, int iterationNumber) override;
};
