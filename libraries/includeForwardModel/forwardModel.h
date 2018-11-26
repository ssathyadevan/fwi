#ifndef FORWARDMODEL_H
#define FORWARDMODEL_H

#include "forwardModelInterface.h"

class ForwardModel : public ForwardModelInterface
{

public:
    ForwardModel(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq, ProfileInterface &profiler, const volField_rect_2D_cpu chi);

    ~ForwardModel();

    virtual void createGreens() ;

    virtual void deleteGreens() ;

    virtual void SetBackground(const volField_rect_2D_cpu &chi_) ;

    virtual void createP0() ;

    virtual void deleteP0() ;

    virtual void deleteTotalField() ;

    virtual void calculateData(std::complex<double> *p_data) ;

    virtual void createTotalField(ConjGrad conjGrad) ;


};

#endif

