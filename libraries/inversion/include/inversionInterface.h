#ifndef INVERSIONINTERFACE_H
#define INVERSIONINTERFACE_H

#include <complex>

#include "greens_rect_2D_cpu.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"
#include "frequencies_group.h"
#include "GreensFunctions.h"
#include <volField_rect_2D_cpu.h>
#include "variable_structure.h"

#include "calcField.h"
#include <array>
#include <string>

class InversionInterface
{

public:
    InversionInterface() {}
    virtual ~InversionInterface()
    {
    }

    virtual volField_rect_2D_cpu Reconstruct(const std::complex<double> *const p_data, Input input) = 0;
};

#endif
