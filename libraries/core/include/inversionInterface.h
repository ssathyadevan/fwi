#ifndef INVERSIONINTERFACE_H
#define INVERSIONINTERFACE_H

#include "calcField.h"

class inversionInterface
{

public:
    inversionInterface() {}
    virtual ~inversionInterface() = default;

    virtual pressureFieldSerial Reconstruct(const std::complex<double> *const p_data, Input input) = 0;
};

#endif
