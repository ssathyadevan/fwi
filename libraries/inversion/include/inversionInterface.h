#ifndef INVERSIONINTERFACE_H
#define INVERSIONINTERFACE_H

#include "calcField.h"

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
