#ifndef INVERSION_CPU
#define INVERSION_CPU


#include <complex>

#include <ProfileInterface.h>

#include "GreensFunctions.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"

#include <volField_rect_2D_cpu.h>
#include "inversion.h"
#include "calcField.h"
#include "einsum.h"
#include <array>
#include "mpi.h"
#include <string>


template<typename T, template<typename> class volComplexField, template<typename> class volField, template<typename> class Greens, template<typename> class Frequencies>
class InversionConcrete_cpu : public Inversion<T, volComplexField, volField, Greens, Frequencies>
{

public:
    InversionConcrete_cpu(const InversionConcrete_cpu<T,volComplexField,volField,Greens,Frequencies>&) = delete;
    InversionConcrete_cpu<T,volComplexField,volField,Greens,Frequencies>& operator=(const InversionConcrete_cpu<T,volComplexField,volField,Greens,Frequencies>&) = delete;

    InversionConcrete_cpu(const grid_rect_2D<T> &grid, const Sources_rect_2D<T> &src, const Receivers_rect_2D<T> &recv, const Frequencies<T> &freq, ProfileInterface &profiler)
    : Inversion<T, volComplexField, volField, Greens, Frequencies>(grid, src, recv, freq, profiler)
    {
    }

    ~InversionConcrete_cpu()
    {
        if (this->m_greens!=nullptr)
            this->deleteGreens();

        if (this->p_0!=nullptr)
            this->deleteP0();

        if (this->p_tot!=nullptr)
            this->deleteTotalField();
    }



    void createTotalField(const int &rank)
    {
        assert(this->m_greens != nullptr);
        assert(this->p_0 != nullptr);
        assert(this->p_tot == nullptr);

        std::string name = "createTotalField rank=" + std::to_string(rank);

        this->p_tot = new volComplexField<T>**[this->m_nfreq];
        this->m_profiler.StartRegion(name);
        for (int i=0; i<this->m_nfreq; i++)
        {
            this->p_tot[i] = new volComplexField<T>*[this->m_nsrc];

            if (rank==0)
            {
                std::cout << "  " << std::endl;
                std::cout << "Creating this->p_tot for " << i+1 << "/ " << this->m_nfreq << "freq" << std::endl;
                std::cout << "  " << std::endl;
            }


            for (int j=0; j<this->m_nsrc; j++)
            {
                this->p_tot[i][j] = new volComplexField<T>(this->m_grid);
                *this->p_tot[i][j] = calcField<T,volComplexField,volField,Greens>(*this->m_greens[i], this->m_chi, *this->p_0[i][j], rank);
            }


            if(rank==0)
            {
                std::cout << "  " << std::endl;
                std::cout << "  " << std::endl;
            }
        }
        this->m_profiler.EndRegion();


    }

    virtual volField_rect_2D_cpu<T> Reconstruct(const std::complex<T> *const p_data, const int &rank)
    {
        T const1 = normSq(p_data,this->m_nfreq*this->m_nrecv*this->m_nsrc);

        if ( sizeof(const1) == sizeof(double) )
            MPI_Allreduce(MPI_IN_PLACE, &const1, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        else
            MPI_Allreduce(MPI_IN_PLACE, &const1, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

        T eta = 1.0/const1;

        T gamma, alpha, res;

        std::array<T,2> alpha_div;

        volComplexField<T> **Kappa, **p_est;
        int l_i;
        const int n_total = this->m_nfreq*this->m_nrecv*this->m_nsrc;

        std::complex<T> *r = new std::complex<T>[n_total];
        std::complex<T> *K_zeta = new std::complex<T>[n_total];

        einsum<T,volComplexField,volField,Greens,Frequencies> ein(this->m_grid,this->m_src,this->m_recv,this->m_freq);

        volField<T> chi_est(this->m_grid), g(this->m_grid), g_old(this->m_grid), zeta(this->m_grid);
        volComplexField<T> tmp(this->m_grid);
        chi_est.Zero();

        Kappa = new volComplexField<T>*[n_total];
        for (int i = 0; i < n_total; i++)
        {
            Kappa[i] = new volComplexField<T>(this->m_grid);
        }

        p_est = new volComplexField<T>*[this->m_nfreq*this->m_nsrc];
        for (int i=0; i<this->m_nfreq; i++)
        {
            l_i = i*this->m_nsrc;
            for (int j=0; j<this->m_nsrc;j++)
                p_est[l_i + j] = new volComplexField<T>(*this->p_0[i][j]);
        }

        volField<T> **gradient_chi_old = new volField<T>*[2];
        volField<T> **gradient_greg_tmp = new volField<T>*[2];
        volField<T> **gradient_zeta_tmp = new volField<T>*[2];

        for (int i = 0; i < 2; i++)
        {
            gradient_chi_old[i] = new volField<T>(this->m_grid);
            gradient_greg_tmp[i] = new volField<T>(this->m_grid);
            gradient_zeta_tmp[i] = new volField<T>(this->m_grid);
        }


        //main loop//
        for(int it=0; it<n_max; it++)
        {
            std::vector<std::complex<T>> res_first_it;
            if (do_reg == 0)
            {
                ein.einsum_Gr_Pest(Kappa, this->m_greens, p_est);

                for (int it1=0; it1<n_iter1; it1++)
                {
                    for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                        r[i] = p_data[i] - Summation(*Kappa[i], chi_est);

                    res = eta*normSq(r,n_total);

                    if ( sizeof(res) == sizeof(double) )
                        MPI_Allreduce(MPI_IN_PLACE, &res, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
                    else
                        MPI_Allreduce(MPI_IN_PLACE, &res, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

                    if (rank==0)
                    {
                        if (it1 > 0)
                            std::cout << "inner loop residual = " << std::setprecision(17) << res << "     " << std::abs(res_first_it[it1-1] - res) << "    " << it1+1 << '/' << n_iter1 << std::endl;
                        else
                            std::cout << "inner loop residual = " << std::setprecision(17) << res << std::endl;
                    }

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

                    alpha_div[0] = 0.0;
                    alpha_div[1] = 0.0;
                    for (int i = 0; i < n_total; i++)
                    {
                        K_zeta[i] = Summation( *Kappa[i], zeta );
                        alpha_div[0] += std::real( conj(r[i]) * K_zeta[i] );
                        alpha_div[1] += std::real( conj(K_zeta[i]) * K_zeta[i] );
                    }

                    if ( sizeof(alpha) == sizeof(double) )
                        MPI_Allreduce(MPI_IN_PLACE, &alpha_div[0], alpha_div.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
                    else
                        MPI_Allreduce(MPI_IN_PLACE, &alpha_div[0], alpha_div.size(), MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

                    alpha = alpha_div[0] / alpha_div[1];

                    /*if (rank==1)
                    {
                        for (int i=0; i < this->m_grid.GetNumberOfGridPoints(); i++)
                            std::cout << i << "   " << *(g.GetDataPtr() + i) << std::endl;
                    }*/
                    //std::cout << rank << "  " << alpha << std::endl;

                    chi_est += alpha*zeta;
                    g_old = g;
                }
            }

            else if (do_reg == 1) {
                T deltasquared_old = T(0.0);
                volField<T> bsquared_old(this->m_grid);
                bsquared_old.Zero();
                T Freg_old = T(1.0);
                T Fdata_old = T(0.0);

                T delta_amplification = delta_amplification_start / (delta_amplification_slope * it + T(1.0));

                ein.einsum_Gr_Pest(Kappa, this->m_greens, p_est);

                //calculate initial residual
                for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                    r[i] = p_data[i] - Summation(*Kappa[i], chi_est);


                //start the inner loop
                for (int it1=0; it1<n_iter1; it1++)
                {
                    if (it1 == 0)
                    {
                        ein.einsum_K_res(Kappa, r, tmp);  //tmp = einsum('ijkl,ijk->l', conj(K), r)
                        g = eta * tmp.GetRealPart();

                        zeta = g;

                        alpha_div[0] = T(0.0);
                        alpha_div[1] = T(0.0);
                        for (int i = 0; i < n_total; i++)
                        {
                            K_zeta[i] = Summation( *Kappa[i], zeta );
                            alpha_div[0] += std::real( conj(r[i]) * K_zeta[i] );
                            alpha_div[1] += std::real( conj(K_zeta[i]) * K_zeta[i] );
                        }

                        if ( sizeof(alpha) == sizeof(double) )
                            MPI_Allreduce(MPI_IN_PLACE, &alpha_div[0], alpha_div.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
                        else
                            MPI_Allreduce(MPI_IN_PLACE, &alpha_div[0], alpha_div.size(), MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

                        alpha = alpha_div[0] / alpha_div[1];

                        chi_est += alpha*zeta;
                        g_old = g;

                        for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                            r[i] = p_data[i] - Summation(*Kappa[i], chi_est);

                        res = eta*normSq(r,n_total);

                        if ( sizeof(res) == sizeof(double) )
                            MPI_Allreduce(MPI_IN_PLACE, &res, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
                        else
                            MPI_Allreduce(MPI_IN_PLACE, &res, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

                        if (rank==0)
                            std::cout << it1+1 << "/" << n_iter1 << "\t (" << it+1 << "/" << n_max << ")\t res: " << std::setprecision(17) << res << std::endl;

                        Fdata_old = res;
                        res_first_it.push_back(res);
                    }
                    else
                    {
                        chi_est.Gradient(gradient_chi_old);
                        volField<T> gradient_chi_old_normsquared(this->m_grid);
                        gradient_chi_old_normsquared = (*gradient_chi_old[0] * *gradient_chi_old[0]) + (*gradient_chi_old[1] * *gradient_chi_old[1]);


                        volField<T> bsquared = ( gradient_chi_old_normsquared + deltasquared_old );
                        bsquared.Reciprocal();
                        bsquared *= (T(1.0) / (this->m_grid.GetDomainArea()) ); //bsquared = (1.0 / domain_area) * (1.0 / (gradient_chi_old_normsquared + deltasquared_old * ones((nz, nx))))  # eq. 2.22
                        volField<T> b = bsquared;
                        b.Sqrt();

                        volField<T> tmpvolfield = b * *gradient_chi_old[0];
                        tmpvolfield.Square();
                        volField<T> tmpvolfield2 = b * *gradient_chi_old[1];
                        tmpvolfield2.Square();
                        tmpvolfield += tmpvolfield2;
                        T deltasquared = delta_amplification * T(0.5) * tmpvolfield.Summation() / bsquared.Summation(); //deltasquared = const*0.5*sum( sum( (b*gradient_chi_old[0])**2 + (b*gradient_chi_old[1])**2 ) ) / ( sum( sum(bsquared) ) ) # eq. 2.23

                        tmpvolfield = bsquared_old * *gradient_chi_old[0];
                        tmpvolfield.Gradient(gradient_greg_tmp);
                        tmpvolfield = *gradient_greg_tmp[0];
                        tmpvolfield2 = bsquared_old * *gradient_chi_old[1];
                        tmpvolfield2.Gradient(gradient_greg_tmp);
                        tmpvolfield2 = *gradient_greg_tmp[1];
                        volField<T> g_reg = tmpvolfield + tmpvolfield2; //g_reg = gradient(bsquared_old * gradient_chi_old[0], dz, dx)[0] + gradient(bsquared_old*gradient_chi_old[1], dz, dx)[1]  # eq. 2.24

                        tmp.Zero();
                        ein.einsum_K_res(Kappa, r, tmp);  //tmp = einsum('ijkl,ijk->l', conj(K), r)
                        g = eta * Freg_old * tmp.GetRealPart() + Fdata_old * g_reg; //g = real(eta * (einsum('ijkl,ijk', conj(K), r)) * Freg_old + (Fdata_old * g_reg).reshape(nx*nz))  # eq. 2.25

                        gamma = g.InnerProduct(g-g_old) / g_old.InnerProduct(g_old); //gamma = dot(g, g - g_old) / (norm(g_old) ** 2)  # eq. 2.14, polak-ribiere direction

                        zeta = g + gamma*zeta; //note that this zeta is actually the zeta of the last iteration, replace with zeta_old for comprehensive code?

                        std::array<T,2> A = {0.0, 0.0};
                        for (int i = 0; i < n_total; i++)
                        {
                            K_zeta[i] = Summation( *Kappa[i], zeta );
                            A[1] += eta * std::real( conj(K_zeta[i]) * K_zeta[i] );
                            A[0] += T(-2.0) * eta * std::real( conj(r[i]) * K_zeta[i] );
                        }

                        MPI_Allreduce(MPI_IN_PLACE, &A[0], A.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

                        T A0 = Fdata_old;

                        zeta.Gradient(gradient_zeta_tmp);
                        tmpvolfield = b * *gradient_zeta_tmp[0];
                        tmpvolfield2 = b * *gradient_zeta_tmp[1];
                        tmpvolfield.Square();
                        tmpvolfield2.Square();
                        T B2 = (tmpvolfield.Summation() + tmpvolfield2.Summation()) * this->m_grid.GetCellVolume();

                        tmpvolfield = (b * *gradient_zeta_tmp[0]) * (b * *gradient_chi_old[0]);
                        tmpvolfield2 = (b * *gradient_zeta_tmp[1]) * (b * *gradient_chi_old[1]);
                        T B1 = T(2.0) * (tmpvolfield.Summation() + tmpvolfield2.Summation()) * this->m_grid.GetCellVolume();

                        tmpvolfield = (b * *gradient_chi_old[0]) * (b * *gradient_chi_old[0]);
                        tmpvolfield2 = (b * *gradient_chi_old[1]) * (b * *gradient_chi_old[1]);
                        T B0 = ((tmpvolfield.Summation() + tmpvolfield2.Summation()) + deltasquared_old * bsquared.Summation()) * this->m_grid.GetCellVolume();

                        T der_a = T(4.0) * A[1] * B2;
                        T der_b = T(3.0) * (A[1] * B1 + A[0] * B2);
                        T der_c = T(2.0) * (A[1] * B0 + A[0] * B1 + A0 * B2);
                        T der_d = A[0] * B0 + A0 * B1;

                        alpha = this->findRealRootFromCubic(der_a, der_b, der_c, der_d);

                        chi_est += alpha*zeta;

                        for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                            r[i] = p_data[i] - Summation(*Kappa[i], chi_est);

                        res = eta*normSq(r,n_total);

                        if ( sizeof(res) == sizeof(double) )
                            MPI_Allreduce(MPI_IN_PLACE, &res, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
                        else
                            MPI_Allreduce(MPI_IN_PLACE, &res, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

                        if (rank==0)
                            std::cout << it1+1 << "/" << n_iter1 << "\t (" << it+1 << "/" << n_max << ")\t res: " << std::setprecision(17) << res << std::endl;

                        Fdata_old = res;
                        res_first_it.push_back(res);

                        //breakout check
                        if ( (it1 > 0) && ( (res < T(tol1)) || ( std::abs(res_first_it[it1-1] - res) < T(tol1) ) ) )
                            break;

                        chi_est.Gradient(gradient_chi_old);
                        volField<T> gradient_chi_normsquared(this->m_grid);
                        gradient_chi_normsquared = (*gradient_chi_old[0] * *gradient_chi_old[0]) + (*gradient_chi_old[1] * *gradient_chi_old[1]);

                        tmpvolfield = (gradient_chi_normsquared + deltasquared_old) / (gradient_chi_old_normsquared + deltasquared_old);
                        Freg_old = (T(1.0) / (this->m_grid.GetDomainArea()) ) * tmpvolfield.Summation() * this->m_grid.GetCellVolume();

                        deltasquared_old = deltasquared;
                        g_old = g;
                        bsquared_old = bsquared;
                        // zeta_old?
                    }

                } // end regularisation loop



            }


            std::string name = "createTotalField" + std::to_string(rank);

            this->m_profiler.StartRegion(name);
            //calculate p_data
            for (int i=0; i<this->m_nfreq; i++)
            {
                l_i = i*this->m_nsrc;
                if (rank==0)
                {
                    std::cout << "  " << std::endl;
                    std::cout << "Creating this->p_tot for " << i+1 << "/ " << this->m_nfreq << "freq" << std::endl;
                    std::cout << "  " << std::endl;
                }


                for (int j=0; j<this->m_nsrc;j++)
                    *p_est[l_i + j] = calcField<T,volComplexField,volField,Greens>(*this->m_greens[i], chi_est, *this->p_0[i][j], rank);
            }
            this->m_profiler.EndRegion();
        }

        //free memory
        for (int i = 0; i < n_total; i++)
            delete Kappa[i];
        delete[] Kappa;
        Kappa = nullptr;

        for (int i = 0; i < this->m_nfreq*this->m_nsrc; i++)
            delete p_est[i];
        delete[] p_est;
        p_est = nullptr;


        for (int i = 0; i < 2; i++)
        {
            delete gradient_chi_old[i];
            delete gradient_greg_tmp[i];
            delete gradient_zeta_tmp[i];
        }
        delete[] gradient_chi_old;
        delete[] gradient_greg_tmp;
        delete[] gradient_zeta_tmp;


        delete[] r;
        delete[] K_zeta;



        volField_rect_2D_cpu<T> result(this->m_grid);
        chi_est.CopyTo(result);
        return result;




    }

};

#endif // INVERSION_CPU
