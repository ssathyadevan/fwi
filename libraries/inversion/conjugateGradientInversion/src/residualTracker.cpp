#include "residualTracker.h"
#include <algorithm>

ResidualTracker::ResidualTracker(int cgInputIter1N) 
    : mNumConsecutiveDecreases(cgInputIter1N-1), mPreviousResiduals(cgInputIter1N+1, 0) 
{

}

bool ResidualTracker::determineIfIncreasing() 
{
    if (mPreviousResiduals[1] - mPreviousResiduals[0] > 0) {
        return true;
    }
    return false;
}

bool ResidualTracker::determineIfDiverging() 
{
    if (determineIfIncreasing()) {
        for (unsigned i = 1; i <= mNumConsecutiveDecreases-1; ++i) {
            if (mPreviousResiduals[i+1] - mPreviousResiduals[i] > 0) {
                return true;
            }
        }
    }
    return false;
}

void ResidualTracker::updateResidualList(double residual) 
{
    std::rotate(mPreviousResiduals.begin(),mPreviousResiduals.begin()+1,mPreviousResiduals.end());
    mPreviousResiduals.back() = residual;
}

std::vector<double>& ResidualTracker:: getResidualList() 
{
    return mPreviousResiduals;
}