#include "forwardModel.h"


    ForwardModel::ForwardModel(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv, const Frequencies_group &freq, ProfileInterface &profiler, const volField_rect_2D_cpu chi)
    : ForwardModelInterface (grid, src, recv, freq, profiler, chi)
    {
        std::cout << "Creating Greens function field..." << std::endl;
        this->createGreens();
        this->SetBackground(chi);
        std::cout << "Creating P0..." << std::endl;
        this->createP0();
    }

    ForwardModel::~ForwardModel()
    {
        if (this->m_greens!=nullptr)
            this->deleteGreens();

        if (this->p_0!=nullptr)
            this->deleteP0();

        if (this->p_tot!=nullptr)
            this->deleteTotalField();
    }

    void ForwardModel::createGreens()
    {
        m_greens = new Greens_rect_2D_cpu*[m_nfreq];

        for (int i=0; i<m_nfreq; i++)
        {
            m_greens[i] = new Greens_rect_2D_cpu(m_grid, Helmholtz2D, m_src, m_recv, m_freq.k[i]);
        }
    }

    void ForwardModel::deleteGreens()
    {
        for (int i=0; i<m_nfreq; i++)
            delete m_greens[i];
        delete[] m_greens;
        m_greens = nullptr;
    }

    void ForwardModel::SetBackground(const volField_rect_2D_cpu &chi_)
    {
        assert(&m_chi.GetGrid() == &chi_.GetGrid());
        m_chi = chi_;
    }

    void ForwardModel::createP0()
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

    void ForwardModel::deleteP0()
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

    void ForwardModel::deleteTotalField()
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

    void ForwardModel::calculateData(std::complex<double> *p_data)
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

    void ForwardModel::createTotalField(ConjGrad conjGrad)
    {
        assert(this->m_greens != nullptr);
        assert(this->p_0 != nullptr);
        assert(this->p_tot == nullptr);

        std::string name = "createTotalFieldCurrentProcessor";

        this->p_tot = new volComplexField_rect_2D_cpu**[this->m_nfreq];
        this->m_profiler.StartRegion(name);
        for (int i=0; i<this->m_nfreq; i++)
        {
            this->p_tot[i] = new volComplexField_rect_2D_cpu*[this->m_nsrc];

                std::cout << "  " << std::endl;
                std::cout << "Creating this->p_tot for " << i+1 << "/ " << this->m_nfreq << "freq" << std::endl;
                std::cout << "  " << std::endl;


            for (int j=0; j<this->m_nsrc; j++)
            {
                this->p_tot[i][j] = new volComplexField_rect_2D_cpu(this->m_grid);
                *this->p_tot[i][j] = calcField(*this->m_greens[i], this->m_chi, *this->p_0[i][j], conjGrad);
            }

                std::cout << "  " << std::endl;
                std::cout << "  " << std::endl;
        }
        this->m_profiler.EndRegion();


    }




