#ifndef CALCFIELD_H
#define CALCFIELD_H

#include "grid2D.h"
#include <Eigen/Dense>
#include "greensSerial.h"
#include "pressureFieldComplexSerial.h"
#include "variableStructure.h"

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
pressureFieldComplexSerial calcField(const Greens_rect_2D_cpu &G, const pressureFieldSerial &chi,
                                      const pressureFieldComplexSerial &p_init, Iter2 iter2);

#endif /* CALCFIELD_H */
