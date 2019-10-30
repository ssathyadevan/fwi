#pragma once

#include <vector>

class ResidualTracker 
{
    public:
        explicit ResidualTracker(int cgInputIter1N);
        ~ResidualTracker() {}

        bool determineIfDiverging();
        void updateResidualList(double residual);
        std::vector<double>& getResidualList();

    private:
        unsigned mNumConsecutiveDecreases;
        std::vector<double> mPreviousResiduals;

        bool determineIfIncreasing();
};