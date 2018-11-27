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
#include "inversion.h"
#include "calcField.h"
#include "einsum.h"
#include <array>
#include <string>
#include "variable_structure.h"
#include "forwardModelInterface.h"
#include "forwardModel.h"


using std::cout;
using std::endl;


class InversionConcrete_cpu : public Inversion
{
private:
    ForwardModelInterface *forwardModel_;

public:
    InversionConcrete_cpu(const InversionConcrete_cpu&) = delete;
    InversionConcrete_cpu& operator=(const InversionConcrete_cpu&) = delete;

    InversionConcrete_cpu(ForwardModelInterface *forwardModel)
    {
        forwardModel_ = forwardModel;
    }

    ~InversionConcrete_cpu()
    {
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

    virtual volField_rect_2D_cpu Reconstruct(const std::complex<double> *const p_data, Iter1 iter1,
                                             ConjGrad conjGrad, DeltaAmplification deltaAmplification, int n_max, bool do_reg)
    {
        double const1 = normSq(p_data,forwardModel_->m_nfreq*forwardModel_->m_nrecv*forwardModel_->m_nsrc);
        double eta = 1.0/const1;
        double gamma, alpha, res;

        std::array<double,2> alpha_div;

        volComplexField_rect_2D_cpu **Kappa, **p_est;
        int l_i;
        const int n_total = forwardModel_->m_nfreq*forwardModel_->m_nrecv*forwardModel_->m_nsrc;

        std::complex<double> *r = new std::complex<double>[n_total];
        std::complex<double> *K_zeta = new std::complex<double>[n_total];

        einsum ein(forwardModel_->m_grid,forwardModel_->m_src,forwardModel_->m_recv,forwardModel_->m_freq);

        volField_rect_2D_cpu chi_est(forwardModel_->m_grid), g(forwardModel_->m_grid), g_old(forwardModel_->m_grid), zeta(forwardModel_->m_grid);
        volComplexField_rect_2D_cpu tmp(forwardModel_->m_grid);
        chi_est.Zero();

        Kappa = new volComplexField_rect_2D_cpu*[n_total];
        for (int i = 0; i < n_total; i++)
        {
            Kappa[i] = new volComplexField_rect_2D_cpu(forwardModel_->m_grid);
        }
        p_est = new volComplexField_rect_2D_cpu*[forwardModel_->m_nfreq*forwardModel_->m_nsrc];
        for (int i=0; i<forwardModel_->m_nfreq; i++)
        {
            l_i = i*forwardModel_->m_nsrc;
            for (int j=0; j<forwardModel_->m_nsrc;j++)
                p_est[l_i + j] = new volComplexField_rect_2D_cpu(*forwardModel_->p_0[i][j]);
        }

        volField_rect_2D_cpu **gradient_chi_old = new volField_rect_2D_cpu*[2];
        volField_rect_2D_cpu **gradient_greg_tmp = new volField_rect_2D_cpu*[2];
        volField_rect_2D_cpu **gradient_zeta_tmp = new volField_rect_2D_cpu*[2];

        for (int i = 0; i < 2; i++)
        {
            gradient_chi_old[i] = new volField_rect_2D_cpu(forwardModel_->m_grid);
            gradient_greg_tmp[i] = new volField_rect_2D_cpu(forwardModel_->m_grid);
            gradient_zeta_tmp[i] = new volField_rect_2D_cpu(forwardModel_->m_grid);
        }

        //main loop//
        for(int it=0; it<n_max; it++)
        {
            std::vector<std::complex<double>> res_first_it;
            if (do_reg == 0)
            {
                ein.einsum_Gr_Pest(Kappa, forwardModel_->m_greens, p_est);

                for (int it1=0; it1<iter1.n; it1++)
                {
                    for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                        r[i] = p_data[i] - Summation(*Kappa[i], chi_est);

                    res = eta*normSq(r,n_total);
                    {
                        if (it1 > 0)
                            std::cout << "inner loop residual = " << std::setprecision(17) << res << "     " << std::abs(res_first_it[it1-1] - res) << "    " << it1+1 << '/' << iter1.n << std::endl;
                        else
                            std::cout << "inner loop residual = " << std::setprecision(17) << res << std::endl;
                    }

                    res_first_it.push_back(res);

                    if ( (it1 > 0) && ( (res < double(iter1.tolerance)) || ( std::abs(res_first_it[it1-1] - res) < double(iter1.tolerance) ) ) )
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
                    alpha = alpha_div[0] / alpha_div[1];
                    chi_est += alpha*zeta; // Babak 11-13-2018: the step size of the parameter in Eq: ContrastUpdate in the user manual.
                    g_old = g;
                }
            }
            else if (do_reg == 1) {
                double deltasquared_old = double(0.0);
                volField_rect_2D_cpu bsquared_old(forwardModel_->m_grid);
                bsquared_old.Zero();
                double Freg_old = double(1.0);
                double Fdata_old = double(0.0);

                double delta_amplification = deltaAmplification.start / (deltaAmplification.slope * it + double(1.0));

                ein.einsum_Gr_Pest(Kappa, forwardModel_->m_greens, p_est);

                //calculate initial residual
                for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                    r[i] = p_data[i] - Summation(*Kappa[i], chi_est);

                //start the inner loop
                for (int it1=0; it1<iter1.n; it1++)
                {
                    if (it1 == 0)
                    {
                        ein.einsum_K_res(Kappa, r, tmp);
                        g = eta * tmp.GetRealPart();
                        zeta = g;
                        alpha_div[0] = double(0.0);
                        alpha_div[1] =double(0.0);
                        for (int i = 0; i < n_total; i++)
                        {
                            K_zeta[i] = Summation( *Kappa[i], zeta );
                            alpha_div[0] += std::real( conj(r[i]) * K_zeta[i] );
                            alpha_div[1] += std::real( conj(K_zeta[i]) * K_zeta[i] );
                        }
                        alpha = alpha_div[0] / alpha_div[1];
                        chi_est += alpha*zeta;
                        g_old = g;

                        for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                            r[i] = p_data[i] - Summation(*Kappa[i], chi_est);

                        res = eta*normSq(r,n_total);
                            std::cout << it1+1 << "/" << iter1.n << "\t (" << it+1 << "/" << n_max << ")\t res: " << std::setprecision(17) << res << std::endl;
                        Fdata_old = res;
                        res_first_it.push_back(res);
                    }
                    else
                    {
                        chi_est.Gradient(gradient_chi_old);
                        volField_rect_2D_cpu gradient_chi_old_normsquared(forwardModel_->m_grid);
                        gradient_chi_old_normsquared = (*gradient_chi_old[0] * *gradient_chi_old[0]) + (*gradient_chi_old[1] * *gradient_chi_old[1]);

                        volField_rect_2D_cpu bsquared = ( gradient_chi_old_normsquared + deltasquared_old );
                        bsquared.Reciprocal();
                        bsquared *= (double(1.0) / (forwardModel_->m_grid.GetDomainArea()) ); //bsquared = (1.0 / domain_area) * (1.0 / (gradient_chi_old_normsquared + deltasquared_old * ones((nz, nx))))  # eq. 2.22
                        volField_rect_2D_cpu b = bsquared;
                        b.Sqrt();

                        volField_rect_2D_cpu tmpvolfield = b * *gradient_chi_old[0];
                        tmpvolfield.Square();
                        volField_rect_2D_cpu tmpvolfield2 = b * *gradient_chi_old[1];
                        tmpvolfield2.Square();
                        tmpvolfield += tmpvolfield2;
                        double deltasquared = delta_amplification * double(0.5) * tmpvolfield.Summation() / bsquared.Summation(); //deltasquared = const*0.5*sum( sum( (b*gradient_chi_old[0])**2 + (b*gradient_chi_old[1])**2 ) ) / ( sum( sum(bsquared) ) ) # eq. 2.23

                        tmpvolfield = bsquared_old * *gradient_chi_old[0];
                        tmpvolfield.Gradient(gradient_greg_tmp);
                        tmpvolfield = *gradient_greg_tmp[0];
                        tmpvolfield2 = bsquared_old * *gradient_chi_old[1];
                        tmpvolfield2.Gradient(gradient_greg_tmp);
                        tmpvolfield2 = *gradient_greg_tmp[1];
                        volField_rect_2D_cpu g_reg = tmpvolfield + tmpvolfield2; //g_reg = gradient(bsquared_old * gradient_chi_old[0], dz, dx)[0] + gradient(bsquared_old*gradient_chi_old[1], dz, dx)[1]  # eq. 2.24
                        tmp.Zero();
                        ein.einsum_K_res(Kappa, r, tmp);  //tmp = einsum('ijkl,ijk->l', conj(K), r)
                        g = eta * Freg_old * tmp.GetRealPart() + Fdata_old * g_reg; //g = real(eta * (einsum('ijkl,ijk', conj(K), r)) * Freg_old + (Fdata_old * g_reg).reshape(nx*nz))  # eq. 2.25

                        gamma = g.InnerProduct(g-g_old) / g_old.InnerProduct(g_old); //gamma = dot(g, g - g_old) / (norm(g_old) ** 2)  # eq. 2.14, polak-ribiere direction

                        zeta = g + gamma*zeta; //note that this zeta is actually the zeta of the last iteration, replace with zeta_old for comprehensive code?

                        std::array<double,2> A = {0.0, 0.0};
                        for (int i = 0; i < n_total; i++)
                        {
                            K_zeta[i] = Summation( *Kappa[i], zeta );
                            A[1] += eta * std::real( conj(K_zeta[i]) * K_zeta[i] );
                            A[0] += double(-2.0) * eta * std::real( conj(r[i]) * K_zeta[i] );
                        }

                        double A0 = Fdata_old;
                        zeta.Gradient(gradient_zeta_tmp);
                        tmpvolfield = b * *gradient_zeta_tmp[0];
                        tmpvolfield2 = b * *gradient_zeta_tmp[1];
                        tmpvolfield.Square();
                        tmpvolfield2.Square();
                        double B2 = (tmpvolfield.Summation() + tmpvolfield2.Summation()) * forwardModel_->m_grid.GetCellVolume();

                        tmpvolfield = (b * *gradient_zeta_tmp[0]) * (b * *gradient_chi_old[0]);
                        tmpvolfield2 = (b * *gradient_zeta_tmp[1]) * (b * *gradient_chi_old[1]);
                        double B1 = double(2.0) * (tmpvolfield.Summation() + tmpvolfield2.Summation()) * forwardModel_->m_grid.GetCellVolume();

                        tmpvolfield = (b * *gradient_chi_old[0]) * (b * *gradient_chi_old[0]);
                        tmpvolfield2 = (b * *gradient_chi_old[1]) * (b * *gradient_chi_old[1]);
                        double B0 = ((tmpvolfield.Summation() + tmpvolfield2.Summation()) + deltasquared_old * bsquared.Summation()) * forwardModel_->m_grid.GetCellVolume();

                        double der_a = double(4.0) * A[1] * B2;
                        double der_b = double(3.0) * (A[1] * B1 + A[0] * B2);
                        double der_c = double(2.0) * (A[1] * B0 + A[0] * B1 + A0 * B2);
                        double der_d = A[0] * B0 + A0 * B1;

                        alpha = this->findRealRootFromCubic(der_a, der_b, der_c, der_d);

                        chi_est += alpha*zeta;

                        for (int i = 0; i < n_total; i++)  //r = p_data - einsum('ijkl,l->ijk', K, chi_est)
                            r[i] = p_data[i] - Summation(*Kappa[i], chi_est);
                        res = eta*normSq(r,n_total);
                            std::cout << it1+1 << "/" << iter1.n << "\t (" << it+1 << "/" << n_max << ")\t res: " << std::setprecision(17) << res << std::endl;

                        Fdata_old = res;
                        res_first_it.push_back(res);

                        //breakout check
                        if ( (it1 > 0) && ( (res < double(iter1.tolerance)) || ( std::abs(res_first_it[it1-1] - res) < double(iter1.tolerance) ) ) )
                            break;

                        chi_est.Gradient(gradient_chi_old);
                        volField_rect_2D_cpu gradient_chi_normsquared(forwardModel_->m_grid);
                        gradient_chi_normsquared = (*gradient_chi_old[0] * *gradient_chi_old[0]) + (*gradient_chi_old[1] * *gradient_chi_old[1]);

                        tmpvolfield = (gradient_chi_normsquared + deltasquared_old) / (gradient_chi_old_normsquared + deltasquared_old);
                        Freg_old = (double(1.0) / (forwardModel_->m_grid.GetDomainArea()) ) * tmpvolfield.Summation() * forwardModel_->m_grid.GetCellVolume();

                        deltasquared_old = deltasquared;
                        g_old = g;
                        bsquared_old = bsquared;
                        // zeta_old?
                    }

                } // end regularisation loop



            }

            std::string name = "createTotalFieldCurrentProcessor";

            forwardModel_->m_profiler.StartRegion(name);
            //calculate p_data
            for (int i=0; i<forwardModel_->m_nfreq; i++)
            {
                l_i = i*forwardModel_->m_nsrc;

                    std::cout << "  " << std::endl;
                    std::cout << "Creating this->p_tot for " << i+1 << "/ " << forwardModel_->m_nfreq << "freq" << std::endl;
                    std::cout << "  " << std::endl;


                for (int j=0; j<forwardModel_->m_nsrc;j++)
                    *p_est[l_i + j] = calcField(*forwardModel_->m_greens[i], chi_est, *forwardModel_->p_0[i][j], conjGrad);
            }
            forwardModel_->m_profiler.EndRegion();
        }
        //free memory
        for (int i = 0; i < n_total; i++)
            delete Kappa[i];
        delete[] Kappa;
        Kappa = nullptr;

        for (int i = 0; i < forwardModel_->m_nfreq*forwardModel_->m_nsrc; i++)
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



        volField_rect_2D_cpu result(forwardModel_->m_grid);
        chi_est.CopyTo(result);
        return result;




    }

};

#endif // INVERSION_CPU

