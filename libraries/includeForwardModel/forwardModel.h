#ifndef FORWARDMODEL_H
#define FORWARDMODEL_H

#include "forwardModelInterface.h"

class ForwardModel : public ForwardModelInterface
{

public:
    ForwardModel(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq, ProfileInterface &profiler, const volField_rect_2D_cpu chi);

    ~ForwardModel();

    void createGreens() ;

    void deleteGreens() ;

    void SetBackground(const volField_rect_2D_cpu &chi_) ;

    void createP0() ;

    void deleteP0() ;

    void deleteTotalField() ;

    void calculateData(std::complex<double> *p_data) ;

    void createTotalField(ConjGrad conjGrad) ;


};

#endif

