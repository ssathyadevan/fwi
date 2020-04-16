#include "GradientDescentStepSizeCalculator.h"

// this will be gradient descent
GradientDescentStepSizeCalculator::GradientDescentStepSizeCalculator(const grid2D chiEstimate) :
    _chiEstimatePrevious(chiEstimate), _chiEstimateCurrent(chiEstimate), _derivativePrevious(chiEstimate), _derivativeCurrent(chiEstimate)
{
}

GradientDescentStepSizeCalculator::~GradientDescentStepSizeCalculator() {}

double GradientDescentStepSizeCalculator::calculateStepSize()
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

void GradientDescentStepSizeCalculator::setCurrentVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent)
{
    _chiEstimateCurrent = chiEstimateCurrent;
    _derivativeCurrent = derivativeCurrent;
}

void GradientDescentStepSizeCalculator::updateVariables(const dataGrid2D &chiEstimateCurrent, const dataGrid2D &derivativeCurrent)
{
    _chiEstimatePrevious = _chiEstimateCurrent;
    _derivativePrevious = _derivativeCurrent;
    setCurrentVariables(chiEstimateCurrent, derivativeCurrent);
}
