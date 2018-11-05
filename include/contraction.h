/*
 * contraction.h
 *
 *  Created on: Jun 25, 2015
 *      Author: Michiel de Reus
 */

#ifndef CONTRACTION_H
#define CONTRACTION_H

#include <functional>
#include <complex>

void create_Greens_rect_2D(std::complex<double> *G, const std::array<double, 2> &dx, const std::array<int, 2> &nx, std::function< std::complex<double>(double,double) > G_func, double k);

void contract_Greens_rect_2D(const std::complex<double> *G, const std::complex<double> *x, std::complex<double> *y, const std::array<int, 2> &nx, int ldG);

void contract_Greens_rect_2D_array(const std::complex<double> **G, const std::complex<double> **x, std::complex<double> **y, int nFreq, int nSrc, const std::array<int,2> &nx, int ldG);

#endif /* CONTRACTION_H */
