#include "utilityFunctions.h"
#include "contraction.h"

#include <array>

void contract_Greens_rect_2D(const std::complex<double> *G, const std::complex<double> *x, std::complex<double> *y, const std::array<int, 2> &nx, int ldG)
{

  for (int i = 0; i < nx[1]; i++)
  {
    for (int j = 0; j < nx[0]; j++)
    {
      y[(nx[1] - i - 1) * nx[0] + (nx[0] - j - 1)] = 0.0;
      for (int k = 0; k < nx[1]; k++)
      {
        for (int l = 0; l < nx[0]; l++)
        {
          y[(nx[1] - i - 1) * nx[0] + (nx[0] - j - 1)] += G[(i + k) * ldG + j + l] * x[k * nx[0] + l];
        }
      }
    }
  }
}
