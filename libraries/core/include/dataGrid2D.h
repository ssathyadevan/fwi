#pragma once

#include "grid2D.h"
#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string.h>

namespace core
{
    class dataGrid2D
    {
    private:
        const grid2D &_grid;
        std::vector<double> _data;

    public:
        explicit dataGrid2D(const grid2D &grid);
        dataGrid2D(const dataGrid2D &rhs);
        ~dataGrid2D() {}

        const std::vector<double> &getData() const { return _data; }
        void setValueAtIndex(const double value, const int index) { _data[index] = value; }
        void addValueAtIndex(const double value, const int index) { _data[index] += value; }

        const grid2D &getGrid() const { return _grid; }
        int getNumberOfGridPoints() const { return _grid.getNumberOfGridPoints(); }
        double getCellVolume() const { return _grid.getCellVolume(); }

        void zero();
        void square();
        void sqrt();
        void reciprocal();

        double norm() const { return std::sqrt(innerProduct(*this)); }
        double relNorm() const { return std::sqrt(innerProduct(*this) / getNumberOfGridPoints()); }
        double summation() const;

        double innerProduct(const dataGrid2D &rhs) const;
        void gradient(std::vector<dataGrid2D> &gradientField) const;

        void toFile(const std::string &filePath) const;
        void fromFile(const std::string &filePath);

        void copyTo(dataGrid2D &dest) { dest = *this; }

        void random();          // unused?
        void randomSaurabh();   // Generates random approximations of Saurabh
        void randomChild(const dataGrid2D &parent, std::default_random_engine &generator, std::normal_distribution<double> &distribution);

        // Operators
        dataGrid2D &operator=(const dataGrid2D &rhs);
        dataGrid2D &operator=(const std::vector<double> &rhs);
        dataGrid2D &operator=(const double rhs);

        dataGrid2D &operator+=(const dataGrid2D &rhs);
        dataGrid2D &operator+=(const std::vector<double> &rhs);
        dataGrid2D &operator+=(const double rhs);

        dataGrid2D &operator-=(const dataGrid2D &rhs);
        dataGrid2D &operator-=(const std::vector<double> &rhs);
        dataGrid2D &operator-=(const double rhs);

        dataGrid2D &operator*=(const dataGrid2D &rhs);
        dataGrid2D &operator*=(const std::vector<double> &rhs);
        dataGrid2D &operator*=(const double rhs);

        dataGrid2D &operator/=(const dataGrid2D &rhs);
        dataGrid2D &operator/=(const std::vector<double> &rhs);
        dataGrid2D &operator/=(const double rhs);
    };

    inline double innerProduct(const dataGrid2D &x, const dataGrid2D &y) { return x.innerProduct(y); }

    inline dataGrid2D operator+(const dataGrid2D &x, const dataGrid2D &y)
    {
        dataGrid2D result(x);
        result += y;
        return result;
    }

    inline dataGrid2D operator+(const dataGrid2D &x, double y)
    {
        dataGrid2D result(x);
        result += y;
        return result;
    }

    inline dataGrid2D operator-(const dataGrid2D &x, const dataGrid2D &y)
    {
        dataGrid2D result(x);
        result -= y;
        return result;
    }

    inline dataGrid2D operator*(const dataGrid2D &x, const dataGrid2D &y)
    {
        dataGrid2D result(x);
        result *= y;
        return result;
    }

    inline dataGrid2D operator*(double x, const dataGrid2D &y)
    {
        dataGrid2D result(y);
        result *= x;
        return result;
    }

    inline dataGrid2D operator/(const dataGrid2D &x, const dataGrid2D &y)
    {
        dataGrid2D result(x);
        result /= y;
        return result;
    }
}   // namespace core
