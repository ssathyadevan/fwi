#pragma once

#include <cassert>

#include "contraction.h"
#include "grid2D.h"
#include "receivers.h"
#include "sources.h"
#include "utilityFunctions.h"
#include <Eigen/Dense>
#include <pressureFieldComplexSerial.h>

using namespace Eigen;

class greensRect2DCpu
{
public:
    greensRect2DCpu(
        const grid2D &grid_, const std::function<std::complex<double>(double, double)> gFunc, const sources &src_, const receivers &recv_, double k_);

    ~greensRect2DCpu();

    const std::complex<double> *getGreensVolume() const { return gVol; }

    const pressureFieldComplexSerial *getReceiverCont(int iRecv) const { return gRecv[iRecv]; }

    pressureFieldComplexSerial contractWithField(const pressureFieldComplexSerial &x) const;

    const grid2D &getGrid() const { return grid; }

    // Babak 2018 10 25: This method generates the dot product of two matrices Greens function and contrast sources dW
    // Equation ID: "rel:buildField"

    pressureFieldComplexSerial dot1(const pressureFieldComplexSerial &dW) const;

private:
    void createGreensVolume();

    void createGreensVolumeAnkit();

    void createGreensRecv();

    void deleteGreensRecv();

    std::function<std::complex<double>(double, double)> G_func;

    const grid2D grid;
    const sources src;
    const receivers recv;
    const double k;

    std::complex<double> *gVol;
    std::vector<pressureFieldComplexSerial *> gRecv;

    Matrix<std::complex<double>, Dynamic, Dynamic, RowMajor> G_vol2;
    void setGreensFunction(pressureFieldComplexSerial &greensFunctionField, const std::function<std::complex<double>(double, double)> func);

    greensRect2DCpu(const greensRect2DCpu &) = delete;
    greensRect2DCpu &operator=(const greensRect2DCpu &) = delete;
};
