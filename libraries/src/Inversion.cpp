#include "Inversion.h"

double Inversion::findRealRootFromCubic(double a, double b, double c, double d)
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
}


volField_rect_2D_cpu Inversion::Reconstruct(const std::complex<double> *const p_data, Input input)
{
    double const1 = normSq(p_data,forwardModel_->getInput().freq.nTotal*forwardModel_->getInput().nSourcesReceivers.rec*forwardModel_->getInput().nSourcesReceivers.src); // used for eta
    double eta = 1.0/const1;
    double gamma, alpha, res;

    std::array<double,2> alpha_div;

    const int n_total = forwardModel_->getInput().freq.nTotal*forwardModel_->getInput().nSourcesReceivers.rec*forwardModel_->getInput().nSourcesReceivers.src;

    volField_rect_2D_cpu chi_est(forwardModel_->get_m_grid()), g(forwardModel_->get_m_grid()), g_old(forwardModel_->get_m_grid()), zeta(forwardModel_->get_m_grid()); // stays here
    volComplexField_rect_2D_cpu tmp(forwardModel_->get_m_grid()); // eq: integrandForDiscreteK, tmp is the argument of Re()
    chi_est.Zero();


    volField_rect_2D_cpu **gradient_chi_old = new volField_rect_2D_cpu*[2];
    volField_rect_2D_cpu **gradient_greg_tmp = new volField_rect_2D_cpu*[2];
    volField_rect_2D_cpu **gradient_zeta_tmp = new volField_rect_2D_cpu*[2];

    for (int i = 0; i < 2; i++)
    {
        gradient_chi_old[i] = new volField_rect_2D_cpu(forwardModel_->get_m_grid());
        gradient_greg_tmp[i] = new volField_rect_2D_cpu(forwardModel_->get_m_grid());
        gradient_zeta_tmp[i] = new volField_rect_2D_cpu(forwardModel_->get_m_grid());
    }

    //main loop//
    for(int it=0; it<input.n_max; it++)
    {
        std::vector<std::complex<double>> res_first_it;
        if (input.do_reg == 0)
        {
            forwardModel_->calculateKappa();
            for (int it1=0; it1<input.iter1.n; it1++)
            {
                forwardModel_->calculateResidual(chi_est, p_data);
                res = forwardModel_->calculateResidualNormSq(eta);
                {
                    if (it1 > 0)
                        std::cout << "inner loop residual = " << std::setprecision(17) << res << "     " <<
                                     std::abs(res_first_it[it1-1] - res) << "    " << it1+1 << '/' << input.iter1.n << std::endl;
                    else
                        std::cout << "inner loop residual = " << std::setprecision(17) << res << std::endl;
                }

                res_first_it.push_back(res);
                if ( (it1 > 0) && ( (res < double(input.iter1.tolerance)) || ( std::abs(res_first_it[it1-1] - res) < double(input.iter1.tolerance) ) ) )
                    break;

                calculate_K_res(forwardModel_, tmp);
                g = (2*eta) * tmp.GetRealPart();

                if (it1==0)
                    zeta = g;
                else
                {
                    gamma = g.InnerProduct(g-g_old) / g_old.InnerProduct(g_old);
                    zeta = g + gamma*zeta;
                }
                alpha_div[0] = 0.0;
                alpha_div[1] = 0.0;
                forwardModel_->calculateK_zeta(zeta);
                for (int i = 0; i < n_total; i++)
                {
                    alpha_div[0] += std::real( conj(forwardModel_->getResidual()[i]) * forwardModel_->get_K_zeta()[i] );
                    alpha_div[1] += std::real( conj(forwardModel_->get_K_zeta()[i]) * forwardModel_->get_K_zeta()[i] );
                }
                alpha = alpha_div[0] / alpha_div[1];
                chi_est += alpha*zeta; // the step size of the parameter in Eq: ContrastUpdate in the user manual.
                g_old = g;
            }
        }
        else if (input.do_reg == 1)
        {
            double deltasquared_old = double(0.0);
            volField_rect_2D_cpu bsquared_old(forwardModel_->get_m_grid());
            bsquared_old.Zero();
            double Freg_old = double(1.0);
            double Fdata_old = double(0.0);
            double delta_amplification = input.deltaAmplification.start / (input.deltaAmplification.slope * it + double(1.0));

            forwardModel_->calculateKappa();
            forwardModel_->calculateResidual(chi_est, p_data);

            //start the inner loop
            for (int it1=0; it1<input.iter1.n; it1++)
            {
                if (it1 == 0)
                {
                    calculate_K_res(forwardModel_, tmp);
                    g = eta * tmp.GetRealPart(); //eq: gradientRunc
                    zeta = g;
                    alpha_div[0] = double(0.0);
                    alpha_div[1] =double(0.0);
                    forwardModel_->calculateK_zeta(zeta);

                    for (int i = 0; i < n_total; i++)
                    {
                        alpha_div[0] += std::real( conj(forwardModel_->getResidual()[i]) * forwardModel_->get_K_zeta()[i] );
                        alpha_div[1] += std::real( conj(forwardModel_->get_K_zeta()[i]) * forwardModel_->get_K_zeta()[i] );
                    }
                    alpha = alpha_div[0] / alpha_div[1]; //eq:optimalStepSizeCG in the readme pdf
                    chi_est += alpha*zeta;
                    g_old = g;

                    forwardModel_->calculateResidual(chi_est, p_data);
                    res = forwardModel_->calculateResidualNormSq(eta);
                    std::cout << it1+1 << "/" << input.iter1.n << "\t (" << it+1 << "/" << input.n_max << ")\t res: " << std::setprecision(17) << res << std::endl;
                    Fdata_old = res;
                    res_first_it.push_back(res);
                }
                else
                {
                    chi_est.Gradient(gradient_chi_old);
                    volField_rect_2D_cpu gradient_chi_old_normsquared(forwardModel_->get_m_grid());
                    gradient_chi_old_normsquared = (*gradient_chi_old[0] * *gradient_chi_old[0]) + (*gradient_chi_old[1] * *gradient_chi_old[1]);

                    volField_rect_2D_cpu bsquared = ( gradient_chi_old_normsquared + deltasquared_old ); // eq: errorFuncRegulWeighting
                    bsquared.Reciprocal();
                    bsquared *= (double(1.0) / (forwardModel_->get_m_grid().GetDomainArea()) ); //bsquared = (1.0 / domain_area) * (1.0 / (gradient_chi_old_normsquared + deltasquared_old * ones((nz, nx))))  # eq. 2.22
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
                    calculate_K_res(forwardModel_, tmp);
                    g = eta * Freg_old * tmp.GetRealPart() + Fdata_old * g_reg; //g = real(eta * (einsum('ijkl,ijk', conj(K), r)) * Freg_old + (Fdata_old * g_reg).reshape(nx*nz))  # eq: integrandForDiscreteK

                    gamma = g.InnerProduct(g-g_old) / g_old.InnerProduct(g_old); //gamma = dot(g, g - g_old) / (norm(g_old) ** 2)  # eq: PolakRibiereDirection

                    zeta = g + gamma*zeta;

                    std::array<double,2> A = {0.0, 0.0};
                    forwardModel_->calculateK_zeta(zeta);
                    for (int i = 0; i < n_total; i++)
                    {
                        A[1] += eta * std::real( conj(forwardModel_->get_K_zeta()[i]) * forwardModel_->get_K_zeta()[i] );
                        A[0] += double(-2.0) * eta * std::real( conj(forwardModel_->getResidual()[i]) * forwardModel_->get_K_zeta()[i] );
                    }

                    double A0 = Fdata_old;
                    zeta.Gradient(gradient_zeta_tmp);
                    tmpvolfield = b * *gradient_zeta_tmp[0];
                    tmpvolfield2 = b * *gradient_zeta_tmp[1];
                    tmpvolfield.Square();
                    tmpvolfield2.Square();
                    double B2 = (tmpvolfield.Summation() + tmpvolfield2.Summation()) * forwardModel_->get_m_grid().GetCellVolume();

                    tmpvolfield = (b * *gradient_zeta_tmp[0]) * (b * *gradient_chi_old[0]);
                    tmpvolfield2 = (b * *gradient_zeta_tmp[1]) * (b * *gradient_chi_old[1]);
                    double B1 = double(2.0) * (tmpvolfield.Summation() + tmpvolfield2.Summation()) * forwardModel_->get_m_grid().GetCellVolume();

                    tmpvolfield = (b * *gradient_chi_old[0]) * (b * *gradient_chi_old[0]);
                    tmpvolfield2 = (b * *gradient_chi_old[1]) * (b * *gradient_chi_old[1]);
                    double B0 = ((tmpvolfield.Summation() + tmpvolfield2.Summation()) + deltasquared_old * bsquared.Summation()) * forwardModel_->get_m_grid().GetCellVolume();

                    double der_a = double(4.0) * A[1] * B2;
                    double der_b = double(3.0) * (A[1] * B1 + A[0] * B2);
                    double der_c = double(2.0) * (A[1] * B0 + A[0] * B1 + A0 * B2);
                    double der_d = A[0] * B0 + A0 * B1;

                    alpha = this->findRealRootFromCubic(der_a, der_b, der_c, der_d);

                    chi_est += alpha*zeta;

                    forwardModel_->calculateResidual(chi_est, p_data);

                    res = forwardModel_->calculateResidualNormSq(eta);

                        std::cout << it1+1 << "/" << input.iter1.n << "\t (" << it+1 << "/" << input.n_max << ")\t res: " << std::setprecision(17) << res << std::endl;

                    Fdata_old = res;
                    res_first_it.push_back(res);

                    //breakout check
                    if ( (it1 > 0) && ( (res < double(input.iter1.tolerance)) || ( std::abs(res_first_it[it1-1] - res) < double(input.iter1.tolerance) ) ) )
                        break;

                    chi_est.Gradient(gradient_chi_old);
                    volField_rect_2D_cpu gradient_chi_normsquared(forwardModel_->get_m_grid());
                    gradient_chi_normsquared = (*gradient_chi_old[0] * *gradient_chi_old[0]) + (*gradient_chi_old[1] * *gradient_chi_old[1]);

                    tmpvolfield = (gradient_chi_normsquared + deltasquared_old) / (gradient_chi_old_normsquared + deltasquared_old);
                    Freg_old = (double(1.0) / (forwardModel_->get_m_grid().GetDomainArea()) ) * tmpvolfield.Summation() * forwardModel_->get_m_grid().GetCellVolume();

                    deltasquared_old = deltasquared;
                    g_old = g;
                    bsquared_old = bsquared;

                }

            } // end regularisation loop



        }

        std::string name = "createTotalFieldCurrentProcessor";
        forwardModel_->get_m_profiler().StartRegion(name);
        forwardModel_->createTotalField1D(input.conjGrad, chi_est); // estimate p_tot from the newly estimated chi (chi_est)
        forwardModel_->get_m_profiler().EndRegion();


    }

    for (int i = 0; i < 2; i++)
    {
        delete gradient_chi_old[i];
        delete gradient_greg_tmp[i];
        delete gradient_zeta_tmp[i];
    }
    delete[] gradient_chi_old;
    delete[] gradient_greg_tmp;
    delete[] gradient_zeta_tmp;

    volField_rect_2D_cpu result(forwardModel_->get_m_grid());
    chi_est.CopyTo(result);
    return result;




}
