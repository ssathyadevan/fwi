#include "newPropietaryInversion.h"

namespace fwi
{
    namespace inversionMethods
    {
        NewPropietaryInversion::NewPropietaryInversion()
        {
        }

        core::dataGrid2D NewPropietaryInversion::reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput)
        {
			(void) pData;
			(void) gInput;
			std::cout<<"\nHey! you got access to a new inversion method\n";
			return core::dataGrid2D(core::grid2D(xmin,xmax,nx));
		}
	}   // namespace inversionMethods
}   // namespace fwi
