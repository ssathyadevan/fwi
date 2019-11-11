/*
 * contraction.h
 *
 *  Created on: Jun 25, 2015
 *      Author: Michiel de Reus
 */


#pragma once

#include <functional>
#include <complex>

void contract_Greens_rect_2D(const std::complex<double> *G, const std::complex<double> *x, std::complex<double> *y, const std::array<int, 2> &nx, int ldG);


