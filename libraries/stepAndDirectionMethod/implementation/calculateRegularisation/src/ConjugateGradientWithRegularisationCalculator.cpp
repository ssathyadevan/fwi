#include "ConjugateGradientWithRegularisationCalculator.h"

// void RegularisationCalculator::calculateRegularisationParameters()
//{

//}

// dataGrid2D RegularisationCalculator::calculateUpdateRegularisationParameters()
//{

//}

// void RegularisationCalculator::calculateStepSizeRegularisation()
//{

//}

// const double RegularisationCalculator::errorFunctional()
//{

//}

// void RegularisationCalculator::logResidualResults()
//{

//}

ConjugateGradientWithRegularisationCalculator::ConjugateGradientWithRegularisationCalculator(
    double errorFunctionalScalingFactor, forwardModelInterface *forwardModel, int regularisationIterations) :
    DirectionCalculator(errorFunctionalScalingFactor, forwardModel),
    _regularisationIterations(regularisationIterations)
{
}

double ConjugateGradientWithRegularisationCalculator::calculateStepSize()
{
    // HERE INVOKE ALL THE REGULARISATION PROCESS
    return 1.0;
}

// void ConjugateGradientWithRegularisationCalculator::updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent, int)
//{

//}
