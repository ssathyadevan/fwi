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
  return (r == 0.0f ? 0.0f : 0.25f * static_cast<float>(y0(k * r)) * k * k);// Babak 2018 10 25: GreensFunc2d (first part of lhs). y0 indicates the Bessel function of the first kind.
}

inline float Helmholtz2D_imag(float k, float r) {
  //const float R = 4.0f;
  //return (r == 0.0f ? 0.5f * k * M_PI * j1f(k * R) : 0.25f * j0f(k * r) * k * k);
  return (r == 0.0f ? 0.0f : -0.25f * static_cast<float>(j0(k * r)) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": (second part of lhs). j0 indicates the Bessel function of the second kind.
}

inline double Helmholtz2D_real(double k, double r) {
  return (r == 0.0 ? 0.0 : 0.25 * y0(k * r) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": (first part of lhs). y0 indicates the Bessel function of the first kind.
}

inline double Helmholtz2D_imag(double k, double r) {
  return (r == 0.0 ? 0.0 : -0.25 * j0(k * r) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": (second pardouble  of lhs). j0 indicates the Bessel function of the second kind.
}


inline std::complex<double> Helmholtz2D(double k, double r) {
  return std::complex<double> (Helmholtz2D_real(k, r), Helmholtz2D_imag(k, r));// Babak 2018 10 25: Equation ID "GreensFunc2d": Creating a complex Greens function.
}

inline double Helmholtz3D_real(double k, double r) {
  return (r == 0.0 ? 0.0 : 0.25 / (r * M_PI) * std::cos(k * r) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": real pardouble of Greens function from a 3D Helmhotz equation.
}


inline double Helmholtz3D_imag(double k, double r) {
  return (r == 0.0 ? 0.0 : 0.25 / (r * M_PI) * std::sin(k * r) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": imnaginary pardouble of Greens function from a 3D Helmhotz equation
}


inline std::complex<double>  Helmholtz3D(double k, double r) {
  return std::complex<double> (Helmholtz3D_real(k, r), Helmholtz3D_imag(k, r));// Babak 2018 10 25: Equation ID "GreensFunc2d": Creating complex Greens fucntion
}

#endif /* GREENSFUNCTIONS_H */
