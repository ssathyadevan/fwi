#pragma once

#include "forwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"
#include "finiteDifferenceForwardModelInput.h"
#include "genericInput.h"

class FiniteDifferenceForwardModel : public ForwardModelInterface
{

public:

    FiniteDifferenceForwardModel( const Grid2D &grid, const Sources &src, const Receivers &recv,
                    const FrequenciesGroup &freq, const GenericInput &gInput );

    ~FiniteDifferenceForwardModel();

    virtual void calculatePData(const PressureFieldSerial &chiEst, std::vector<std::complex<double>> &kOperator);

    void calculateKappa();
    virtual void calculatePTot(const PressureFieldSerial &chiEst);
    virtual void getUpdateDirectionInformation(const std::vector<std::complex<double> > &res, PressureFieldComplexSerial &kRes);
    virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &res, PressureFieldComplexSerial &kRes, const int offset, const int block_size);
    virtual void getResidualGradient(std::vector<std::complex<double>> &res, PressureFieldComplexSerial &kRes);
    virtual void mapDomainToSignal(const PressureFieldSerial &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator);


    //void createKappaOperator(const PressureFieldComplexSerial &CurrentPressureFieldComplexSerial, std::complex<double> *kOperator);

private:

    Greens_rect_2D_cpu          **_Greens;

    PressureFieldComplexSerial  ***_p0;
    PressureFieldComplexSerial  **_pTot;
    PressureFieldComplexSerial  **_Kappa;
    FiniteDifferenceForwardModelInput _fmInput;

    void createP0();
    void deleteP0();

    void createPTot(const FrequenciesGroup &freq, const Sources &src);

    void createGreens();
    void deleteGreens();

    void deletePtot();

    PressureFieldComplexSerial calcTotalField(const Greens_rect_2D_cpu &G, const PressureFieldSerial &chiEst, const PressureFieldComplexSerial &Pinit);

    void applyKappa(const PressureFieldSerial &CurrentPressureFieldSerial, std::vector<std::complex<double>> &pData);
    void createKappa(const FrequenciesGroup &freq, const Sources &src, const Receivers &recv);
    void deleteKappa();
};
