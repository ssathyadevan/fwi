#include "complexDataGrid2D.h"
#include "log.h"

complexDataGrid2D::complexDataGrid2D(const grid2D &grid_) :
    _grid(grid_), _data(std::vector<std::complex<double>>(getNumberOfGridPoints(), std::complex<double>(0.0, 0.0)))
{
}

complexDataGrid2D::complexDataGrid2D(const complexDataGrid2D &rhs) : complexDataGrid2D(rhs.getGrid())
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
}

void complexDataGrid2D::zero()
{
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = 0;
    }
}

void complexDataGrid2D::square()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= _data[i];
    }
}

void complexDataGrid2D::sqrt()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::sqrt(_data[i]);
    }
}

void complexDataGrid2D::reciprocal()
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

void complexDataGrid2D::conjugate()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::conj(_data[i]);
    }
}

std::complex<double> complexDataGrid2D::Summation() const
{
    std::complex<double> result = 0.0;
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        result += _data[i];
    }
    return result;
}

double complexDataGrid2D::innerProduct(const complexDataGrid2D &rhs) const
{
    assert(getGrid() == rhs.getGrid());

    double prod = 0.0;
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        prod += real(_data[i] * conj(rhsData[i]));
    }
    return prod;
}

std::complex<double> complexDataGrid2D::dotProduct(const complexDataGrid2D &rhs) const
{
    assert(getGrid() == rhs.getGrid());

    std::complex<double> sum(0.0, 0.0);
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhsData[i];
    }
    return sum;
}

std::complex<double> complexDataGrid2D::dotProduct(const dataGrid2D &rhs) const
{
    assert(getGrid() == rhs.getGrid());

    std::complex<double> sum(0.0, 0.0);
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhsData[i];
    }
    return sum;
}

void complexDataGrid2D::toFile(const std::string &filePath) const
{
    std::ofstream file;
    file.open(filePath, std::ios::out | std::ios::trunc);
    if(!file)
    {
        throw std::invalid_argument("Unable to write DataGrid2D to file: " + filePath);
    }

    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        file << std::showpos << std::setprecision(17) << "(" << real(_data[i]) << imag(_data[i]) << "j)";
    }
    file.close();
}

void complexDataGrid2D::fromFile(const std::string &filePath)
{
    std::ifstream file(filePath, std::ios::in);
    if(!file)
    {
        throw std::invalid_argument("Unable to load DataGrid2D from file: " + filePath);
    }

    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        file >> _data[i];
    }

    file.close();
}

dataGrid2D complexDataGrid2D::getRealPart() const
{
    dataGrid2D result(getGrid());

    std::vector<double> realData(getNumberOfGridPoints(), 0.0);
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        realData[i] = std::real(_data[i]);
    }
    result = realData;
    return result;
}

void complexDataGrid2D::Random()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::complex<double>(double(std::rand()) / double(RAND_MAX), double(std::rand()) / double(RAND_MAX));
    }
}

// Operators
complexDataGrid2D &complexDataGrid2D::operator=(const complexDataGrid2D &rhs)
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

complexDataGrid2D &complexDataGrid2D::operator=(const dataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = complexData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = data[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = complexValue;
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::complex<double>(value, 0.0);
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator+=(const complexDataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator+=(const dataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator+=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += complexData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator+=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += data[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator+=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += complexValue;
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator+=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += value;
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator-=(const complexDataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator-=(const dataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator-=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= complexData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator-=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= data[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator-=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= complexValue;
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator-=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= value;
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator*=(const complexDataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator*=(const dataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator*=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= complexData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator*=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= data[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator*=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= complexValue;
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator*=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= value;
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator/=(const complexDataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());

    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        if(rhsData[i].real() == 0.0 && rhsData[i].imag() == 0.0)
        {
            throw std::overflow_error("Operator devides by zero");
        }
        _data[i] /= rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator/=(const dataGrid2D &rhs)
{
    assert(getGrid() == rhs.getGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        if(rhsData[i] == 0.0)
        {
            throw std::overflow_error("Operator devides by zero");
        }
        _data[i] /= rhsData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator/=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        if(complexData[i].real() == 0.0 && complexData[i].imag() == 0.0)
        {
            throw std::overflow_error("Operator devides by zero");
        }
        _data[i] /= complexData[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator/=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        if(data[i] == 0.0)
        {
            throw std::overflow_error("Operator devides by zero");
        }
        _data[i] /= data[i];
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator/=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        if(complexValue.real() == 0.0 && complexValue.imag() == 0.0)
        {
            throw std::overflow_error("Operator devides by zero");
        }
        _data[i] /= complexValue;
    }
    return *this;
}

complexDataGrid2D &complexDataGrid2D::operator/=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        if(value == 0.0)
        {
            throw std::overflow_error("Operator devides by zero");
        }
        _data[i] /= value;
    }
    return *this;
}
