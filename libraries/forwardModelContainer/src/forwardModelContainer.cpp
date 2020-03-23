#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "log.h"
#include <algorithm>
#include <forwardModelContainer.h>
#include <omp.h>

#include <iostream>

ForwardModelContainer::ForwardModelContainer(const GenericInput &gInput, const std::string &desired_forward_model, const Grid2D &grid, const Sources &src,
    const Receivers &recv, const FrequenciesGroup &freq) :
    _forwardmodels(),
    _numberOfThreads(std::min(freq.nFreq, omp_get_max_threads())), _numberOfSources(src.nSrc), _numberOfReceivers(recv.nRecv),
    _numberOfFrequencies(_numberOfThreads, 1), _residuals(), _frequenciesVector(), _allFrequencies(freq)
{
    L_(linfo) << "Container uses " << _numberOfThreads << " threads for parallelization.";
    devideFrequencies();
    createForwardModels(gInput, desired_forward_model, grid, src, recv);
}

ForwardModelContainer::~ForwardModelContainer()
{
    for(int threadNumber = 0; threadNumber < _numberOfThreads; threadNumber++)
    {
        delete _forwardmodels[threadNumber];
    }
}

void ForwardModelContainer::createForwardModels(
    const GenericInput &gInput, const std::string &desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv)
{
    if(desired_forward_model == "integralForwardModel")
    {
        IntegralForwardModelInputCardReader integralreader(gInput.caseFolder);
        for(int threadNumber = 0; threadNumber < _numberOfThreads; threadNumber++)
        {
            ForwardModelInterface *model = new IntegralForwardModel(grid, src, recv, _frequenciesVector[threadNumber], integralreader.getInput());
            _forwardmodels.push_back(model);
        }
    }
    else if(desired_forward_model == "finiteDifferenceForwardModel")
    {
        FiniteDifferenceForwardModelInputCardReader finitedifferencereader(gInput.caseFolder);
        for(int threadNumber = 0; threadNumber < _numberOfThreads; threadNumber++)
        {
            ForwardModelInterface *model =
                new FiniteDifferenceForwardModel(grid, src, recv, _frequenciesVector[threadNumber], finitedifferencereader.getInput());
            _forwardmodels.push_back(model);
        }
    }
    else
    {
        L_(linfo) << "The ForwardModel " << desired_forward_model << " was not found";
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

int ForwardModelContainer::ComputeThreadOffset()
{
    int cummulativeNumberOfFrequencies = 0;
    for(int threadNumber = 0; threadNumber < omp_get_thread_num(); threadNumber++)
    {
        cummulativeNumberOfFrequencies += _numberOfFrequencies[threadNumber];
    }
    int offset = _numberOfReceivers * _numberOfSources * cummulativeNumberOfFrequencies;

    return offset;
}

std::vector<std::complex<double>> &ForwardModelContainer::calculateResidualParallel(
    const PressureFieldSerial &chiEstimate, const std::vector<std::complex<double>> &pDataRef)
{
    std::vector<std::complex<double>> all_residuals(_numberOfReceivers * _numberOfSources * _allFrequencies.nFreq);
    omp_set_num_threads(_numberOfThreads);
#pragma omp parallel
    {
        int privateOffset = ComputeThreadOffset();
        int privateLength = _numberOfReceivers * _numberOfSources * _numberOfFrequencies[omp_get_thread_num()];

        std::vector<std::complex<double>> my_pDataRef(privateLength);
        std::copy(pDataRef.begin() + privateOffset, pDataRef.begin() + privateOffset + privateLength, my_pDataRef.begin());

        std::vector<std::complex<double>> privateResiduals = _forwardmodels[omp_get_thread_num()]->calculateResidual(chiEstimate, my_pDataRef);

        std::copy(privateResiduals.begin(), privateResiduals.end(), all_residuals.begin() + privateOffset);
    }
    _residuals = all_residuals;
    return _residuals;
}

double ForwardModelContainer::calculateResidualNormSqParallel(const std::vector<std::complex<double>> &residual)
{
    double residualNormSq = 0.0;
    omp_set_num_threads(_numberOfThreads);
#pragma omp parallel reduction(+ : residualNormSq)
    {
        int privateOffset = ComputeThreadOffset();
        int privateLength = _numberOfReceivers * _numberOfSources * _numberOfFrequencies[omp_get_thread_num()];

        std::vector<std::complex<double>> privateResidual(privateLength);
        std::copy(residual.begin() + privateOffset, residual.begin() + privateOffset + privateLength, privateResidual.begin());

        double privateResidualNormSq = _forwardmodels[omp_get_thread_num()]->calculateResidualNormSq(privateResidual);
        residualNormSq = privateResidualNormSq;
    }
    return residualNormSq;
}

const FrequenciesGroup &ForwardModelContainer::getFrequencies() { return _allFrequencies; }

const Grid2D &ForwardModelContainer::getGrid() { return _forwardmodels[0]->getGrid(); }

const Sources &ForwardModelContainer::getSources() { return _forwardmodels[0]->getSrc(); }

const Receivers &ForwardModelContainer::getReceivers() { return _forwardmodels[0]->getRecv(); }

void ForwardModelContainer::devideFrequencies()
{
    double currentMinimumFrequency = _allFrequencies.freq[0];
    for(int threadNumber = 0; threadNumber < _numberOfThreads; threadNumber++)
    {
        int totalFrequenciesForThread = static_cast<int>(std::floor(_allFrequencies.nFreq * 1.0 / (_numberOfThreads * 1.0)));

        if(threadNumber < _allFrequencies.nFreq % _numberOfThreads) { totalFrequenciesForThread += 1; }
        double minimumFrequencyForThread = currentMinimumFrequency;
        double maximumFrequencyForThread = minimumFrequencyForThread + (totalFrequenciesForThread - 1) * _allFrequencies.d_freq;

        Freq frequencyStruct = {minimumFrequencyForThread, maximumFrequencyForThread, totalFrequenciesForThread};
        FrequenciesGroup frequenciesForThread(frequencyStruct, _allFrequencies.c_0);

        frequenciesForThread.Print(totalFrequenciesForThread);
        _frequenciesVector.push_back(frequenciesForThread);

        _numberOfFrequencies[threadNumber] = totalFrequenciesForThread;
        currentMinimumFrequency = maximumFrequencyForThread + _allFrequencies.d_freq;
    }
}
