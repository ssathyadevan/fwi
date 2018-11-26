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

float Helmholtz2D_real(float k, float r);

float Helmholtz2D_imag(float k, float r);

double Helmholtz2D_real(double k, double r);

double Helmholtz2D_imag(double k, double r);

std::complex<double> Helmholtz2D(double k, double r);

double Helmholtz3D_real(double k, double r);

double Helmholtz3D_imag(double k, double r);

std::complex<double>  Helmholtz3D(double k, double r);

#endif /* GREENSFUNCTIONS_H */
