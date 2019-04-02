#include "utilityFunctions.h"
#include "contraction.h"

#include <array>

void create_Greens_rect_2D(std::complex<double> *G, const std::array<double,2> &dx, const std::array<int,2> &nx, std::function< std::complex<double>(double,double) > G_func, double k)
{

  double vol = dx[0] * dx[1];
  for(int i=-nx[1]+1; i <= nx[1]-1; i++)
  {
    double z = i * dx[1];
    for(int j=-nx[0]+1; j <= nx[0]-1; j++)
    {
      double x = j * dx[0];
      double r = dist(z, x);
      G[(nx[1] + i - 1) * 2 * nx[0] + (nx[0] + j - 1)] = G_func(k, r) * vol;
    }
  }
}


void contract_Greens_rect_2D(const std::complex<double> *G, const std::complex<double> *x, std::complex<double> *y, const std::array<int,2> &nx, int ldG)
{

  for(int i=0; i<nx[1]; i++)
  {
    for(int j=0; j<nx[0]; j++)
    {
      y[(nx[1] - i - 1) * nx[0] + (nx[0] - j - 1)] = 0.0;
      for(int k=0; k<nx[1]; k++)
      {
        for(int l=0; l<nx[0]; l++)
        {
          y[(nx[1] - i - 1) * nx[0] + (nx[0] - j - 1)] += G[(i + k) * ldG + j + l] * x[k * nx[0] + l];
        }
      }
    }
  }
}


