#ifndef FORWARDMODEL_H
#define FORWARDMODEL_H

#include "forwardModelInterface.h"

class ForwardModel : public ForwardModelInterface
{
public:

    ForwardModel(const grid_rect_2D &grid, const Sources_rect_2D &src,
                 const Receivers_rect_2D &recv, const Frequencies_group &freq,
                 Input input);

    ~ForwardModel();

    void createGreens() ;

    void deleteGreens() ;

    void createP0() ;

    void deleteP0() ;

    void deleteTotalField() ;

    void calculateData(std::complex<double> *p_data, volField_rect_2D_cpu chi, Iter2 conjGrad) ;

    void createTotalField(Iter2 conjGrad, volField_rect_2D_cpu chi) ;

    virtual void createTotalField1D(Iter2 conjGrad, volField_rect_2D_cpu chi_est);

    virtual Input getInput() ;

    void calculateKappa();

    virtual void intermediateForwardModelStep1();

    virtual void calculateResidual(volField_rect_2D_cpu chi_est, const std::complex<double> *const p_data);

    virtual std::complex<double>* getResidual();

    virtual double calculateResidualNormSq(double eta);

    virtual void calculateKZeta(volField_rect_2D_cpu zeta);

    virtual void calculateKRes(volComplexField_rect_2D_cpu &kRes) ;

    virtual std::complex<double>* intermediateForwardModelStep2(volField_rect_2D_cpu zeta);



private:

    Greens_rect_2D_cpu **greens;
    volComplexField_rect_2D_cpu ***p0;
    volComplexField_rect_2D_cpu ***pTot;
    volComplexField_rect_2D_cpu **Kappa; // greens*p_tot in eq:ModelDataGreensConv
    volComplexField_rect_2D_cpu **pTot1D; // p_tot stored as a 1D array
    std::complex<double>* residual; // residual for the loop with input.iter1.n
    std::complex<double> *kZeta ;
};

#endif
