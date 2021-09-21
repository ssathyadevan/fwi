#pragma once

#include "ignoreStupidWarnings.h"
#include "dataGrid2D.h"
#include "grid2D.h"
#include <array>
#include <cassert>
#include <complex>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace fwi
{
    namespace core
    {
        template<class _V = std::complex<double>>
        class complexDataGrid2D
        {
        private:
            const grid2D &_grid;
            std::vector<_V> _data;

        public:
            explicit complexDataGrid2D(const grid2D &grid)
                : _grid(grid),
                  _data(std::vector<_V>(getNumberOfGridPoints()))
            {

            }
            complexDataGrid2D(const complexDataGrid2D<_V> &rhs)
                : complexDataGrid2D(rhs.getGrid())
            {
                assert(getGrid() == rhs.getGrid());
                const std::vector<_V> &rhsData = rhs.getData();
                for(int i = 0; i < getNumberOfGridPoints(); ++i)
                {
                    _data[i] = rhsData[i];
                }
            }
            ~complexDataGrid2D() {}

            const std::vector<_V> &getData() const { return _data; }
            void setValueAtIndex(const _V value, const int index) { _data[index] = value; }
            void addValueAtIndex(const _V value, const int index) { _data[index] += value; }

            const grid2D &getGrid() const { return _grid; }
            int getNumberOfGridPoints() const { return _grid.getNumberOfGridPoints(); }
            double getCellVolume() const { return _grid.getCellVolume(); }

            void zero()
            {
                for(int i = 0; i < getNumberOfGridPoints(); ++i)
                {
                    _data[i] = 0;
                }
            }
            void square()
            {
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] *= _data[i];
                }
            }

            void sqrt()
            {
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] = std::sqrt(_data[i]);
                }
            }

            void reciprocal()
            {
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    if(_data[i] == 0.0)
                    {
                        throw std::overflow_error("reciprocal devides by zero");
                    }
                    _data[i] = 1.0 / _data[i];
                }
            }

            void conjugate()
            {
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] = std::conj(_data[i]);
                }
            }

            void random();
            void randomSaurabh();

            double norm() const { return std::sqrt(innerProduct(*this)); }
            double relNorm() const { return std::sqrt(innerProduct(*this) / getNumberOfGridPoints()); }
            _V summation() const
            {
                _V result = 0.0;
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    result += _data[i];
                }
                return result;
            }

            double innerProduct(const complexDataGrid2D<_V> &rhs) const
            {
                assert(getGrid() == rhs.getGrid());

                double prod = 0.0;
                const std::vector<_V> &rhsData = rhs.getData();
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    prod += std::real(_data[i] * std::conj(rhsData[i]));
                }
                return prod;
            }

            _V dotProduct(const complexDataGrid2D<_V> &rhs) const
            {
                assert(getGrid() == rhs.getGrid());

                _V sum;
                const std::vector<_V> &rhsData = rhs.getData();
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    sum += _data[i] * rhsData[i];
                }
                return sum;
            }

            complexDataGrid2D<double> getRealPart() const
            {
                complexDataGrid2D<double> result(getGrid());

                std::vector<double> realData(getNumberOfGridPoints(), 0.0);
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    realData[i] = std::real(_data[i]);
                }
                result = realData;
                return result;
            }

            void toFile(const std::string &fileName) const;
            void fromFile(const std::string &fileName);


            // Operators
            complexDataGrid2D<_V> &operator=(const complexDataGrid2D &rhs)
            {
                if(this == &rhs)
                {
                    throw std::logic_error("Assign operator with itself");
                }

                assert(getGrid() == rhs.getGrid());
                const std::vector<std::complex<double>> &rhsData = rhs.getData();
                for(int i = 0; i < getNumberOfGridPoints(); ++i)
                {
                    _data[i] = rhsData[i];
                }
                return *this;
            }

            complexDataGrid2D<_V> &operator=(const std::vector<_V> &rhs)
            {
                assert(getNumberOfGridPoints() == (int)rhs.size());
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] = rhs[i];
                }
                return *this;
            }

            complexDataGrid2D<_V> &operator=(const _V rhs)
            {
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] = rhs;
                }
                return *this;
            }

            complexDataGrid2D<_V> &operator+=(const complexDataGrid2D<_V> &rhs)
            {
                assert(getGrid() == rhs.getGrid());
                const std::vector<_V> &rhsData = rhs.getData();
                this += rhsData;
            }

            complexDataGrid2D<_V> &operator+=(const std::vector<_V> &rhs)
            {
                assert(getNumberOfGridPoints() == (int)rhs.size());
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] += rhs[i];
                }

                return *this;
            }

            complexDataGrid2D<_V> &operator+=(const _V rhs)
            {
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] += rhs;
                }

                return *this;
            };

            complexDataGrid2D<_V> &operator-=(const complexDataGrid2D &rhs)
            {
                assert(getGrid() == rhs.getGrid());
                const std::vector<std::complex<double>> &rhsData = rhs.getData();
                this -= rhsData;
            }

            complexDataGrid2D<_V> &operator-=(const std::vector<_V> &rhs)
            {
                assert(getNumberOfGridPoints() == (int)rhs.size());
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] -= rhs[i];
                }

                return *this;
            }

            complexDataGrid2D<_V> &operator-=(const _V rhs)
            {
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] -= rhs;
                }

                return *this;
            }

            complexDataGrid2D<_V> &operator*=(const complexDataGrid2D &rhs)
            {
                assert(getGrid() == rhs.getGrid());
                const auto &rhsData = rhs.getData();
                this *= rhsData;
            }

            complexDataGrid2D<_V> &operator*=(const std::vector<_V> &rhs)
            {
                assert(getNumberOfGridPoints() == (int)rhs.size());
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] *= rhs[i];
                }

                return *this;
            }

            complexDataGrid2D<_V> &operator*=(const _V rhs)
            {
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] *= rhs;
                }

                return *this;
            }

            complexDataGrid2D<_V> &operator/=(const complexDataGrid2D<_V> &rhs)
            {
                const std::vector<_V> &rhsData = rhs.getData();
                this /= rhsData;
            }

            complexDataGrid2D<_V> &operator/=(const std::vector<_V> &rhs)
            {
                assert(getNumberOfGridPoints() == (int)rhs.size());
                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    if(rhs[i] == 0.0)
                    {
                        throw std::overflow_error("Operator devides by zero");
                    }
                    _data[i] /= rhs[i];
                }

                return *this;
            }

            complexDataGrid2D<_V> &operator/=(const _V rhs)
            {
                if(rhs == 0.0)
                {
                    throw std::overflow_error("Operator devides by zero");
                }

                for(int i = 0; i < getNumberOfGridPoints(); i++)
                {
                    _data[i] /= rhs;
                }

                return *this;
            }

        }; // class declaration/definition

        template <class _V = std::complex<double>>
        inline double innerProduct(const complexDataGrid2D<_V> &t1, const complexDataGrid2D<_V> &t2)
        {
            return t1.innerProduct(t2);
        }

        template <class _V = std::complex<double>>
        inline _V dotProduct(const complexDataGrid2D<_V> &t1, const complexDataGrid2D<_V> &t2)
        {
            return t1.dotProduct(t2);
        }

        template <class _V = std::complex<double>>
        inline complexDataGrid2D<_V> operator+(const complexDataGrid2D<_V> &t1, const complexDataGrid2D<_V> &t2)
        {
            complexDataGrid2D t3(t1);
            t3 += t2;
            return t3;
        }

        template <class _V = std::complex<double>>
        inline complexDataGrid2D<_V> operator-(const complexDataGrid2D<_V> &t1, const complexDataGrid2D<_V> &t2)
        {
            complexDataGrid2D t3(t1);
            t3 -= t2;
            return t3;
        }

        template <class _V = std::complex<double>>
        inline complexDataGrid2D<_V> operator*(const complexDataGrid2D<_V> &t1, const complexDataGrid2D<_V> &t2)
        {
            complexDataGrid2D t3(t1);
            t3 *= t2;
            return t3;
        }

        template <class _V = std::complex<double>>
        inline complexDataGrid2D<_V> operator*(const complexDataGrid2D<_V> &t1, const _V t2)
        {
            complexDataGrid2D t3(t1);
            t3 *= t2;
            return t3;
        }

        template <class _V = std::complex<double>>
        inline complexDataGrid2D<_V> operator*(const _V t1, const complexDataGrid2D<_V> &t2) { return t2 * t1; }

        template <class _V = std::complex<double>>
        inline complexDataGrid2D<_V> operator/(const complexDataGrid2D<_V> &t1, const double t2)
        {
            complexDataGrid2D t3(t1);
            t3 /= t2;
            return t3;
        }
    }   // namespace core
}   // namespace fwi
