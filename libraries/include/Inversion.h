#ifndef INVERSION_CPU
#define INVERSION_CPU

#include <complex>

#include <ProfileInterface.h>

#include "GreensFunctions.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"
#include "frequencies_group.h"
#include <volField_rect_2D_cpu.h>
#include <volComplexField_rect_2D_cpu.h>
#include "inversionInterface.h"
#include "calcField.h"
#include <array>
#include <string>
#include "variable_structure.h"
#include "forwardModelInterface.h"
#include "forwardModel.h"


using std::cout;
using std::endl;


class Inversion : public InversionInterface
{
private:
    ForwardModelInterface *forwardModel_;

public:
    Inversion(const Inversion&) = delete;
    Inversion& operator=(const Inversion&) = delete;

    Inversion(ForwardModelInterface *forwardModel)
    {
        forwardModel_ = forwardModel;
    }

    ~Inversion()
    {
    }

    double findRealRootFromCubic(double a, double b, double c, double d);
    volField_rect_2D_cpu Reconstruct(const std::complex<double> *const pData, Input input);
};

#endif // INVERSION_CPU

