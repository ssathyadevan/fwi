#include "BorzilaiBorweinStepSizeCalculator.h"
// the Borzilai-Borwein is the most common choice for the step computation of the Gradient Descent algorithm,
// see https://en.wikipedia.org/wiki/Gradient_descent

// this will be gradient descent
BorzilaiBorweinStepSizeCalculator::BorzilaiBorweinStepSizeCalculator(const grid2D &chiEstimate) :
    _chiEstimatePrevious(chiEstimate), _chiEstimateCurrent(chiEstimate), _derivativePrevious(chiEstimate), _derivativeCurrent(chiEstimate)
{
}

BorzilaiBorweinStepSizeCalculator::~BorzilaiBorweinStepSizeCalculator() {}

double BorzilaiBorweinStepSizeCalculator::calculateStepSize()
{
    const int nGridPoints = _chiEstimateCurrent.getNumberOfGridPoints();

    const std::vector<double> &dataCurrent = _chiEstimateCurrent.getData();
    const std::vector<double> &dataPrevious = _chiEstimatePrevious.getData();
    const std::vector<double> &derivativeCurrent = _derivativeCurrent.getData();
    const std::vector<double> &derivativePrevious = _derivativePrevious.getData();

    std::vector<double> dx(nGridPoints, 0.0);
    for(int i = 0; i < nGridPoints; ++i)
    {
        dx[i] = dataCurrent[i] - dataPrevious[i];
    }

    std::vector<double> ddFdx(nGridPoints, 0.0);
    for(int i = 0; i < nGridPoints; ++i)
    {
        ddFdx[i] = derivativeCurrent[i] - derivativePrevious[i];
    }

    double gammaNumerator = 0.0;
    for(int i = 0; i < nGridPoints; ++i)
    {
        gammaNumerator += dx[i] * ddFdx[i];
    }
    gammaNumerator = fabs(gammaNumerator);

    double gammaDenominator = 0.0;
    for(int i = 0; i < nGridPoints; ++i)
    {
        gammaDenominator += pow(ddFdx[i], 2);
    }

    if(gammaDenominator == 0.0)
    {
        throw std::overflow_error("Operator divides by zero");
    }

    double gamma = gammaNumerator / gammaDenominator;
    return gamma;
}

void BorzilaiBorweinStepSizeCalculator::setCurrentVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent)
{
    // dataGrid2D = dataGrid2D only reassigns _data[], but I need the _grid to be the same for the = to take place
    _chiEstimateCurrent = chiEstimateCurrent;
    _derivativeCurrent = derivativeCurrent;
}

void BorzilaiBorweinStepSizeCalculator::updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent)
{
    _chiEstimatePrevious = _chiEstimateCurrent;
    _derivativePrevious = _derivativeCurrent;
    setCurrentVariables(chiEstimateCurrent, derivativeCurrent);
}
