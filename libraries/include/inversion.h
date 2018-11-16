#ifndef INVERSION_H
#define INVERSION_H

#include <complex>

#include <ProfileInterface.h>

#include "greens_rect_2D_cpu.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"
#include "frequencies_group.h"
#include "GreensFunctions.h"
#include <volField_rect_2D_cpu.h>
#include "variable_structure.h"

#include "calcField.h"
#include "einsum.h"
#include <array>
#include <string>
//#include  "/usr/include/mpi/mpi.h"


// Babak 2018 10 23: Improved the documentation of this header file by indicating the equation ID in the FWI_document.
//Babak 2018 10 29: Got rid of template
//Babak 2018 10 29: Gott rid of inheritence

/*
    Babak 2018 10 23: Improved the documentation of this header file by indicating the equation ID in the FWI_document.
    Babak 2018 10 29: Got rid of template
    Babak 2018 10 29: Gott rid of inheritence
    Babak 2018 11 05: Detemplating the class
*/

inline double normSq(const std::complex<double> *data, int n) {
    double result = double(0.0);
    for(int i=0; i<n; i++) {
        result += std::norm(data[i]);
    }
    return result;
}

// Babak 2018 10 29: remove templates from volField_rect_2D
//template<typename T, template<typename> class volComplexField_rect_2D_cpu, template<typename> class volField, template<typename> class Greens_rect_2D_cpu, template<typename> class Frequencies_group>
class Inversion
{
protected:
    // const grid_rect_2D<double> &m_grid;// Babak 2018 10 29: Get rid of template in grid_rect_2D class
    const grid_rect_2D &m_grid;
    const Sources_rect_2D &m_src;
    const Receivers_rect_2D &m_recv;
    const Frequencies_group &m_freq;

    Greens_rect_2D_cpu **m_greens;
    volComplexField_rect_2D_cpu ***p_0;
    volComplexField_rect_2D_cpu ***p_tot;

    volField_rect_2D_cpu m_chi;
    ProfileInterface &m_profiler;

    const int m_nfreq;
    const int m_nrecv;
    const int m_nsrc;

public:
//    Inversion(const grid_rect_2D<double> &grid, const Sources_rect_2D<double> &src, const Receivers_rect_2D<double> &recv, const Frequencies_group &freq, ProfileInterface &profiler)
//        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_greens(), p_0(), p_tot(), m_chi(m_grid), m_profiler(profiler), m_nfreq(m_freq.nFreq), m_nrecv(m_recv.nRecv), m_nsrc(m_src.nSrc)
        // Babak 2018 10 29: Get rid of template in grid_rect_2D class
    Inversion(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq, ProfileInterface &profiler)
            : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_greens(), p_0(), p_tot(), m_chi(m_grid), m_profiler(profiler), m_nfreq(m_freq.nFreq), m_nrecv(m_recv.nRecv), m_nsrc(m_src.nSrc)
    {
    }

    virtual void createGreens()
    {
        m_greens = new Greens_rect_2D_cpu*[m_nfreq];

        for (int i=0; i<m_nfreq; i++)
        {
            m_greens[i] = new Greens_rect_2D_cpu(m_grid, Helmholtz2D, m_src, m_recv, m_freq.k[i]);
        }
    }

    virtual void deleteGreens()
    {
        for (int i=0; i<m_nfreq; i++)
            delete m_greens[i];
        delete[] m_greens;
        m_greens = nullptr;
    }

    virtual void SetBackground(const volField_rect_2D_cpu &chi_) {
        assert(&m_chi.GetGrid() == &chi_.GetGrid());
        m_chi = chi_;
    }
    // Babak 2018 10 23: not clear how this p0 is calculated in createP0 from the equations.
    // I assume the p0 can be calculated from the Born approximation by considering p_tot ~ p0; Maybe this is
    virtual void createP0()
    {
        assert(m_greens != nullptr);
        assert(p_0 == nullptr);

        p_0 = new volComplexField_rect_2D_cpu**[m_nfreq];

         for (int i=0; i<m_nfreq; i++)
        {
            p_0[i] = new volComplexField_rect_2D_cpu*[m_nsrc];

            for (int j=0; j<m_nsrc; j++)
            {
                p_0[i][j] = new volComplexField_rect_2D_cpu(m_grid);
                *p_0[i][j] = *( m_greens[i]->GetReceiverCont(j) ) / (m_freq.k[i] * m_freq.k[i] * m_grid.GetCellVolume());
            }
        }

    }

    virtual void deleteP0()
    {
        for (int i=0; i<m_nfreq; i++)
        {
            for (int j=0; j<m_nsrc; j++)
                delete p_0[i][j];

            delete[] p_0[i];
        }
        delete[] p_0;
        p_0 = nullptr;
    }

    // Babak 2018 10 25: in createTotalField we use calcField class which is responsible
    // for creating the total field with Equation ID: "weightingFactorField"
    virtual void createTotalField(ConjGrad conjGrad) = 0;


    virtual void deleteTotalField()
    {
        for (int i=0; i<m_nfreq; i++)
        {
            for (int j=0; j<m_nsrc; j++)
                delete p_tot[i][j];

            delete[] p_tot[i];
        }
        delete[] p_tot;
        p_tot = nullptr;
    }


    virtual void calculateData(std::complex<double> *p_data)
    // Babak 2018 10 25: calculation of p_data based on "eq:calculateData" modified MELISSEN 2018 11 01
    {
        int l_i, l_j;
        for (int i=0; i<m_nfreq; i++)
        {
            l_i = i*m_nrecv*m_nsrc;
            for (int j=0; j<m_nrecv; j++)
            {
                l_j = j*m_nsrc;
                for (int k=0; k<m_nsrc; k++)
                {
                    p_data[l_i + l_j + k] = Summation( *( m_greens[i]->GetReceiverCont(j) ) , *p_tot[i][k]*m_chi );
                }
            }
        }
    }


    virtual double findRealRootFromCubic(double a, double b, double c, double d)
    {
        // assuming ax^3 + bx^2 +cx + d and assuming only one real root, which is expected in this algorithm
        // uses Cardano's formula
        double f = ((double(3.0) * c / a) - (std::pow(b,2) / std::pow(a,2)))/double(3.0);
        double g = ((double(2.0) * std::pow(b,3) / std::pow(a,3)) - (double(9.0) * b * c / std::pow(a,2)) + (double(27.0) * d / a)) / double(27.0);
        double h = ( std::pow(g, 2) / double(4.0) ) + ( std::pow(f, 3) / double(27.0));
        double r = -(g / double(2.0)) + std::sqrt(h);
        double s = std::cbrt(r);
        double t = -(g / double(2.0)) - std::sqrt(h);
        double u = std::cbrt(t);

        double realroot = s + u - (b/(double(3.0) * a));
        return realroot;

        // note that the other (complex) roots should be
        // std::complex<double>(x2) = -(s+u)/T(2.0) - (b/T(3.0)*a) + 1.0i * (s-u) * (std::sqrt(3)/2)
        // std::complex<double>(x3) = -(s+u)/T(2.0) - (b/T(3.0)*a) - 1.0i * (s-u) * (std::sqrt(3)/2)
        // but this is not tested
    }

    //purely virtual function//
    virtual volField_rect_2D_cpu Reconstruct(const std::complex<double> *const p_data, Iter1 iter1, ConjGrad conjgrad,
                                             DeltaAmplification deltaAmplification, int n_max, bool do_reg) = 0;


};

#endif
