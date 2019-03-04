#ifndef FORWARDMODELBASICOPTIMIZATION_H
#define FORWARDMODELBASICOPTIMIZATION_H

#include "forwardModelInterface.h"

class forwardModelBasicOptimization : public ForwardModelInterface
{

public:

    forwardModelBasicOptimization( const grid2D &grid, const sources &src, const receivers &recv,
                    const frequenciesGroup &freq, const forwardModelInput &fmInput );

    ~forwardModelBasicOptimization();

    virtual void initializeForwardModel(const pressureFieldSerial &chiEst);

    virtual std::complex<double>* createPdataEst( const pressureFieldSerial &chiEst );

    std::complex<double>* createKappaOperator(const pressureFieldSerial &CurrentPressureFieldSerial);
    std::complex<double>* createKappaOperator(const pressureFieldComplexSerial &CurrentPressureFieldComplexSerial);

private:

    Greens_rect_2D_cpu **m_greens;
    pressureFieldComplexSerial **m_Kappa;

    pressureFieldComplexSerial ***m_P0;
    pressureFieldComplexSerial ***m_Ptot;
    pressureFieldComplexSerial **m_Ptot1D;

    void createP0();
    void deleteP0();

    void createGreensSerial();
    void deleteGreensSerial();

    void createPtot( const pressureFieldSerial &chiEst );
    void createPtot1D( const pressureFieldSerial &chiEst );
    void deletePtot();
    void deletePtot1D();

    pressureFieldComplexSerial calcTotalField( const Greens_rect_2D_cpu &G, const pressureFieldSerial &chiEst, const pressureFieldComplexSerial &Pinit );

    void calculateKappa( const pressureFieldSerial &chiEst );

};

#endif
