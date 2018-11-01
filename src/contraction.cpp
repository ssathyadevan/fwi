/*
 * contraction.h
 *
 *  Created on: Jun 25, 2015
 *      Author: Michiel de Reus
 */

#include <functional>
#include <complex>
#include <array>

#include <helper.h>

#include "contraction.h"

//template <typename T>
//void create_Greens_rect_2D(std::complex<T> *G, const std::array<T,2> &dx, const std::array<int,2> &nx, std::function< std::complex<T>(T,T) > G_func, T k) {

//  T vol = dx[0] * dx[1];

//  for(int i=-nx[1]+1; i <= nx[1]-1; i++) {
//    T z = i * dx[1];
//    for(int j=-nx[0]+1; j <= nx[0]-1; j++) {
//      T x = j * dx[0];

//      T r = dist(z, x);

//      G[(nx[1] + i - 1) * 2 * nx[0] + (nx[0] + j - 1)] = G_func(k, r) * vol;

//    }
//  }
//}

//template void create_Greens_rect_2D<float>(std::complex<float> *G, const std::array<float,2> &dx, const std::array<int,2> &nx, std::function< std::complex<float>(float,float) > G_func, float k);
//template void create_Greens_rect_2D<double>(std::complex<double> *G, const std::array<double, 2> &dx, const std::array<int,2> &nx, std::function< std::complex<double>(double,double) > G_func, double k);

template <typename T>
void contract_Greens_rect_2D(const std::complex<T> *G, const std::complex<T> *x, std::complex<T> *y, const std::array<int,2> &nx, int ldG) {

  for(int i=0; i<nx[1]; i++) {
    for(int j=0; j<nx[0]; j++) {

      y[(nx[1] - i - 1) * nx[0] + (nx[0] - j - 1)] = 0.0;

      for(int k=0; k<nx[1]; k++) {

        for(int l=0; l<nx[0]; l++) {
          y[(nx[1] - i - 1) * nx[0] + (nx[0] - j - 1)] += G[(i + k) * ldG + j + l] * x[k * nx[0] + l];
        }

      }

    }
  }
}

template void contract_Greens_rect_2D<float>(const std::complex<float> *G, const std::complex<float> *x, std::complex<float> *y, const std::array<int,2> &nx, int ldG);
template void contract_Greens_rect_2D<double>(const std::complex<double> *G, const std::complex<double> *x, std::complex<double> *y, const std::array<int,2> &nx, int ldG);


template <typename T>
void contract_Greens_rect_2D_array(const std::complex<T> **G, const std::complex<T> **x, std::complex<T> **y, int nFreq, int nSrc, const std::array<int,2> &nx, int ldG) {

  for(int iFreq=0; iFreq<nFreq; iFreq++) {
    for(int iSrc=0; iSrc<nSrc; iSrc++) {
      contract_Greens_rect_2D(G[iFreq], x[iFreq * nSrc + iSrc], y[iFreq * nSrc + iSrc], nx, ldG);
    }
  }

}

template void contract_Greens_rect_2D_array<float>(const std::complex<float> **G, const std::complex<float> **x, std::complex<float> **y, int nFreq, int nSrc, const std::array<int,2> &nx, int ldG);
template void contract_Greens_rect_2D_array<double>(const std::complex<double> **G, const std::complex<double> **x, std::complex<double> **y, int nFreq, int nSrc, const std::array<int,2> &nx, int ldG);
