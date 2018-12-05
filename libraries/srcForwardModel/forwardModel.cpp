#include "forwardModel.h"


    ForwardModel::ForwardModel(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq,
                               ProfileInterface &profiler, Input input)
    : ForwardModelInterface (grid, src, recv, freq, profiler, input),
      m_greens(), p_0(), p_tot(), m_profiler(profiler)
    {
        residual = new std::complex<double>[input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src];
        K_zeta = new std::complex<double>[input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src];

        std::cout << "Creating Greens function field..." << std::endl;
        this->createGreens();
        std::cout << "Creating P0..." << std::endl;
        this->createP0();

        // initialize kappa
        Kappa = new volComplexField_rect_2D_cpu*[input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src];
        for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src; i++)
        {
            Kappa[i] = new volComplexField_rect_2D_cpu(m_grid);
        }

        // initialize p_tot1D
        p_tot1D = new volComplexField_rect_2D_cpu*[input.freq.nTotal*input.nSourcesReceivers.src];
        int l_i;
        for (int i=0; i<input.freq.nTotal; i++)
        {
            l_i = i*input.nSourcesReceivers.src;
            for (int j=0; j<input.nSourcesReceivers.src;j++)
                p_tot1D[l_i + j] = new volComplexField_rect_2D_cpu(*p_0[i][j]); // p_tot1D is initialized to p_0 (section 3.3.3 of the report)
        }
    }

    ForwardModel::~ForwardModel()
    {
        if (this->m_greens!=nullptr)
            this->deleteGreens();

        if (this->p_0!=nullptr)
            this->deleteP0();

        if (this->p_tot!=nullptr)
            this->deleteTotalField();

        for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src; i++)
            delete Kappa[i];
        delete[] Kappa;
        Kappa = nullptr;

        for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.src; i++)
            delete p_tot1D[i];
        delete[] p_tot1D;
        p_tot1D = nullptr;
    }

    void ForwardModel::createGreens()
    {
        m_greens = new Greens_rect_2D_cpu*[input.freq.nTotal];

        for (int i=0; i<input.freq.nTotal; i++)
        {
            m_greens[i] = new Greens_rect_2D_cpu(m_grid, Helmholtz2D, m_src, m_recv, m_freq.k[i]);
        }
    }

    void ForwardModel::deleteGreens()
    {
        for (int i=0; i<input.freq.nTotal; i++)
            delete m_greens[i];
        delete[] m_greens;
        m_greens = nullptr;
    }

    void ForwardModel::createP0()
    {
        assert(m_greens != nullptr);
        assert(p_0 == nullptr);

        p_0 = new volComplexField_rect_2D_cpu**[input.freq.nTotal];

         for (int i=0; i<input.freq.nTotal; i++)
        {
            p_0[i] = new volComplexField_rect_2D_cpu*[input.nSourcesReceivers.src];

            for (int j=0; j<input.nSourcesReceivers.src; j++)
            {
                p_0[i][j] = new volComplexField_rect_2D_cpu(m_grid);
                *p_0[i][j] = *( m_greens[i]->GetReceiverCont(j) ) / (m_freq.k[i] * m_freq.k[i] * m_grid.GetCellVolume());
            }
        }

    }

    void ForwardModel::deleteP0()
    {
        for (int i=0; i<input.freq.nTotal; i++)
        {
            for (int j=0; j<input.nSourcesReceivers.src; j++)
                delete p_0[i][j];

            delete[] p_0[i];
        }
        delete[] p_0;
        p_0 = nullptr;
    }

    void ForwardModel::deleteTotalField()
    {
        for (int i=0; i<input.freq.nTotal; i++)
        {
            for (int j=0; j<input.nSourcesReceivers.src; j++)
                delete p_tot[i][j];

            delete[] p_tot[i];
        }
        delete[] p_tot;
        p_tot = nullptr;
    }

    void ForwardModel::calculateData(std::complex<double> *p_data, volField_rect_2D_cpu chi, ConjGrad conjGrad )
    {
        this->createTotalField(conjGrad, chi);
        int l_i, l_j;
        for (int i=0; i<input.freq.nTotal; i++)
        {
            l_i = i*input.nSourcesReceivers.rec*input.nSourcesReceivers.src;
            for (int j=0; j<input.nSourcesReceivers.rec; j++)
            {
                l_j = j*input.nSourcesReceivers.src;
                for (int k=0; k<input.nSourcesReceivers.src; k++)
                {
                    p_data[l_i + l_j + k] = Summation( *( m_greens[i]->GetReceiverCont(j) ) , *p_tot[i][k]*chi );
                }
            }
        }
    }

    void ForwardModel::createTotalField(ConjGrad conjGrad, volField_rect_2D_cpu chi)
    {
        assert(this->m_greens != nullptr);
        assert(this->p_0 != nullptr);
        assert(this->p_tot == nullptr);

        std::string name = "createTotalFieldCurrentProcessor";

        this->p_tot = new volComplexField_rect_2D_cpu**[input.freq.nTotal];
        this->m_profiler.StartRegion(name);
        for (int i=0; i<this->input.freq.nTotal; i++)
        {
            this->p_tot[i] = new volComplexField_rect_2D_cpu*[input.nSourcesReceivers.src];

                std::cout << "  " << std::endl;
                std::cout << "Creating this->p_tot for " << i+1 << "/ " << input.freq.nTotal << "freq" << std::endl;
                std::cout << "  " << std::endl;


            for (int j=0; j<this->input.nSourcesReceivers.src; j++)
            {
                this->p_tot[i][j] = new volComplexField_rect_2D_cpu(this->m_grid);
                *this->p_tot[i][j] = calcField(*this->m_greens[i], chi, *this->p_0[i][j], conjGrad);
            }

                std::cout << "  " << std::endl;
                std::cout << "  " << std::endl;
        }
        this->m_profiler.EndRegion();

    }

    void ForwardModel::createTotalField1D(ConjGrad conjGrad, volField_rect_2D_cpu chi_est)
    {
        int l_i;
        for (int i=0; i<input.freq.nTotal; i++)
        {
            l_i = i*input.nSourcesReceivers.src;

                std::cout << "  " << std::endl;
                std::cout << "Creating this->p_tot for " << i+1 << "/ " << input.freq.nTotal << "freq" << std::endl;
                std::cout << "  " << std::endl;


            for (int j=0; j<input.nSourcesReceivers.src;j++)
                *p_tot1D[l_i + j] = calcField(*m_greens[i], chi_est, *p_0[i][j], conjGrad);
        }
    }

    ProfileInterface& ForwardModel::get_m_profiler()
    {
        return m_profiler;
    }

    Input ForwardModel::getInput()
    {
        return input;
    }

    Greens_rect_2D_cpu** ForwardModel::get_m_greens()
    {
        return m_greens;
    }

    volComplexField_rect_2D_cpu** ForwardModel::getKappa()
    {
        return Kappa;
    }

    void ForwardModel::calculateKappa()
    {
        int l_i, l_j;

        for (int i = 0; i < input.freq.nTotal; i++)
        {
            l_i = i*input.nSourcesReceivers.rec*input.nSourcesReceivers.src;
            for (int j = 0; j < input.nSourcesReceivers.rec; j++)
            {
                l_j = j*input.nSourcesReceivers.src;
                for(int k = 0; k < input.nSourcesReceivers.src; k++)
                {
                    *Kappa[l_i + l_j + k] = ( *m_greens[i]->GetReceiverCont(j) ) * (*p_tot1D[i*input.nSourcesReceivers.src + k]);
                }
            }
        }
    }

    void ForwardModel::calculateResidual(volField_rect_2D_cpu chi_est, const std::complex<double> *const p_data)
    {
        for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src; i++)
            residual[i] = p_data[i] - Summation(*Kappa[i], chi_est);
    }

    std::complex<double>* ForwardModel::getResidual()
    {
        return residual;
    }

    double ForwardModel::calculateResidualNormSq(double eta)
    {
        return eta*normSq(residual,input.nSourcesReceivers.rec*input.nSourcesReceivers.src*input.freq.nTotal);
    }

    void ForwardModel::calculateK_zeta(volField_rect_2D_cpu zeta)
    {
        for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src; i++)
        {
            K_zeta[i] = Summation( *Kappa[i], zeta);
        }
    }

    std::complex<double>* ForwardModel::get_K_zeta()
    {
        return K_zeta;
    }
