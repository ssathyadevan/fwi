#include "pressureFieldComplexSerial.h"
#include "log.h"

pressureFieldComplexSerial ::pressureFieldComplexSerial(const grid2D &grid) :
    pressureFieldComplex(grid), _data(std::vector<std::complex<double>>(getNumberOfGridPoints(), std::complex<double>(0.0, 0.0))), _dataPointer(&_data[0])
{
}

pressureFieldComplexSerial::pressureFieldComplexSerial(const pressureFieldComplexSerial &rhs) : pressureFieldComplexSerial(rhs.GetGrid())
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
}

// Virtual overrides
void pressureFieldComplexSerial::zero()
{
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = 0;
    }
}

void pressureFieldComplexSerial::square()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= _data[i];
    }
}

void pressureFieldComplexSerial::sqrt()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::sqrt(_data[i]);
    }
}

void pressureFieldComplexSerial::reciprocal()
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

void pressureFieldComplexSerial::conjugate()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::conj(_data[i]);
    }
}

std::complex<double> pressureFieldComplexSerial::Summation() const
{
    std::complex<double> result = 0.0;
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        result += _data[i];
    }
    return result;
}

void pressureFieldComplexSerial::toBuffer(std::complex<double> *buffer) const
{
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        buffer[i] = _data[i];
    }
}

void pressureFieldComplexSerial::fromBuffer(const std::complex<double> *buffer)
{
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = buffer[i];
    }
}

void pressureFieldComplexSerial::toFile(const std::string &fileName) const
{
    std::ofstream file;
    file.open(fileName, std::ios::out | std::ios::trunc);
    if(!file)
    {
        L_(lerror) << "Unable to open the file in .toFile method of volComplexField_rect_2D_cpu class " << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        file << std::showpos << std::setprecision(17) << "(" << real(_data[i]) << imag(_data[i]) << "j)";
    }
    file.close();
}

void pressureFieldComplexSerial::fromFile(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::in);
    if(!file)
    {
        L_(lerror) << "Unable to open the file in .fromFile method of volComplexField_rect_2D_cpu class ";
        std::exit(EXIT_FAILURE);
    }
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        file >> _data[i];
    }
    file.close();
}

// Non virtual members
double pressureFieldComplexSerial::innerProduct(const pressureFieldComplexSerial &rhs) const
{
    assert(GetGrid() == rhs.GetGrid());

    double prod = 0.0;
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        prod += real(_data[i] * conj(rhsData[i]));
    }
    return prod;
}

std::complex<double> pressureFieldComplexSerial::dotProduct(const pressureFieldComplexSerial &rhs) const
{
    assert(GetGrid() == rhs.GetGrid());

    std::complex<double> sum(0.0, 0.0);
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhsData[i];
    }
    return sum;
}

std::complex<double> pressureFieldComplexSerial::dotProduct(const pressureFieldSerial &rhs) const
{
    assert(GetGrid() == rhs.GetGrid());

    std::complex<double> sum(0.0, 0.0);
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhsData[i];
    }
    return sum;
}

pressureFieldSerial pressureFieldComplexSerial::getRealPart() const
{
    pressureFieldSerial result(GetGrid());

    std::vector<double> realData(getNumberOfGridPoints(), 0.0);
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        realData[i] = std::real(_data[i]);
    }
    result = realData;
    return result;
}

void pressureFieldComplexSerial::Random()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::complex<double>(double(std::rand()) / double(RAND_MAX), double(std::rand()) / double(RAND_MAX));
    }
}

// Operators
pressureFieldComplexSerial &pressureFieldComplexSerial::operator=(const pressureFieldComplexSerial &rhs)
{
    if(this == &rhs)
    {
        throw std::logic_error("Assign operator with itself");
    }

    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator=(const pressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = complexData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = data[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = complexValue;
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::complex<double>(value, 0.0);
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator+=(const pressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += rhsData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator+=(const pressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += rhsData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator+=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += complexData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator+=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += data[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator+=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += complexValue;
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator+=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += value;
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator-=(const pressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= rhsData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator-=(const pressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= rhsData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator-=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= complexData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator-=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= data[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator-=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= complexValue;
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator-=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= value;
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator*=(const pressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= rhsData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator*=(const pressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= rhsData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator*=(const std::vector<std::complex<double>> &complexData)
{
    assert(getNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= complexData[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator*=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= data[i];
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator*=(const std::complex<double> complexValue)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= complexValue;
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator*=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= value;
    }
    return *this;
}

pressureFieldComplexSerial &pressureFieldComplexSerial::operator/=(const pressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());

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

pressureFieldComplexSerial &pressureFieldComplexSerial::operator/=(const pressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
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

pressureFieldComplexSerial &pressureFieldComplexSerial::operator/=(const std::vector<std::complex<double>> &complexData)
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

pressureFieldComplexSerial &pressureFieldComplexSerial::operator/=(const std::vector<double> &data)
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

pressureFieldComplexSerial &pressureFieldComplexSerial::operator/=(const std::complex<double> complexValue)
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

pressureFieldComplexSerial &pressureFieldComplexSerial::operator/=(const double value)
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
