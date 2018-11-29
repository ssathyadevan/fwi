#ifndef INVERSIONINTERFACE_H
#define INVERSIONINTERFACE_H

#include <complex>
#include <ProfileInterface.h>

#include "greens_rect_2D_cpu.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"
#include "frequencies_group.h"
#include "GreensFunctions.h"
#include <volField_rect_2D_cpu.h>
#include "variable_structure.h"

#include "calcField.h"
#include "einsum.h"
#include <array>
#include <string>

inline double normSq(const std::complex<double> *data, int n)
{
    double result = double(0.0);
    for(int i=0; i<n; i++)
    {
        result += std::norm(data[i]);
    }
    return result;
}

class InversionInterface
{

public:
    InversionInterface() {}

    virtual volField_rect_2D_cpu Reconstruct(const std::complex<double> *const p_data, Input input) = 0;
};

#endif
