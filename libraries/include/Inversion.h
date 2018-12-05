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

// this function calculates the argument of the Re() in eq: integrandForDiscreteK
inline void calculate_K_res(ForwardModelInterface* forwardModel, volComplexField_rect_2D_cpu &K_res)
{
    int l_i, l_j;
    K_res.Zero();
    volComplexField_rect_2D_cpu K_dummy(forwardModel->get_m_grid());

    for (int i = 0; i < forwardModel->getInput().freq.nTotal; i++)
    {
        l_i = i*forwardModel->getInput().nSourcesReceivers.rec*forwardModel->getInput().nSourcesReceivers.src;

        for (int j = 0; j < forwardModel->getInput().nSourcesReceivers.rec; j++)
        {
            l_j = j*forwardModel->getInput().nSourcesReceivers.src;

            for(int k = 0; k < forwardModel->getInput().nSourcesReceivers.src; k++)
            {
                K_dummy = *forwardModel->getKappa()[l_i + l_j + k];
                K_dummy.Conjugate(); //take conjugate of elements of Kappa (required for algorithm einsum('ijkl,ijk->l',conk(K),r) )
                K_res += K_dummy * forwardModel->getResidual()[l_i + l_j + k];

            }
        }
    }
}

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
    volField_rect_2D_cpu Reconstruct(const std::complex<double> *const p_data, Input input);
};

#endif // INVERSION_CPU

