#pragma once

#include <vector>

class ResidualTracker 
{
    public:
        ResidualTracker();
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

        void updatePreviousResiduals(double residual);
        void updateLowPoints();
};
