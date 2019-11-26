#include <gtest/gtest.h>
#include "conjugateGradientInversion.h"

// double ConjugateGradientInversion::findRealRootFromCubic(double a, double b, double c, double d)
// {
//     // assuming ax^3 + bx^2 +cx + d and assuming only one real root, which is expected in this algorithm
//     // uses Cardano's formula
//     double f = ((double(3.0) * c / a) - (std::pow(b, 2) / std::pow(a, 2))) / double(3.0);
//     double g = ((double(2.0) * std::pow(b, 3) / std::pow(a, 3)) -
//                 (double(9.0) * b * c / std::pow(a, 2)) +
//                 (double(27.0) * d / a)) /
//                double(27.0);
//     double h = (std::pow(g, 2) / double(4.0)) + (std::pow(f, 3) / double(27.0));
//     double r = -(g / double(2.0)) + std::sqrt(h);
//     double s = std::cbrt(r);
//     double t = -(g / double(2.0)) - std::sqrt(h);
//     double u = std::cbrt(t);

//     double realroot = s + u - (b / (double(3.0) * a));
//     return realroot;
// }
