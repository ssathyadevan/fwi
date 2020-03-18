#include <forwardModelContainer.h>
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "log.h"
#include <omp.h>
#include <algorithm>

#include <iostream>

ForwardModelContainer::ForwardModelContainer(const GenericInput &gInput, const std::string desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv,
                                             const FrequenciesGroup &freq)
            : _forwardmodels(), _nrSources(src.nSrc), _nrReceivers(recv.nRecv), _nrFrequencies(omp_get_max_threads(), 1), _residuals(), _frequenciesVector()
{
    devideFrequencies(freq,omp_get_max_threads());
    createForwardModels(gInput, desired_forward_model, grid, src, recv, freq);
}

ForwardModelContainer::~ForwardModelContainer()
{
    for (int thread_nr = 0; thread_nr < omp_get_max_threads() ; thread_nr ++)
    {
        delete _forwardmodels[thread_nr];
    }
}

void ForwardModelContainer::createForwardModels(const GenericInput &gInput, const std::string desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv,
                    const FrequenciesGroup &freq)
{
    (void)freq;
    const int nr_threads = omp_get_max_threads();
   // std::vector<FrequenciesGroup> devided_frequenties = devideFrequencies(freq, nr_threads);
    if (desired_forward_model == "integralForwardModel"){
        IntegralForwardModelInputCardReader integralreader(gInput.caseFolder);
        for (int threadNr = 0; threadNr < nr_threads ; threadNr ++)
        {
            ForwardModelInterface *model = new IntegralForwardModel(grid, src, recv, _frequenciesVector[threadNr], integralreader.getInput());
            _forwardmodels.push_back(model);
        }
    }
    else if(desired_forward_model == "finiteDifferenceForwardModel"){
        FiniteDifferenceForwardModelInputCardReader finitedifferencereader(gInput.caseFolder);
        for (int threadNr = 0; threadNr < nr_threads ; threadNr ++)
        {
            ForwardModelInterface *model = new FiniteDifferenceForwardModel(grid, src, recv, _frequenciesVector[threadNr], finitedifferencereader.getInput());
            _forwardmodels.push_back(model);
        }
    }
    else
    {   L_(linfo) << "The ForwardModel "<< desired_forward_model <<" was not found" ;
        exit(EXIT_FAILURE);
    }

}

void ForwardModelContainer::calculateKappaParallel()
{
#pragma omp parallel
    {
        _forwardmodels[omp_get_thread_num()]->calculateKappa();
    }

}

std::vector<std::complex<double> > &ForwardModelContainer::calculateResidualParallel(const PressureFieldSerial &chiEstimate, const std::vector<std::complex<double> > &pDataRef)
{
    int total_freq = std::accumulate(_nrFrequencies.begin(), _nrFrequencies.end(), 0);
    std::vector<std::complex<double>> all_residuals(_nrReceivers*_nrSources*total_freq);

#pragma omp parallel
    {
        int prev_freq = 0;
        for (int i = 0 ; i <omp_get_thread_num() ; i++){
            prev_freq+= _nrFrequencies[i];
        }
        int my_offset =  _nrReceivers*_nrSources* prev_freq;

        int my_length = _nrReceivers * _nrSources * _nrFrequencies[omp_get_thread_num()];
        std::vector<std::complex<double>> my_pDataRef(my_length);
        std::copy(pDataRef.begin()+ my_offset, pDataRef.begin() + my_offset + my_length, my_pDataRef.begin());

        std::vector<std::complex<double>> my_residuals = _forwardmodels[omp_get_thread_num()]->calculateResidual(chiEstimate, my_pDataRef);

        std::copy(my_residuals.begin(), my_residuals.end(), all_residuals.begin()+ my_offset);
    }
    _residuals = all_residuals;
    return _residuals;
}

double ForwardModelContainer::calculateResidualNormSqParallel(std::vector<std::complex<double> > &residual)
{
    double residualNormSQ = 0.0;
#pragma omp parallel reduction(+: residualNormSQ)
    {
        residualNormSQ = _forwardmodels[omp_get_thread_num()]->calculateResidualNormSq(residual);
        std::cout << "Thread: " << omp_get_thread_num() << " computed res norm square of: " << residualNormSQ << std::endl;
    }
    return residualNormSQ;
}

void ForwardModelContainer::devideFrequencies(const FrequenciesGroup &frequenties, const int& nr_threads)
{
    double current_freq = frequenties.freq[0];
    for (int thread_nr = 0; thread_nr < nr_threads; thread_nr ++)
    {
        int thread_total = static_cast<int>( std::floor(frequenties.nFreq*1.0 / (nr_threads*1.0)));

        if (thread_nr < frequenties.nFreq%nr_threads) {thread_total += 1;}
        double thread_min = current_freq;
        double thread_max = thread_min + (thread_total - 1) * frequenties.d_freq;

        Freq freq = {thread_min, thread_max, thread_total};
        FrequenciesGroup thread_freq(freq, frequenties.c_0);

        thread_freq.Print(thread_total);
        _frequenciesVector.push_back(thread_freq);

        _nrFrequencies[thread_nr]=thread_total;
        current_freq = thread_max + frequenties.d_freq;
    }
}
