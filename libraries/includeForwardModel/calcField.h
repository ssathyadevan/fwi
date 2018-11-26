#ifndef CALCFIELD_H
#define CALCFIELD_H

#include "grid_rect_2D.h"
#include <eigen3/Eigen/Dense>
#include "ProfileCpu.h"
#include "greens_rect_2D_cpu.h"
#include "volComplexField_rect_2D_cpu.h"
#include "variable_structure.h"

#include <complex>
#include <vector>
#include <cassert>
#include <string>

extern const int g_verbosity;

using namespace Eigen;

/*
    Babak 2018 10 24: This is the header file to do nonlinear field update on the domain equation
    by calculating p_tot from the following equation IDs in the FWI_document:
    "incrementalContrastSrcs"
    "weightingFactorsField"
*/
volComplexField_rect_2D_cpu calcField(const Greens_rect_2D_cpu &G, const volField_rect_2D_cpu &chi,
                                      const volComplexField_rect_2D_cpu &p_init, ConjGrad conjGrad);

#endif /* CALCFIELD_H */
