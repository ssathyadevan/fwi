#ifndef FORWARDMODELBASICOPTIMIZATION_H
#define FORWARDMODELBASICOPTIMIZATION_H

#include "forwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"

class forwardModelBasicOptimization : public ForwardModelInterface
{

public:

    forwardModelBasicOptimization( const grid2D &grid, const sources &src, const receivers &recv,
                    const frequenciesGroup &freq, const forwardModelInput &fmInput );

    ~forwardModelBasicOptimization();

    virtual void calculatePdataEst(const pressureFieldSerial &chiEst, std::complex<double> *pData);
    virtual void getPdataEst(const pressureFieldSerial &chiEst, std::complex<double> *pData);

    virtual void updateForwardModel(const pressureFieldSerial &chiEst);

    // Ideally these should be private
    void initializeForwardModel(const pressureFieldSerial &chiEst);

    std::complex<double>* createKappaOperator(const pressureFieldSerial &CurrentPressureFieldSerial);
    std::complex<double>* createKappaOperator(const pressureFieldComplexSerial &CurrentPressureFieldComplexSerial);

    void calculateOperatorKres(pressureFieldComplexSerial &kRes, std::complex<double>* res);

private:

    Greens_rect_2D_cpu          **_Greens;

    pressureFieldComplexSerial  ***_P0;
    pressureFieldComplexSerial  ***_Ptot;
    pressureFieldComplexSerial  **_Ptot1D;
    pressureFieldComplexSerial  **_Kappa;

    void createP0();
    void deleteP0();

    void createGreensSerial();
    void deleteGreensSerial();

    void createPtot(const pressureFieldSerial &chiEst);
    void createPtot1D(const pressureFieldSerial &chiEst);
    void deletePtot();
    void deletePtot1D();

    pressureFieldComplexSerial calcTotalField(const Greens_rect_2D_cpu &G, const pressureFieldSerial &chiEst, const pressureFieldComplexSerial &Pinit);

    void calculateKappa(const pressureFieldSerial &chiEst);
    void deleteKappa();
};

#endif
