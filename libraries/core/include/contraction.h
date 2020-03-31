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

void create_Greens_rect_2D(std::complex<double> *G, const std::array<double, 2> &dx, const std::array<int, 2> &nx,
    std::function<std::complex<double>(double, double)> G_func, double k);

void contract_Greens_rect_2D(
    const std::complex<double> *G, const PressureFieldComplexSerial &x, PressureFieldComplexSerial &testField, const std::array<int, 2> &nx, int ldG);
