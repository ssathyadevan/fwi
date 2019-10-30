#include "residualTracker.h"
#include <algorithm>

ResidualTracker::ResidualTracker(int cgInputIter1N)
    : _peakPeriod(cgInputIter1N)
{
    _previousResiduals = {1.0, 1.0};
    _currentLowPoint = 1.0;
    _previousLowPoint = 1.0;
    _finalPeriodCounter = 0;
    _diverging = false;
}

void ResidualTracker::updatePreviousResiduals(double residual) {
    _previousResiduals[0] = _previousResiduals[1];
    _previousResiduals[1] = residual;
}

void ResidualTracker::updateLowPoints() {
    if (_previousResiduals[1] > _previousResiduals[0]) {
        _previousLowPoint = _currentLowPoint;
        _currentLowPoint = _previousResiduals[0];
    }
}

void ResidualTracker::determineIfDiverging(double residual) 
{
    updatePreviousResiduals(residual);
    updateLowPoints();
    if (_previousLowPoint < _currentLowPoint) {
        _diverging = true;
    }
}

void ResidualTracker::incrementFinalPeriodCounter() {
    _finalPeriodCounter++;
}

int ResidualTracker::getFinalPeriodCounter() {
    return _finalPeriodCounter;
}

bool ResidualTracker::isDiverging() {
    return _diverging;
}
