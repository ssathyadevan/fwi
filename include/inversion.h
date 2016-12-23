#ifndef INVERSION_H
#define INVERSION_H

#include <complex>

#include <ProfileInterface.h>

#include "GreensFunctions.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "frequencies.h"
#include "grid_rect_2D.h"

#include <volField_rect_2D_cpu.h>

#include "calcField.h"
#include "einsum.h"

template <class T>
T normSq(const std::complex<T> *data, int n) {
    T result = T(0.0);
    for(int i=0; i<n; i++) {
        result += std::norm(data[i]);
    }
    return result;
}

template <class T>
class Inversion {

public:

    virtual ~Inversion() {}
    virtual void createGreens() = 0;
    virtual void deleteGreens() = 0;
    virtual void SetBackground(const volField_rect_2D_cpu<T> &chi_) = 0;
    virtual void createP0() = 0;
    virtual void deleteP0() = 0;
    virtual void createTotalField() = 0;
    virtual void deleteTotalField() = 0;
    virtual void calculateData(std::complex<T> *p_data) = 0;
    virtual volField_rect_2D_cpu<T> Reconstruct(const std::complex<T> *p_data) = 0;

};


template<typename T, template<typename> class volComplexField, template<typename> class volField, template<typename> class Greens>
class InversionConcrete : public Inversion<T>
{
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
    InversionConcrete(const InversionConcrete<T,volComplexField,volField,Greens>&) = delete;
    InversionConcrete<T,volComplexField,volField,Greens>& operator=(const InversionConcrete<T,volComplexField,volField,Greens>&) = delete;

    InversionConcrete(const grid_rect_2D<T> &grid, const Sources_rect_2D<T> &src, const Receivers_rect_2D<T> &recv, const Frequencies<T> &freq, ProfileInterface &profiler)
        : m_grid(grid), m_src(src), m_recv(recv), m_freq(freq), m_greens(), p_0(), p_tot(), m_chi(m_grid), m_profiler(profiler), m_nfreq(m_freq.nFreq), m_nrecv(m_recv.nRecv), m_nsrc(m_src.nSrc)
    {
    }

    ~InversionConcrete()
    {
        if (m_greens!=nullptr)
            deleteGreens();

        if (p_0!=nullptr)
            deleteP0();

        if (p_tot!=nullptr)
            deleteTotalField();
    }

    void createGreens()
    {
        m_greens = new Greens<T>*[m_nfreq];

        for (int i=0; i<m_nfreq; i++)
        {
            m_greens[i] = new Greens<T>(m_grid, Helmholtz2D<T>, m_src, m_recv, m_freq.k[i]);
        }
    }

    void deleteGreens()
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

    void createP0()
    {
        assert(m_greens != nullptr);
        assert(p_0 == nullptr);

        p_0 = new volComplexField<T>**[m_nfreq];

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

    void deleteP0()
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

    void createTotalField()
    {
        assert(m_greens != nullptr);
        assert(p_0 != nullptr);
        assert(p_tot == nullptr);

        m_profiler.StartRegion("createTotalField");

        p_tot = new volComplexField<T>**[m_nfreq];

        for (int i=0; i<m_nfreq; i++)
        {
            p_tot[i] = new volComplexField<T>*[m_nsrc];

            std::cout << "  " << std::endl;
            std::cout << "Creating P_tot for " << i+1 << "/ " << m_nfreq << "freq" << std::endl;
            std::cout << "  " << std::endl;

            for (int j=0; j<m_nsrc; j++)
            {
                p_tot[i][j] = new volComplexField<T>(m_grid);
                *p_tot[i][j] = calcField<T,volComplexField,volField,Greens>(*m_greens[i], m_chi, *p_0[i][j]);
            }

            std::cout << "  " << std::endl;
            std::cout << "  " << std::endl;
        }

        m_profiler.EndRegion();
    }

    void deleteTotalField()
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



    void calculateData(std::complex<T> *p_data)
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

    volField_rect_2D_cpu<T> Reconstruct(const std::complex<T> *const p_data)
    {
        T eta = 1.0/normSq(p_data,m_nfreq*m_nrecv*m_nsrc);
        T res, gamma, alpha, alpha_num = T(0.0), alpha_den = T(0.0);

        volComplexField<T> **Kappa, **p_est;
        int l_i;
        const int n_total = m_nfreq*m_nrecv*m_nsrc;

        std::complex<T> *r = new std::complex<T>[n_total];
        std::complex<T> *K_zeta = new std::complex<T>[n_total];

        einsum<T,volComplexField,volField,Greens> ein(m_grid,m_src,m_recv,m_freq);

        volField<T> chi_est(m_grid), g(m_grid), g_old(m_grid), zeta(m_grid);
        volComplexField<T> tmp(m_grid);
        chi_est.Zero();

        volField<T> chi_old(chi_est);
        std::vector<std::complex<T>> res_first_it, res_inner_loop;


        Kappa = new volComplexField<T>*[n_total];
        for (int i = 0; i < n_total; i++)
        {
            Kappa[i] = new volComplexField<T>(m_grid);
        }

        p_est = new volComplexField<T>*[m_nfreq*m_nsrc];
        for (int i=0; i<m_nfreq; i++)
        {
            l_i = i*m_nsrc;
            for (int j=0; j<m_nsrc;j++)
                p_est[l_i + j] = new volComplexField<T>(*p_0[i][j]);
        }

        //main loop//
        for(int it=0; it<n_max; it++)
        {
            if (do_reg == 0)
            {
                ein.einsum_Gr_Pest(Kappa, m_greens, p_est);

                for (int it1=0; it1<n_iter1; it1++)
                {
                    for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                        r[i] = p_data[i] - Summation(*Kappa[i], chi_est);

                    res = eta*normSq(r,n_total);

                    if (it1 > 0)
                        std::cout << "inner loop residual = " << std::setprecision(17) << res << "     " << std::abs(res_first_it[it1-1] - res) << "    " << it1+1 << '/' << n_iter1 << std::endl;
                    else
                        std::cout << "inner loop residual = " << std::setprecision(17) << res << std::endl;


                    res_first_it.push_back(res);

                    if ( (it1 > 0) && ( (res < T(tol1)) || ( std::abs(res_first_it[it1-1] - res) < T(tol1) ) ) )
                        break;

                    ein.einsum_K_res(Kappa, r, tmp);  //tmp = einsum('ijkl,ijk->l', conj(K), r)
                    g = (2*eta) * tmp.GetRealPart();  //g = 2.0 * eta * real(tmp)

                    if (it1==0)
                        zeta = g;
                    else
                    {
                        gamma = g.InnerProduct(g-g_old) / g_old.InnerProduct(g_old);
                        zeta = g + gamma*zeta;
                    }

                    for (int i = 0; i < n_total; i++)
                    {
                        K_zeta[i] = Summation( *Kappa[i], zeta );
                        alpha_num += std::real( conj(r[i]) * K_zeta[i] );
                        alpha_den += std::real( conj(K_zeta[i]) * K_zeta[i] );
                    }

                    alpha = alpha_num / alpha_den;

                    chi_est += alpha*zeta;
                    g_old = g;
                    alpha_num = T(0.0);
                    alpha_den = T(0.0);
                }
            }


            //calculate p_data
            for (int i=0; i<m_nfreq; i++)
            {
                l_i = i*m_nsrc;
                std::cout << "  " << std::endl;
                std::cout << "Creating P_tot for " << i+1 << "/ " << m_nfreq << "freq" << std::endl;
                std::cout << "  " << std::endl;
                for (int j=0; j<m_nsrc;j++)
                    *p_est[l_i + j] = calcField<T,volComplexField,volField,Greens>(*m_greens[i], chi_est, *p_0[i][j]);
            }

        }

        //free memory
        for (int i = 0; i < n_total; i++)
            delete Kappa[i];
        delete[] Kappa;
        Kappa = nullptr;

        for (int i = 0; i < m_nfreq*m_nsrc; i++)
            delete p_est[i];
        delete[] p_est;
        p_est = nullptr;


        delete[] r;
        delete[] K_zeta;



        volField_rect_2D_cpu<T> result(m_grid);
        chi_est.CopyTo(result);
        return result;




    }



};

#endif
