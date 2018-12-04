#ifndef FORWARDMODEL_H
#define FORWARDMODEL_H

#include "forwardModelInterface.h"

class ForwardModel : public ForwardModelInterface
{

public:
    ForwardModel(const grid_rect_2D &grid, const Sources_rect_2D &src,
                 const Receivers_rect_2D &recv, const Frequencies_group &freq,
                 ProfileInterface &profiler, const volField_rect_2D_cpu chi);

    ~ForwardModel();

    void createGreens() ;

    void deleteGreens() ;

    void SetBackground(const volField_rect_2D_cpu &chi_) ;

    void createP0() ;

    void deleteP0() ;

    void deleteTotalField() ;

    void calculateData(std::complex<double> *p_data) ;

    void createTotalField(ConjGrad conjGrad) ;

    virtual void createTotalField1D(ConjGrad conjGrad, volField_rect_2D_cpu chi_est);

    const grid_rect_2D& get_m_grid() ;

    const Sources_rect_2D& get_m_src() ;

    const Receivers_rect_2D& get_m_recv() ;

    const Frequencies_group& get_m_freq() ;

    virtual ProfileInterface& get_m_profiler() ;

    virtual const int get_m_nfreq() ;

    virtual const int get_m_nrecv() ;

    virtual const int get_m_nsrc() ;

    virtual volComplexField_rect_2D_cpu*** get_p_0() ;

    virtual Greens_rect_2D_cpu** get_m_greens() ;

    virtual volComplexField_rect_2D_cpu** getKappa();

    virtual void calculateKappa();

    virtual void calculateResidual(volField_rect_2D_cpu chi_est, const std::complex<double> *const p_data);

    virtual std::complex<double>* get_residual();

    virtual double calculateResidualNormSq(double eta);

    virtual void calculateK_zeta(volField_rect_2D_cpu zeta);

    virtual std::complex<double>* get_K_zeta();

};

#endif

