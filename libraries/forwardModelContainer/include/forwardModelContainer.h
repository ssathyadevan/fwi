#pragma once

#include "genericInput.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include <string>


class ForwardModelContainer
{

public:
    ForwardModelContainer(const GenericInput &gInput, const std::string desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv,
                          const FrequenciesGroup &freq);
   ~ForwardModelContainer();

   void createForwardModels(const GenericInput &gInput, const std::string desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv,
                    const FrequenciesGroup &freq);
   void calculateKappaParallel();
   std::vector<std::complex<double>>& calculateResidualParallel(const PressureFieldSerial& chiEstimate, const std::vector<std::complex<double>>& pDataRef);
   double calculateResidualNormSqParallel(std::vector<std::complex<double>> &residual);


private:
    std::vector<ForwardModelInterface*> _forwardmodels;
    const int _nrSources;
    const int _nrReceivers;
    std::vector<int> _nrFrequencies;
    std::vector<std::complex<double>> _residuals;
    std::vector<FrequenciesGroup> _frequenciesVector;

    void devideFrequencies(const FrequenciesGroup& frequenties, const int& nr_threads);
};
