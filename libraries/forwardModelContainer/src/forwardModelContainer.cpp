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
    _numberOfThreads(std::min(freq.nFreq, omp_get_max_threads())), _nrSources(src.nSrc), _nrReceivers(recv.nRecv), _nrFrequencies(_numberOfThreads, 1),
    _residuals(), _frequenciesVector(), _allFrequencies(freq)
{
    L_(linfo) << "Container uses " << _numberOfThreads << " threads for parallelization.";
    devideFrequencies(freq);
    createForwardModels(gInput, desired_forward_model, grid, src, recv);
}

ForwardModelContainer::~ForwardModelContainer()
{
    for(int thread_nr = 0; thread_nr < _numberOfThreads; thread_nr++)
    {
        delete _forwardmodels[thread_nr];
    }
}

void ForwardModelContainer::createForwardModels(
    const GenericInput &gInput, const std::string &desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv)
{
    // const int nr_threads = omp_get_max_threads();
    if(desired_forward_model == "integralForwardModel")
    {
        IntegralForwardModelInputCardReader integralreader(gInput.caseFolder);
        for(int threadNr = 0; threadNr < _numberOfThreads; threadNr++)
        {
            ForwardModelInterface *model = new IntegralForwardModel(grid, src, recv, _frequenciesVector[threadNr], integralreader.getInput());
            _forwardmodels.push_back(model);
        }
    }
    else if(desired_forward_model == "finiteDifferenceForwardModel")
    {
        FiniteDifferenceForwardModelInputCardReader finitedifferencereader(gInput.caseFolder);
        for(int threadNr = 0; threadNr < _numberOfThreads; threadNr++)
        {
            ForwardModelInterface *model = new FiniteDifferenceForwardModel(grid, src, recv, _frequenciesVector[threadNr], finitedifferencereader.getInput());
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
    int prev_freq = 0;
    for(int i = 0; i < omp_get_thread_num(); i++)
    {
        prev_freq += _nrFrequencies[i];
    }
    int my_offset = _nrReceivers * _nrSources * prev_freq;

    return my_offset;
}

std::vector<std::complex<double>> &ForwardModelContainer::calculateResidualParallel(
    const PressureFieldSerial &chiEstimate, const std::vector<std::complex<double>> &pDataRef)
{
    int total_freq = std::accumulate(_nrFrequencies.begin(), _nrFrequencies.end(), 0);
    std::vector<std::complex<double>> all_residuals(_nrReceivers * _nrSources * total_freq);
    omp_set_num_threads(_numberOfThreads);
#pragma omp parallel
    {
        int my_offset = ComputeThreadOffset();

        int my_length = _nrReceivers * _nrSources * _nrFrequencies[omp_get_thread_num()];
        std::vector<std::complex<double>> my_pDataRef(my_length);
        std::copy(pDataRef.begin() + my_offset, pDataRef.begin() + my_offset + my_length, my_pDataRef.begin());

        std::vector<std::complex<double>> my_residuals = _forwardmodels[omp_get_thread_num()]->calculateResidual(chiEstimate, my_pDataRef);

        std::copy(my_residuals.begin(), my_residuals.end(), all_residuals.begin() + my_offset);
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
        int offset = ComputeThreadOffset();
        int length = _nrReceivers * _nrSources * _nrFrequencies[omp_get_thread_num()];

        std::vector<std::complex<double>> my_residual(length);
        std::copy(residual.begin() + offset, residual.begin() + offset + length, my_residual.begin());

        double my_residualNormSq;
        my_residualNormSq = _forwardmodels[omp_get_thread_num()]->calculateResidualNormSq(my_residual);
        residualNormSq = my_residualNormSq;
    }
    return residualNormSq;
}

const FrequenciesGroup &ForwardModelContainer::getFrequencies() { return _allFrequencies; }

const Grid2D &ForwardModelContainer::getGrid() { return _forwardmodels[0]->getGrid(); }

const Sources &ForwardModelContainer::getSources() { return _forwardmodels[0]->getSrc(); }

const Receivers &ForwardModelContainer::getReceivers() { return _forwardmodels[0]->getRecv(); }

void ForwardModelContainer::devideFrequencies(const FrequenciesGroup &frequenties)
{
    double current_freq = frequenties.freq[0];
    for(int thread_nr = 0; thread_nr < _numberOfThreads; thread_nr++)
    {
        int thread_total = static_cast<int>(std::floor(frequenties.nFreq * 1.0 / (_numberOfThreads * 1.0)));

        if(thread_nr < frequenties.nFreq % _numberOfThreads) { thread_total += 1; }
        double thread_min = current_freq;
        double thread_max = thread_min + (thread_total - 1) * frequenties.d_freq;

        Freq freq = {thread_min, thread_max, thread_total};
        FrequenciesGroup thread_freq(freq, frequenties.c_0);

        thread_freq.Print(thread_total);
        _frequenciesVector.push_back(thread_freq);

        _nrFrequencies[thread_nr] = thread_total;
        current_freq = thread_max + frequenties.d_freq;
    }
}
