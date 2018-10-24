/*
 * GreensFunctions.h
 *
 *  Created on: Jun 25, 2015
 *      Author: Michiel de Reus
 */

#ifndef GREENSFUNCTIONS_H
#define GREENSFUNCTIONS_H

#include <cmath>
#include <complex>
/*
   Babak 2018 10 23: These are not the Helmholtz equations but the Greens function for the Helmholtz equation.
   For clarification, the equation ID is indicated in the code correspondent to the equation ID in the FWI_document.
   comment: It is more clear to choose a different method names related to Greens.
*/
inline float Helmholtz2D_real(float k, float r) {
  //const float R = 4.0f;
  //return (r == 0.0f ? -1.0f - 0.5f * k * M_PI * y1f(k * R) : -0.25f * y0f(k * r) * k * k);
  return (r == 0.0f ? 0.0f : 0.25f * static_cast<float>(y0(k * r)) * k * k);// GreensFunc2d (first part of lhs). y0 indicates the Bessel function of the first kind.
}

inline float Helmholtz2D_imag(float k, float r) {
  //const float R = 4.0f;
  //return (r == 0.0f ? 0.5f * k * M_PI * j1f(k * R) : 0.25f * j0f(k * r) * k * k);
  return (r == 0.0f ? 0.0f : -0.25f * static_cast<float>(j0(k * r)) * k * k);// GreensFunc2d (second part of lhs). j0 indicates the Bessel function of the second kind.
}

inline double Helmholtz2D_real(double k, double r) {
  return (r == 0.0 ? 0.0 : 0.25 * y0(k * r) * k * k);// GreensFunc2d (first part of lhs). y0 indicates the Bessel function of the first kind.
}

inline double Helmholtz2D_imag(double k, double r) {
  return (r == 0.0 ? 0.0 : -0.25 * j0(k * r) * k * k);// GreensFunc2d (second part of lhs). j0 indicates the Bessel function of the second kind.
}

template <typename T>
inline std::complex<T> Helmholtz2D(T k, T r) {
  return std::complex<T>(Helmholtz2D_real(k, r), Helmholtz2D_imag(k, r));// Creating a complex Greens function.
}

template <typename T>
inline T Helmholtz3D_real(T k, T r) {
  return (r == 0.0 ? 0.0 : 0.25 / (r * M_PI) * std::cos(k * r) * k * k);// real part of Greens function from a 3D Helmhotz equation.
}

template <typename T>
inline T Helmholtz3D_imag(T k, T r) {
  return (r == 0.0 ? 0.0 : 0.25 / (r * M_PI) * std::sin(k * r) * k * k);// imnaginary part of Greens function from a 3D Helmhotz equation
}

template <typename T>
inline std::complex<T> Helmholtz3D(T k, T r) {
  return std::complex<T>(Helmholtz3D_real(k, r), Helmholtz3D_imag(k, r));// Creating complex Greens fucntion
}

#endif /* GREENSFUNCTIONS_H */
