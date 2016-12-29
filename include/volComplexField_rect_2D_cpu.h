#ifndef COLCOMPLEXFIELD_RECT_2D_CPU_H
#define COLCOMPLEXFIELD_RECT_2D_CPU_H

#include <iostream>
#include <complex>
#include <cassert>
#include <functional>
#include <array>
#include <vector>
#include <string>

#include "volComplexField_rect_2D.h"
#include "grid_rect_2D.h"
#include "volField_rect_2D_cpu.h"

template <class T>
class volComplexField_rect_2D_cpu : public volComplexField_rect_2D<T>
{
    private:
    std::complex<T> * const data;

    public:
        volComplexField_rect_2D_cpu(const grid_rect_2D<T> &grid)
        : volComplexField_rect_2D<T>(grid), data(new std::complex<T>[this->GetNumberOfGridPoints()])
        {
        }

        volComplexField_rect_2D_cpu(const volComplexField_rect_2D_cpu<T> &rhs)
        : volComplexField_rect_2D_cpu<T>(rhs.GetGrid())
        {
            memcpy(data, rhs.data, this->GetNumberOfGridPoints() * sizeof(std::complex<T>));
        }


        virtual ~volComplexField_rect_2D_cpu()
        {
            delete[] data;
        }


        virtual void SetField(const std::function< std::complex<T>(T,T) > func)
        {
            const std::array<int,2> &nx = this->GetGrid().GetGridDimensions();
            const std::array<T,2> &dx = this->GetGrid().GetCellDimensions();
            const std::array<T,2> &x_min = this->GetGrid().GetGridStart();

            for (int j=0; j<nx[1]; j++)
            {
                T z = x_min[1] + (T(0.5) + j)*dx[1];
                int nx_j = nx[0]*j;

                for (int i=0; i<nx[0]; i++)
                {
                    T x = x_min[0] + (T(0.5) + i)*dx[0];
                    data[nx_j+i] = func(x,z);
                }
            }
        }

        virtual void toBuffer(std::complex<T> *buffer) const
        {
            memcpy(buffer, data, sizeof(std::complex<T>)*this->GetNumberOfGridPoints());
        }

        virtual void fromBuffer(const std::complex<T> *buffer)
        {
            memcpy(data, buffer, sizeof(std::complex<T>)*this->GetNumberOfGridPoints());
        }


        virtual void toFile(const std::string &fileName) const
        {
            std::ofstream file;
            file.open(fileName, std::ios::out | std::ios::trunc);
            assert(file.is_open());

            for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
              file << std::showpos << std::setprecision(17) << "(" << real(data[i]) << imag(data[i]) << "j)" << std::endl;
            }
            file.close();

        }


        virtual void fromFile(const std::string &fileName)
        {

            std::ifstream file(fileName, std::ios::in);
            assert(file.is_open());

            for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
              file >> data[i];
            }
            file.close();

        }

        virtual void Zero()
        {
            memset( data, 0, sizeof(std::complex<T>)*this->GetNumberOfGridPoints() );
        }

        virtual void Square()
        {
            for(int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] *= data[i];
        }

        virtual void Reciprocal()
        {
            for(int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] = T(1.0)/data[i];
        }

        virtual void Conjugate()
        {
            for(int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] = std::conj(data[i]);
        }

        virtual void Random()
        {
            for(int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] = std::complex<T>( T(std::rand())/T(RAND_MAX), T(std::rand())/T(RAND_MAX) );
        }

        virtual T Norm() const
        {
            return std::sqrt( std::real(InnerProduct(*this)) );
        }

        virtual T RelNorm() const
        {
            return std::sqrt( std::real(InnerProduct(*this)) ) / this->GetNumberOfGridPoints();
        }

        const std::complex<T>* GetDataPtr() const
        {
            return data;
        }

        std::complex<T>* GetDataPtr()
        {
            return data;
        }

        volComplexField_rect_2D_cpu<T>& operator=(const volComplexField_rect_2D_cpu<T>& rhs)
        {
            if (this != &rhs)
            {
                assert(&this->GetGrid() == &rhs.GetGrid());
                memcpy(data, rhs.data, sizeof(std::complex<T>)*this->GetNumberOfGridPoints() );
            }

            return *this;
        }

        volComplexField_rect_2D_cpu<T>& operator=(const volField_rect_2D_cpu<T>& rhs)
        {
            assert(&this->GetGrid() == &rhs.GetGrid());
            const T *rhs_data = rhs.GetDataPtr();
            memcpy(data, rhs_data, sizeof(std::complex<T>)*this->GetNumberOfGridPoints() );
            return *this;
        }

        volComplexField_rect_2D_cpu<T>& operator=(const T& rhs)
        {
            for(int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] = std::complex<T>(rhs, 0.0);

            return *this;
        }

        volComplexField_rect_2D_cpu<T>& operator+=(const volComplexField_rect_2D_cpu<T>& rhs)
        {
            assert(&this->GetGrid() == &rhs.GetGrid());

            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] += rhs.data[i];

            return *this;
        }


        volComplexField_rect_2D_cpu<T>& operator-=(const volComplexField_rect_2D_cpu<T>& rhs)
        {
            assert(&this->GetGrid() == &rhs.GetGrid());
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] -= rhs.data[i];

            return *this;
        }


        volComplexField_rect_2D_cpu<T>& operator*=(const volComplexField_rect_2D_cpu<T>& rhs)
        {
            assert(&this->GetGrid() == &rhs.GetGrid());
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] *= rhs.data[i];

            return *this;
        }


        volComplexField_rect_2D_cpu<T>& operator*=(const volField_rect_2D_cpu<T>& rhs)
        {
            assert(&this->GetGrid() == &rhs.GetGrid());

            const T *rhs_data = rhs.GetDataPtr();
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] *= rhs_data[i];

            return *this;
        }


        volComplexField_rect_2D_cpu<T> & operator+=(const T& rhs)
        {
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] += rhs;

            return *this;
        }


        volComplexField_rect_2D_cpu<T> & operator-=(const T& rhs)
        {
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] -= rhs;

            return *this;
        }

        volComplexField_rect_2D_cpu<T> & operator*=(const T& rhs)
        {
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] *= rhs;

            return *this;
        }


        volComplexField_rect_2D_cpu<T> & operator/=(const T& rhs)
        {
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] /= rhs;

            return *this;
        }


        volComplexField_rect_2D_cpu<T> & operator+=(const std::complex<T>& rhs)
        {
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] += rhs;

            return *this;
        }


        volComplexField_rect_2D_cpu<T> & operator-=(const std::complex<T>& rhs)
        {
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] -= rhs;

            return *this;
        }

        volComplexField_rect_2D_cpu<T> & operator*=(const std::complex<T>& rhs)
        {
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] *= rhs;

            return *this;
        }


        volComplexField_rect_2D_cpu<T> & operator/=(const std::complex<T>& rhs)
        {
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                data[i] /= rhs;

            return *this;
        }


        std::complex<T> InnerProduct(const volComplexField_rect_2D_cpu<T>& rhs) const
        {
            std::complex<T> prod(0.0,0.0);
            assert(&this->GetGrid() == &rhs.GetGrid());
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                prod += data[i]*conj(rhs.data[i]);
            return prod;
        }

        std::complex<T> Summation(const volComplexField_rect_2D_cpu<T>& rhs) const
        {
            std::complex<T> sum(0.0,0.0);
            assert(&this->GetGrid() == &rhs.GetGrid());

            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                sum += data[i] * rhs.data[i];
            return sum;
        }


        /*std::complex<T> Summation(const volField_rect_2D_cpu<T>& rhs, const int &conju) const
        {
            std::complex<T> sum(0.0,0.0);
            assert(&this->GetGrid() == &rhs.GetGrid());
            const T *rhs_data = rhs.GetDataPtr();

            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                sum += std::conj(data[i]) * rhs_data[i];
            return sum;
        }*/


        std::complex<T> Summation(const volField_rect_2D_cpu<T>& rhs) const
        {
            std::complex<T> sum(0.0,0.0);
            assert(&this->GetGrid() == &rhs.GetGrid());

            const T *rhs_data = rhs.GetDataPtr();
            for (int i=0; i<this->GetNumberOfGridPoints(); i++)
                sum += data[i] * rhs_data[i];
            return sum;
        }


        volField_rect_2D_cpu<T> GetRealPart() const
        {
            volField_rect_2D_cpu<T> result(this->GetGrid());
            T *data_ptr = result.GetDataPtr();

            for(int i=0; i<this->GetNumberOfGridPoints(); i++)
                data_ptr[i] = std::real(data[i]);

            return result;
        }


      void GetLSCoefficients(std::vector< volComplexField_rect_2D_cpu<T>* > A_vectors, std::complex<T> *x, int n) {

        for(int i=0; i<n; i++) {
          x[i] = std::complex<T>(1.0, 0.0);
        }

    /*
    int nGrid = this->GetGrid().this->GetNumberOfGridPoints();

    std::complex<T> *A = new std::complex<T>[nGrid * n];
    std::complex<T> *f = new std::complex<T>[nGrid];

    this->toBuffer(f);

    for(int i=0; i<n; i++) {
      A_vectors[i]->toBuffer(&A[i * nGrid]);
    }

#ifndef NDEBUG
    int info = gels_wrap(nGrid, n, A, f);
    assert(info == 0);
#else
    gels_wrap(nGrid, n, A, f);
#endif

    for(int i=0; i<n; i++) {
      x[i] = f[i];
    }

    delete[] A;
    delete[] f;*/

  }


    protected:



};


template<typename T>
std::complex<T> InnerProduct(const volComplexField_rect_2D_cpu<T> &t1, const volComplexField_rect_2D_cpu<T> &t2)
{
    return t1.InnerProduct(t2);
}


template<typename T>
std::complex<T> Summation(const volComplexField_rect_2D_cpu<T> &t1, const volComplexField_rect_2D_cpu<T> &t2)
{
    return t1.Summation(t2);
}


/*template<typename T>
std::complex<T> Summation(const volComplexField_rect_2D_cpu<T> &t1, const volField_rect_2D_cpu<T> &t2, const int &conju)
{
    std::complex<T> sum;
    if (conju==0) ////if conju==0 then do not take the conjugate of Kappa
        sum = t1.Summation(t2);
    else if(conju == 1) ////if conju==1 then take the conjugate of Kappa
        sum = t1.Summation(t2,conju);

    return sum;
}*/


template<typename T>
std::complex<T> Summation(const volComplexField_rect_2D_cpu<T> &t1, const volField_rect_2D_cpu<T> &t2)
{
    return t1.Summation(t2);
}


template<typename T>
volComplexField_rect_2D_cpu<T> operator-(const volComplexField_rect_2D_cpu<T> &t1, const volComplexField_rect_2D_cpu<T> &t2)
{
    volComplexField_rect_2D_cpu<T> t3(t1);
    t3 -= t2;
    return t3;
}


template<typename T>
volComplexField_rect_2D_cpu<T> operator*(const volComplexField_rect_2D_cpu<T> &t1, const volField_rect_2D_cpu<T> &t2)
{
    volComplexField_rect_2D_cpu<T> t3(t1);
    t3 *= t2;
    return t3;
}


template<typename T>
volComplexField_rect_2D_cpu<T> operator*(const volComplexField_rect_2D_cpu<T> &t1, const volComplexField_rect_2D_cpu<T> &t2)
{
    volComplexField_rect_2D_cpu<T> t3(t1);
    t3 *= t2;
    return t3;
}


template<typename T>
volComplexField_rect_2D_cpu<T> operator*(const volField_rect_2D_cpu<T> &t1, const volComplexField_rect_2D_cpu<T> &t2)
{
    return t2*t1;
}



template<typename T>
volComplexField_rect_2D_cpu<T> operator*(const volComplexField_rect_2D_cpu<T> &t1, const std::complex<T> &t2)
{
    volComplexField_rect_2D_cpu<T> t3(t1);
    t3 *= t2;
    return t3;
}


template<typename T>
volComplexField_rect_2D_cpu<T> operator*(const std::complex<T> &t1, const volComplexField_rect_2D_cpu<T> &t2)
{
    return t2*t1;
}


template<typename T>
volComplexField_rect_2D_cpu<T> operator/(const volComplexField_rect_2D_cpu<T> &t1, const T &t2)
{
    volComplexField_rect_2D_cpu<T> t3(t1);
    t3 /= t2;
    return t3;
}
#endif // COLCOMPLEXFIELD_RECT_2D_CPU_H

