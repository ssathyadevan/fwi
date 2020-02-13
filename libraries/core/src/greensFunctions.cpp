#include "greensFunctions.h"

namespace fwi {
	static const double pi = std::atan(1.0) * 4.0;
}

namespace GreensFunctions
{

float Helmholtz2D_real(float k, float r)
{
    float value = 0.0f;
    if (r != 0.0f)
    {
        value = 0.25f * static_cast<float>(std::cyl_neumann(0, k * r)) * k * k; // Babak 2018 10 25: GreensFunc2d (first part of rhs). y0 indicates the Bessel function of the first kind.
    }
    return value;
}

float Helmholtz2D_imag(float k, float r)
{
    float value = 0.0f;
    if (r != 0.0f)
    {
        value = -0.25f * static_cast<float>(std::cyl_bessel_j(0, k * r)) * k * k; // Babak 2018 10 25: Equation ID "GreensFunc2d": (second part of rhs). j0 indicates the Bessel function of the second kind.
    }
    return value;
}

double Helmholtz2D_real(double k, double r)
{
    double value = 0.0;
    if (r != 0.0)
    {
        value = 0.25 * std::cyl_neumann(0, k * r) * k * k; // Babak 2018 10 25: Equation ID "GreensFunc2d": (first part of rhs). y0 indicates the Bessel function of the first kind.
    }
    return value;
}

double Helmholtz2D_imag(double k, double r)
{
    double value = 0.0;
    if (r != 0.0)
    {
        value = -0.25 * std::cyl_bessel_j(0, k * r) * k * k; // Babak 2018 10 25: Equation ID "GreensFunc2d": (second part of rhs). j0 indicates the Bessel function of the second kind.

    }
    return value;
}

std::complex<double> Helmholtz2D(double k, double r)
{
    return std::complex<double>(Helmholtz2D_real(k, r), Helmholtz2D_imag(k, r)); // Babak 2018 10 25: Equation ID "GreensFunc2d": Creating a complex Greens function.
}

double Helmholtz3D_real(double k, double r)
{
    double value = 0.0;
    if (r != 0.0)
    {
        value = 0.25 / (r * fwi::pi) * std::cos(k * r) * k * k; // Babak 2018 10 25: Equation ID "GreensFunc2d": real pardouble of Greens function from a 3D Helmhotz equation.
    }
    return value;
}

double Helmholtz3D_imag(double k, double r)
{
    double value = 0.0;
    if (r != 0.0)
    {
        value = 0.25 / (r * fwi::pi) * std::sin(k * r) * k * k; // Babak 2018 10 25: Equation ID "GreensFunc2d": imnaginary pardouble of Greens function from a 3D Helmhotz equation
    }
    return value;
}

std::complex<double> Helmholtz3D(double k, double r)
{
    return std::complex<double>(Helmholtz3D_real(k, r), Helmholtz3D_imag(k, r)); // Babak 2018 10 25: Equation ID "GreensFunc2d": Creating complex Greens fucntion
}

} // namespace GreensFunctions
