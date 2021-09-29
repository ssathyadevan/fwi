#pragma once

#include "contraction.h"
#include "grid2D.h"
#include "receivers.h"
#include "sources.h"
#include "utilityFunctions.h"
#include <Eigen/Dense>
#include <cassert>
#include <complexDataGrid2D.h>

using namespace Eigen;
 
namespace fwi
{
    namespace core
    {
        class greensRect2DCpu
        {
        public:
            greensRect2DCpu(
                const grid2D &grid_, const std::function<std::complex<double>(double, double)> gFunc, const Sources &source_, const Receivers &receiver_, double k_);

            ~greensRect2DCpu();

            const std::complex<double> *getGreensVolume() const { return gVol; }

            const complexDataGrid2D *getReceiverCont(int iRecv) const { return gRecv[iRecv]; }

            complexDataGrid2D contractWithField(const complexDataGrid2D &x) const;

            const grid2D &getGrid() const { return grid; }

            // Babak 2018 10 25: This method generates the dot product of two matrices Greens function and contrast Sources dW
            // Equation ID: "rel:buildField"

            complexDataGrid2D dot1(const complexDataGrid2D &dW) const;

        private:
            void createGreensVolume();

            void createGreensVolumeAnkit();

            void createGreensRecv();

            void deleteGreensRecv();

            std::function<std::complex<double>(double, double)> G_func;

            const grid2D grid;
            const Sources source;
            const Receivers receiver;
            const double k;

            std::complex<double> *gVol;
            std::vector<complexDataGrid2D *> gRecv;

            Matrix<std::complex<double>, Dynamic, Dynamic, RowMajor> G_vol2;
            void setGreensFunction(complexDataGrid2D &greensFunctionField, const std::function<std::complex<double>(double, double)> func);

            greensRect2DCpu(const greensRect2DCpu &) = delete;
            greensRect2DCpu &operator=(const greensRect2DCpu &) = delete;
        };
    }   // namespace core
}   // namespace fwi
