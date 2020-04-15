#include "DirectionInput.h"

DirectionInput::DirectionInput(dataGrid2D startingChi) : _startingChi(startingChi) {}

DirectionInput::DirectionInput(double tolerance, dataGrid2D startingChi, int maxIterationsNumber, double h) :
    _tolerance(tolerance), _startingChi(startingChi), _maxIterationsNumber(maxIterationsNumber), _h(h)
{
}
