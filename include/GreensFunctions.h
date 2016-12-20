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

inline float Helmholtz2D_real(float k, float r) {
  //const float R = 4.0f;
  //return (r == 0.0f ? -1.0f - 0.5f * k * M_PI * y1f(k * R) : -0.25f * y0f(k * r) * k * k);
  return (r == 0.0f ? 0.0f : -0.25f * static_cast<float>(y0(k * r)) * k * k);
}

inline float Helmholtz2D_imag(float k, float r) {
  //const float R = 4.0f;
  //return (r == 0.0f ? 0.5f * k * M_PI * j1f(k * R) : 0.25f * j0f(k * r) * k * k);
  return (r == 0.0f ? 0.0f : 0.25f * static_cast<float>(j0(k * r)) * k * k);
}

inline double Helmholtz2D_real(double k, double r) {
  return (r == 0.0 ? 0.0 : -0.25 * y0(k * r) * k * k);
}

inline double Helmholtz2D_imag(double k, double r) {
  return (r == 0.0 ? 0.0 : 0.25 * j0(k * r) * k * k);
}

template <typename T>
inline std::complex<T> Helmholtz2D(T k, T r) {
  return std::complex<T>(Helmholtz2D_real(k, r), Helmholtz2D_imag(k, r));
}

template <typename T>
inline T Helmholtz3D_real(T k, T r) {
  return (r == 0.0 ? 0.0 : 0.25 / (r * M_PI) * std::cos(k * r) * k * k);
}

template <typename T>
inline T Helmholtz3D_imag(T k, T r) {
  return (r == 0.0 ? 0.0 : 0.25 / (r * M_PI) * std::sin(k * r) * k * k);
}

template <typename T>
inline std::complex<T> Helmholtz3D(T k, T r) {
  return std::complex<T>(Helmholtz3D_real(k, r), Helmholtz3D_imag(k, r));
}

#endif /* GREENSFUNCTIONS_H */
