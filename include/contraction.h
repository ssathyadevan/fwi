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

// Saurabh Sharma 31-10-2018, template below never used, hence commented
//template <typename T>
//void create_Greens_rect_2D(std::complex<T> *G, const std::array<T, 2> &dx, const std::array<int, 2> &nx, std::function< std::complex<T>(T,T) > G_func, T k);

template <typename T>
void contract_Greens_rect_2D(const std::complex<T> *G, const std::complex<T> *x, std::complex<T> *y, const std::array<int, 2> &nx, int ldG);

template <typename T>
void contract_Greens_rect_2D_array(const std::complex<T> **G, const std::complex<T> **x, std::complex<T> **y, int nFreq, int nSrc, const std::array<int,2> &nx, int ldG);

#endif /* CONTRACTION_H */

