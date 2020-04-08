#pragma once
#include "directionInput.h"
#include "forwardModelInterface.h"

inline double normSq(const std::vector<std::complex<double>> &data, int n)
{
    double result = double(0.0);
    for(int i = 0; i < n; i++)
    {
        result += std::norm(data[i]);
    }

    return result;
}

class StepAndDirectionInterface
{
private:
    chosenStep *_chosenStep;
    directionCalculator *_chosenDirection;
    forwardModelInterface *_forwardModel;
    DirectionInput _directionInput;   // need to CREATE the right generic input

    const grid2D &_grid;
    const sources &_src;
    const receivers &_recv;
    const frequenciesGroup _freq;

public:
    StepAndDirectionInterface(chosenStep *step, directionCalculator *direction, forwardModelInterface *forwardModel, const directionInput &input);
    ~inversionInterface() = default;

    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);
    // why are there gInput and _input? also and expecially in the gradientDescentInversion files

    dataGrid2D calculateNextMove(dataGrid2D chiEstimate, const std::vector<double> &direction, const double step);
    double functionF(const dataGrid2D chiEstimate, const std::vector<std::complex<double>> &pData, double eta);
    double calculateResidualNormSq(std::vector<std::complex<double>> &residual);
};
