#ifndef FORWARDMODEL_H
#define FORWARDMODEL_H

#include "forwardModelInterface.h"

class forwardModel : public ForwardModelInterface
{
public:

    forwardModel(const grid2D &grid, const sources &src,
                 const receivers &recv, const frequenciesGroup &freq,
                 Input input);

    ~forwardModel();

    void createGreens() ;

    void deleteGreens() ;

    void createP0() ;

    void deleteP0() ;

    void deleteTotalField() ;

    void calculateData(std::complex<double> *p_data, pressureFieldSerial chi, Iter2 conjGrad) ;

    void createTotalField(Iter2 conjGrad, pressureFieldSerial chi) ;

    virtual void createTotalField1D(Iter2 conjGrad, pressureFieldSerial chi_est);

    virtual Input getInput() ;

    void calculateKappa();

    virtual void intermediateForwardModelStep1();

    virtual void calculateResidual(pressureFieldSerial chi_est, const std::complex<double> *const p_data);

    virtual std::complex<double>* getResidual();

    virtual double calculateResidualNormSq(double eta);

    virtual void calculateKZeta(pressureFieldSerial zeta);

    virtual void calculateKRes(pressureFieldComplexSerial &kRes) ;

    virtual std::complex<double>* intermediateForwardModelStep2(pressureFieldSerial zeta);



private:

    Greens_rect_2D_cpu **greens;
    pressureFieldComplexSerial ***p0;
    pressureFieldComplexSerial ***pTot;
    pressureFieldComplexSerial **Kappa; // greens*p_tot in eq:ModelDataGreensConv
    pressureFieldComplexSerial **pTot1D; // p_tot stored as a 1D array
    std::complex<double>* residual; // residual for the loop with input.iter1.n
    std::complex<double> *kZeta ;
};

#endif

