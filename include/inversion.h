#ifndef INVERSION_H
#define INVERSION_H

#include <complex>

#include <ProfileInterface.h>

#include "GreensFunctions.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"

#include <volField_rect_2D_cpu.h>

#include "calcField.h"
#include "einsum.h"
#include <array>
#include "mpi.h"
#include <string>


//Babak 2018 10 23: Improved the documentation of this header file by indicating the equation ID in the FWI_document.

template <class T>
T normSq(const std::complex<T> *data, int n) {
    T result = T(0.0);
    for(int i=0; i<n; i++) {
        result += std::norm(data[i]);
    }
    return result;
}


template<typename T, template<typename> class volComplexField, template<typename> class volField, template<typename> class Greens, template<typename> class Frequencies>
class Inversion
{
protected:
    const grid_rect_2D<T> &m_grid;
    const Sources_rect_2D<T> &m_src;
    const Receivers_rect_2D<T> &m_recv;
    const Frequencies<T> &m_freq;

    Greens<T> **m_greens;
    volComplexField<T> ***p_0;
    volComplexField<T> ***p_tot;

    volField<T> m_chi;
    ProfileInterface &m_profiler;

    const int m_nfreq;
    const int m_nrecv;
    const int m_nsrc;

public:
    Inversion(const grid_rect_2D<T> &grid, const Sources_rect_2D<T> &src, const Receivers_rect_2D<T> &recv, const Frequencies<T> &freq, ProfileInterface &profiler)
        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_greens(), p_0(), p_tot(), m_chi(m_grid), m_profiler(profiler), m_nfreq(m_freq.nFreq), m_nrecv(m_recv.nRecv), m_nsrc(m_src.nSrc)
    {
    }

    virtual void createGreens()
    {
        m_greens = new Greens<T>*[m_nfreq];

        for (int i=0; i<m_nfreq; i++)
        {
            m_greens[i] = new Greens<T>(m_grid, Helmholtz2D<T>, m_src, m_recv, m_freq.k[i]);
        }
    }

    virtual void deleteGreens()
    {
        for (int i=0; i<m_nfreq; i++)
            delete m_greens[i];
        delete[] m_greens;
        m_greens = nullptr;
    }

    virtual void SetBackground(const volField_rect_2D_cpu<T> &chi_) {
        assert(&m_chi.GetGrid() == &chi_.GetGrid());
        m_chi = chi_;
    }

    virtual void createP0()
    {
        assert(m_greens != nullptr);
        assert(p_0 == nullptr);

        p_0 = new volComplexField<T>**[m_nfreq];
        // Babak 2018 10 23: not clear how this p0 is calculated from the equations.
        // I assume the p0 can be calculated from the Born approximation by considering p_tot ~ p0; Maybe this is

         for (int i=0; i<m_nfreq; i++)
        {
            p_0[i] = new volComplexField<T>*[m_nsrc];

            for (int j=0; j<m_nsrc; j++)
            {
                p_0[i][j] = new volComplexField<T>(m_grid);
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

    //pure virtual function//
    virtual void createTotalField(const int &rank) = 0;


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



    virtual void calculateData(std::complex<T> *p_data)
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


    virtual T findRealRootFromCubic(T a, T b, T c, T d)
    {
        // assuming ax^3 + bx^2 +cx + d and assuming only one real root, which is expected in this algorithm
        // uses Cardano's formula
        T f = ((T(3.0) * c / a) - (std::pow(b,2) / std::pow(a,2)))/T(3.0);
        T g = ((T(2.0) * std::pow(b,3) / std::pow(a,3)) - (T(9.0) * b * c / std::pow(a,2)) + (T(27.0) * d / a)) / T(27.0);
        T h = ( std::pow(g, 2) / T(4.0) ) + ( std::pow(f, 3) / T(27.0));
        T r = -(g / T(2.0)) + std::sqrt(h);
        T s = std::cbrt(r);
        T t = -(g / T(2.0)) - std::sqrt(h);
        T u = std::cbrt(t);

        T realroot = s + u - (b/(T(3.0) * a));
        return realroot;

        // note that the other (complex) roots should be
        // std::complex<T>(x2) = -(s+u)/T(2.0) - (b/T(3.0)*a) + 1.0i * (s-u) * (std::sqrt(3)/2)
        // std::complex<T>(x3) = -(s+u)/T(2.0) - (b/T(3.0)*a) - 1.0i * (s-u) * (std::sqrt(3)/2)
        // but this is not tested
    }

    //purely virtual function//
    virtual volField_rect_2D_cpu<T> Reconstruct(const std::complex<T> *const p_data, const int &rank) = 0;


};

#endif
