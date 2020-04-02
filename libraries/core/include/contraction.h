/*
 * contraction.h
 *
 *  Created on: Jun 25, 2015
 *      Author: Michiel de Reus
 */

#pragma once

#include "pressureFieldComplexSerial.h"
#include <complex>
#include <functional>

void createGreensRect2D(std::complex<double> *G, const std::array<double, 2> &dx, const std::array<int, 2> &nx,
    std::function<std::complex<double>(double, double)> gFunc, double k);

void contractGreensRect2D(
    const std::complex<double> *G, const pressureFieldComplexSerial &x, pressureFieldComplexSerial &testField, const std::array<int, 2> &nx, int ldG);
