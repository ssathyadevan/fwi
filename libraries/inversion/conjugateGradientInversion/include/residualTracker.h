#pragma once

#include <vector>

class ResidualTracker 
{
    public:
        explicit ResidualTracker(int cgInputIter1N);
        ~ResidualTracker() {}

        void determineIfDiverging(double residual);
        void incrementFinalPeriodCounter();
        int getFinalPeriodCounter();
        bool isDiverging();


    private:
        bool mDiverging;
        std::vector<double> mPreviousResiduals;
        double mPreviousLowPoint;
        double mCurrentLowPoint;
        int mFinalPeriodCounter;
        int mPeakPeriod;

        void updatePreviousResiduals(double residual);
        void updateLowPoints();
};
