#ifndef FORWARDMODEL_H
#define FORWARDMODEL_H

#include "forwardModelInterface.h"

class ForwardModel : public ForwardModelInterface
{

public:
    ForwardModel(const grid_rect_2D &grid, const Sources_rect_2D &src,
                 const Receivers_rect_2D &recv, const Frequencies_group &freq,
                 ProfileInterface &profiler, Input input);

    ~ForwardModel();

    void createGreens() ;

    void deleteGreens() ;

    void createP0() ;

    void deleteP0() ;

    void deleteTotalField() ;

    void calculateData(std::complex<double> *p_data, volField_rect_2D_cpu chi, ConjGrad conjGrad) ;

    void createTotalField(ConjGrad conjGrad, volField_rect_2D_cpu chi) ;

    virtual void createTotalField1D(ConjGrad conjGrad, volField_rect_2D_cpu chi_est);

    virtual ProfileInterface& get_m_profiler() ;

    virtual Input getInput() ;

    virtual Greens_rect_2D_cpu** get_m_greens() ;

    virtual volComplexField_rect_2D_cpu** getKappa();

    virtual void calculateKappa();

    virtual void calculateResidual(volField_rect_2D_cpu chi_est, const std::complex<double> *const p_data);

    virtual std::complex<double>* getResidual();

    virtual double calculateResidualNormSq(double eta);

    virtual void calculateK_zeta(volField_rect_2D_cpu zeta);

    virtual std::complex<double>* get_K_zeta();
private:
    Greens_rect_2D_cpu **m_greens;
    volComplexField_rect_2D_cpu ***p_0;
    volComplexField_rect_2D_cpu ***p_tot;

    ProfileInterface &m_profiler;

    volComplexField_rect_2D_cpu **Kappa; // greens*p_tot in eq:ModelDataGreensConv
    volComplexField_rect_2D_cpu **p_tot1D; // p_tot stored as a 1D array
    std::complex<double>* residual; // residual for the loop with input.iter1.n
    std::vector<std::complex<double>> res_first_it; // normsquare of above residual (each element
                                                    // of this vector is the residual corresponding to that iteration in the iter1 loop)
    std::complex<double> *K_zeta ;
};

#endif

