#include "residualTracker.h"
#include <algorithm>

ResidualTracker::ResidualTracker(int cgInputIter1N)
    : mPeakPeriod(cgInputIter1N)
{
    mPreviousResiduals = {1.0, 1.0};
    mCurrentLowPoint = 1.0;
    mPreviousLowPoint = 1.0;
    mFinalPeriodCounter = 0;
    mDiverging = false;
}

void ResidualTracker::updatePreviousResiduals(double residual) {
    mPreviousResiduals[0] = mPreviousResiduals[1];
    mPreviousResiduals[1] = residual;
}

void ResidualTracker::updateLowPoints() {
    if (mPreviousResiduals[1] > mPreviousResiduals[0]) {
        mPreviousLowPoint = mCurrentLowPoint;
        mCurrentLowPoint = mPreviousResiduals[0];
    }
}

void ResidualTracker::determineIfDiverging(double residual) 
{
    updatePreviousResiduals(residual);
    updateLowPoints();
    if (mPreviousLowPoint < mCurrentLowPoint) {
        mDiverging = true;
    }
}

void ResidualTracker::incrementFinalPeriodCounter() {
    mFinalPeriodCounter++;
}

int ResidualTracker::getFinalPeriodCounter() {
    return mFinalPeriodCounter;
}

bool ResidualTracker::isDiverging() {
    return mDiverging;
}
