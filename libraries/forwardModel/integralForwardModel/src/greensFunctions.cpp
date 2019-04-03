#include "greensFunctions.h"

namespace GreensFunctions {

float Helmholtz2D_real(float k, float r)
{
    return (r == 0.0f ? 0.0f : 0.25f * static_cast<float>(std::cyl_neumann(0,k * r)) * k * k);// Babak 2018 10 25: GreensFunc2d (first part of rhs). y0 indicates the Bessel function of the first kind.
}

float Helmholtz2D_imag(float k, float r)
{
    return (r == 0.0f ? 0.0f : -0.25f * static_cast<float>(std::cyl_bessel_j(0,k * r)) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": (second part of rhs). j0 indicates the Bessel function of the second kind.
}

double Helmholtz2D_real(double k, double r)
{
    return (r == 0.0 ? 0.0 : 0.25 * std::cyl_neumann(0,k * r) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": (first part of rhs). y0 indicates the Bessel function of the first kind.
}

double Helmholtz2D_imag(double k, double r)
{
    return (r == 0.0 ? 0.0 : -0.25 * std::cyl_bessel_j(0,k * r) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": (second part of rhs). j0 indicates the Bessel function of the second kind.
}


std::complex<double> Helmholtz2D(double k, double r)
{
    return std::complex<double> (Helmholtz2D_real(k, r), Helmholtz2D_imag(k, r));// Babak 2018 10 25: Equation ID "GreensFunc2d": Creating a complex Greens function.
}

double Helmholtz3D_real(double k, double r)
{
    return (r == 0.0 ? 0.0 : 0.25 / (r * M_PI) * std::cos(k * r) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": real pardouble of Greens function from a 3D Helmhotz equation.
}


double Helmholtz3D_imag(double k, double r)
{
    return (r == 0.0 ? 0.0 : 0.25 / (r * M_PI) * std::sin(k * r) * k * k);// Babak 2018 10 25: Equation ID "GreensFunc2d": imnaginary pardouble of Greens function from a 3D Helmhotz equation
}


std::complex<double>  Helmholtz3D(double k, double r)
{
    return std::complex<double> (Helmholtz3D_real(k, r), Helmholtz3D_imag(k, r));// Babak 2018 10 25: Equation ID "GreensFunc2d": Creating complex Greens fucntion
}

} //namespace
