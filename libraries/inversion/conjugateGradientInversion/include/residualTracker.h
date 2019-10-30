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
        bool _diverging;
        std::vector<double> _previousResiduals;
        double _previousLowPoint;
        double _currentLowPoint;
        int _finalPeriodCounter;
        int _peakPeriod;

        void updatePreviousResiduals(double residual);
        void updateLowPoints();
};
