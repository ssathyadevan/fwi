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
    virtual void createTotalField(int maxIt, T tol) = 0;
    virtual void deleteTotalField() = 0;
    virtual void calculateData(std::complex<T> *p_data) = 0;
    virtual volField_rect_2D_cpu<T> Reconstruct(const std::complex<T> *p_data, int maxIt, T tol) = 0;

};

template <typename T, template<typename> class volField, template<typename> class volComplexField, template<typename> class Greens>
class InversionConcrete : public Inversion<T> {

    const grid_rect_2D<T> &grid;
    const Sources_rect_2D<T> &src;
    const Receivers_rect_2D<T> &recv;
    const Frequencies<T> &freq;

    Greens<T> **greens;
    volComplexField<T> ***p_0;
    volComplexField<T> ***p_tot;

    volField<T> chi;

    ProfileInterface &profiler;

    InversionConcrete(const InversionConcrete<T, volField, volComplexField, Greens>&) = delete;
    InversionConcrete<T, volField, volComplexField, Greens>& operator=(const InversionConcrete<T, volField, volComplexField, Greens>&) = delete;

public:

    InversionConcrete(const grid_rect_2D<T> &grid_, const Sources_rect_2D<T> &src_, const Receivers_rect_2D<T> &recv_, const Frequencies<T> &freq_, ProfileInterface &profiler_) :
        grid(grid_), src(src_), recv(recv_), freq(freq_), greens(), p_0(), p_tot(), chi(grid), profiler(profiler_) {}

    ~InversionConcrete() {
        if(greens != nullptr) deleteGreens();
        if(p_0 != nullptr) deleteP0();
        if(p_tot != nullptr) deleteTotalField();
    }

    void createGreens() {

        assert(greens == nullptr);

        greens = new Greens<T>*[freq.nFreq];
        for(int iFreq=0; iFreq<freq.nFreq; iFreq++) {
            greens[iFreq] = new Greens<T>(grid, Helmholtz2D<T>, src, recv, freq.k[iFreq]);
        }

    }

    void deleteGreens() {

        assert(greens != nullptr);

        for(int iFreq=0; iFreq<freq.nFreq; iFreq++) {
            delete greens[iFreq];
        }
        delete[] greens;
        greens = nullptr;

    }

    virtual void SetBackground(const volField_rect_2D_cpu<T> &chi_) {
        assert(&chi.GetGrid() == &chi_.GetGrid());
        chi = chi_;
    }

    void createP0() {

        assert(greens != nullptr);
        assert(p_0 == nullptr);

        p_0 = new volComplexField<T>**[freq.nFreq];

        for(int iFreq=0; iFreq<freq.nFreq; iFreq++) {

            p_0[iFreq] = new volComplexField<T>*[src.nSrc];
            for(int iSrc=0; iSrc<src.nSrc; iSrc++) {
                // TODO: This is only correct if src == recv
                p_0[iFreq][iSrc] = new volComplexField<T>(grid);
                *p_0[iFreq][iSrc] = (*greens[iFreq]->GetReceiverCont(iSrc)) / grid.GetCellVolume();   //// change here - P0 multiplied by K**2////

            }

        }
    }

    void deleteP0() {

        assert(p_0 != nullptr);

        for(int iFreq=0; iFreq<freq.nFreq; iFreq++) {
            for(int iSrc=0; iSrc<src.nSrc; iSrc++) {
                delete p_0[iFreq][iSrc];
            }
            delete[] p_0[iFreq];
        }
        delete[] p_0;
        p_0 = nullptr;
    }

    void createTotalField(int maxIt, T tol) {

        assert(greens != nullptr);
        assert(p_0 != nullptr);
        assert(p_tot == nullptr);

        profiler.StartRegion("createTotalField");

        p_tot = new volComplexField<T>**[freq.nFreq];

        for(int iFreq=0; iFreq<freq.nFreq; iFreq++) {

            std::cout << "Frequency " << iFreq << " of " << freq.nFreq << std::endl;

            p_tot[iFreq] = new volComplexField<T>*[src.nSrc];
            for(int iSrc=0; iSrc<src.nSrc; iSrc++) {
                p_tot[iFreq][iSrc] = new volComplexField<T>(grid);
                *p_tot[iFreq][iSrc] = calcField(*greens[iFreq], chi, *p_0[iFreq][iSrc], maxIt, tol);
            }

        }

        profiler.EndRegion();

    }

    void deleteTotalField() {

        assert(p_tot != nullptr);

        for(int iFreq=0; iFreq<freq.nFreq; iFreq++) {
            for(int iSrc=0; iSrc<src.nSrc; iSrc++) {
                delete p_tot[iFreq][iSrc];
            }
            delete[] p_tot[iFreq];
        }
        delete[] p_tot;
        p_tot = nullptr;
    }

    void calculateData(std::complex<T> *p_data) {

        assert(p_tot != nullptr);
        assert(greens != nullptr);

        volComplexField<T> chi_p(chi.GetGrid());
        int nFreq = freq.nFreq;
        int nSrc = src.nSrc;
        int nRecv = recv.nRecv;

        for(int iFreq=0; iFreq<nFreq; iFreq++) {
            for(int iSrc=0; iSrc<nSrc; iSrc++) {
                chi_p = *p_tot[iFreq][iSrc] * chi;
                for(int iRecv=0; iRecv<nRecv; iRecv++) {
                    // TODO: Only correct when recv == src
                    p_data[iFreq * nRecv * nSrc + iRecv * nSrc + iSrc] =
                            Summation(chi_p, *greens[iFreq]->GetReceiverCont(iRecv));
                }
            }

        }

    }

    volField_rect_2D_cpu<T> Reconstruct(const std::complex<T> *p_data, int maxIt, T tol) {

        int nFreq = freq.nFreq;
        int nSrc = src.nSrc;
        int nRecv = recv.nRecv;

        T eta = 1.0 / normSq(p_data, nFreq * nRecv * nSrc);
        T res;

        int nIt = 0;

        std::complex<T> *r = new std::complex<T>[nFreq * nRecv * nSrc];
        volField<T> chi_est(grid);
        chi_est.Zero();
        volField<T> g(grid), g_old(grid);
        volField<T> zeta(grid), zeta_old(grid);
        volComplexField<T> temp(grid);

        // Initialize array
        volComplexField<T> **K = new volComplexField<T>*[nFreq * nRecv * nSrc];
        for(int iFreq=0; iFreq<nFreq; iFreq++) {
            for(int iSrc=0; iSrc<nSrc; iSrc++) {
                for(int iRecv=0; iRecv<nRecv; iRecv++) {
                    K[iFreq * nRecv * nSrc + iRecv * nSrc + iSrc] =
                            new volComplexField<T>(grid);
                }
            }
        }

        volComplexField<T> **p_est = new volComplexField<T>*[nFreq * nSrc];
        for(int iFreq=0; iFreq<nFreq; iFreq++) {
            for(int iSrc=0; iSrc<nSrc; iSrc++) {
                p_est[iFreq * nSrc + iSrc] = new volComplexField<T>(*p_0[iFreq][iSrc]);
            }
        }

        volField<T> diff(grid);

        // Main loop
        for(int it=0; it<maxIt; it++) {

            nIt++;

            // Start of chi update
            std::cout << "Chi update" << std::endl;
            temp.Zero();
            res = 0.0;

            for(int iFreq=0; iFreq<nFreq; iFreq++) {
                for(int iSrc=0; iSrc<nSrc; iSrc++) {
                    for(int iRecv=0; iRecv<nRecv; iRecv++) {

                        int index = iFreq * nRecv * nSrc + iRecv * nSrc + iSrc;

                        *K[index] = *greens[iFreq]->GetReceiverCont(iRecv);
                        *K[index] *= *p_est[iFreq * nSrc + iSrc];

                        r[index] = p_data[index] - Summation(*K[index], chi_est);

                        res += std::pow(real(r[index]), 2.0);
                        res += std::pow(imag(r[index]), 2.0);

                        temp += *K[index] * std::conj(r[index]);

                    }
                }
            }

            res *= eta;

            g = T(2.0 * eta) * temp.GetRealPart();


            if(it == 0) {
                zeta = g;
            }
            else {
                diff = g - g_old;
                T gamma = g.InnerProduct(diff) / std::pow(g_old.Norm(), 2.0);
                zeta = g + gamma * zeta_old;
            }

            T alpha_num = 0.0;
            T alpha_den = 0.0;
            for(int iFreq=0; iFreq<nFreq; iFreq++) {
                for(int iSrc=0; iSrc<nSrc; iSrc++) {
                    for(int iRecv=0; iRecv<nRecv; iRecv++) {
                        int index = iFreq * nRecv * nSrc + iRecv * nSrc + iSrc;

                        std::complex<T> Kzeta = Summation(*K[index], zeta);
                        alpha_num += real(conj(r[index]) * Kzeta);
                        alpha_den += abs(Kzeta) * abs(Kzeta);

                    }
                }
            }

            T alpha = alpha_num / alpha_den;
            T update = alpha * zeta.Norm();
            std::cout << "Update of chi_est = " << update << ", alpha = " << alpha << ", res = " << res << std::endl;
            chi_est += (alpha * zeta);
            g_old = g;
            zeta_old = zeta;
            // End of chi update

            // Start of field update
            std::cout << "Field update" << std::endl;
            profiler.StartRegion("Field update");
            for (int iFreq=0; iFreq < nFreq; iFreq++) {
                std::cout << "Frequency " << iFreq+1 << "  of " << nFreq << std::endl;
                for (int iSrc=0; iSrc < nSrc; iSrc++) {
                    *p_est[iFreq * nSrc + iSrc] = calcField(*greens[iFreq], chi_est, *p_0[iFreq][iSrc], 30, tol);
                }
            }
            profiler.EndRegion();
            // End of field update

        } // End of main loop

        // Clean up

        for(int iFreq=0; iFreq<nFreq; iFreq++) {
            for(int iSrc=0; iSrc<nSrc; iSrc++) {
                delete p_est[iFreq * nSrc + iSrc];
            }
        }

        delete[] p_est;

        delete[] r;

        for(int i=0; i<nFreq*nSrc*nRecv; i++) {
            delete K[i];
        }
        delete[] K;


        volField_rect_2D_cpu<T> result(grid);
        chi_est.CopyTo(result);
        return result;

    }


private:

    const volComplexField<T> &GetP0(int iFreq, int iSrc) const {
        assert(p_0 != nullptr);
        return *p_0[iFreq][iSrc];
    }

    const volComplexField<T> &GetTotalField(int iFreq, int iSrc) const {
        assert(p_tot != nullptr);
        return *p_tot[iFreq][iSrc];
    }

};

#endif /* INVERSION_H */
