#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "log.h"
#include <algorithm>
#include <forwardModelContainer.h>
#include <omp.h>

#include <iostream>

ForwardModelContainer::ForwardModelContainer(const genericInput &genericInput, const std::string &desiredForwardModel, const grid2D &grid,
    const sources &sources, const receivers &receivers, const frequenciesGroup &frequencies) :
    _forwardmodels(),
    _numberOfThreads(std::min(frequencies.nFreq, omp_get_max_threads())), _numberOfSources(sources.nSrc), _numberOfReceivers(receivers.nRecv),
    _numberOfFrequenciesPerThread(_numberOfThreads, 0), _residuals(), _frequenciesVector(), _allFrequencies(frequencies)
{
    L_(linfo) << "Container uses " << _numberOfThreads << " threads for parallelization.";
    divideFrequencies();
    createForwardModels(genericInput, desiredForwardModel, grid, sources, receivers);
}

ForwardModelContainer::~ForwardModelContainer()
{
    for(int threadNumber = 0; threadNumber < _numberOfThreads; threadNumber++)
    {
        delete _forwardmodels[threadNumber];
    }
}

void ForwardModelContainer::createForwardModels(
    const genericInput &gInput, const std::string &desiredForwardModel, const grid2D &grid, const sources &sources, const receivers &receivers)
{
    if(desiredForwardModel == "integralForwardModel")
    {
        integralForwardModelInputCardReader integralreader(gInput.caseFolder);
        for(int threadNumber = 0; threadNumber < _numberOfThreads; threadNumber++)
        {
            forwardModelInterface *model = new IntegralForwardModel(grid, sources, receivers, _frequenciesVector[threadNumber], integralreader.getInput());
            _forwardmodels.push_back(model);
        }
    }
    else if(desiredForwardModel == "finiteDifferenceForwardModel")
    {
        finiteDifferenceForwardModelInputCardReader finitedifferencereader(gInput.caseFolder);
        for(int threadNumber = 0; threadNumber < _numberOfThreads; threadNumber++)
        {
            forwardModelInterface *model =
                new finiteDifferenceForwardModel(grid, sources, receivers, _frequenciesVector[threadNumber], finitedifferencereader.getInput());
            _forwardmodels.push_back(model);
        }
    }
    else
    {
        L_(linfo) << "The ForwardModel " << desiredForwardModel << " was not found";
        exit(EXIT_FAILURE);
    }
}

void ForwardModelContainer::calculateKappaParallel()
{
    omp_set_num_threads(_numberOfThreads);
#pragma omp parallel
    {
        _forwardmodels[omp_get_thread_num()]->calculateKappa();
    }
}

int ForwardModelContainer::computeThreadOffset()
{
    int cummulativeNumberOfFrequencies = 0;
    for(int threadNumber = 0; threadNumber < omp_get_thread_num(); threadNumber++)
    {
        cummulativeNumberOfFrequencies += _numberOfFrequenciesPerThread[threadNumber];
    }
    int offset = _numberOfReceivers * _numberOfSources * cummulativeNumberOfFrequencies;

    return offset;
}

std::vector<std::complex<double>> &ForwardModelContainer::calculateResidualParallel(
    const pressureFieldSerial &chiEstimate, const std::vector<std::complex<double>> &pDataRef)
{
    std::vector<std::complex<double>> allResiduals(_numberOfReceivers * _numberOfSources * _allFrequencies.nFreq);
    omp_set_num_threads(_numberOfThreads);
#pragma omp parallel
    {
        int privateOffset = computeThreadOffset();
        int privateLength = _numberOfReceivers * _numberOfSources * _numberOfFrequenciesPerThread[omp_get_thread_num()];

        std::vector<std::complex<double>> privatePDataRef(privateLength);
        std::copy(pDataRef.begin() + privateOffset, pDataRef.begin() + privateOffset + privateLength, privatePDataRef.begin());

        std::vector<std::complex<double>> privateResiduals = _forwardmodels[omp_get_thread_num()]->calculateResidual(chiEstimate, privatePDataRef);

        std::copy(privateResiduals.begin(), privateResiduals.end(), allResiduals.begin() + privateOffset);
    }
    _residuals = allResiduals;
    return _residuals;
}

double ForwardModelContainer::calculateResidualNormSqParallel(const std::vector<std::complex<double>> &residual)
{
    double residualNormSq = 0.0;
    omp_set_num_threads(_numberOfThreads);
#pragma omp parallel reduction(+ : residualNormSq)
    {
        int privateOffset = computeThreadOffset();
        int privateLength = _numberOfReceivers * _numberOfSources * _numberOfFrequenciesPerThread[omp_get_thread_num()];

        std::vector<std::complex<double>> privateResidual(privateLength);
        std::copy(residual.begin() + privateOffset, residual.begin() + privateOffset + privateLength, privateResidual.begin());

        double privateResidualNormSq = _forwardmodels[omp_get_thread_num()]->calculateResidualNormSq(privateResidual);
        residualNormSq = privateResidualNormSq;
    }
    return residualNormSq;
}

void ForwardModelContainer::divideFrequencies()
{
    double currentMinimumFrequency = _allFrequencies.freq[0];
    for(int threadNumber = 0; threadNumber < _numberOfThreads; threadNumber++)
    {
        int totalFrequenciesForThread = static_cast<int>(std::floor(_allFrequencies.nFreq * 1.0 / (_numberOfThreads * 1.0)));

        if(threadNumber < _allFrequencies.nFreq % _numberOfThreads) { totalFrequenciesForThread += 1; }
        double minimumFrequencyForThread = currentMinimumFrequency;
        double maximumFrequencyForThread = minimumFrequencyForThread + (totalFrequenciesForThread - 1) * _allFrequencies.dFreq;

        Freq frequencyStruct = {minimumFrequencyForThread, maximumFrequencyForThread, totalFrequenciesForThread};
        frequenciesGroup frequenciesForThread(frequencyStruct, _allFrequencies.c0);

        frequenciesForThread.Print(totalFrequenciesForThread);
        _frequenciesVector.push_back(frequenciesForThread);

        _numberOfFrequenciesPerThread[threadNumber] = totalFrequenciesForThread;
        currentMinimumFrequency = maximumFrequencyForThread + _allFrequencies.dFreq;
    }
}
