#include "pressureFieldComplexSerial.h"
#include "log.h"

PressureFieldComplexSerial ::PressureFieldComplexSerial(const Grid2D &grid) :
    PressureFieldComplex(grid), _data(std::vector<std::complex<double>>(GetNumberOfGridPoints(), 0.0)), _dataPointer(&_data[0])
{
}

PressureFieldComplexSerial::PressureFieldComplexSerial(const PressureFieldComplexSerial &rhs) : PressureFieldComplexSerial(rhs.GetGrid())
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
}

// TODO: REMOVE
void PressureFieldComplexSerial::SetField(const std::function<std::complex<double>(double, double)> func) { (void)func; }

// Virtual overrides
void PressureFieldComplexSerial::Zero()
{
    for(int i = 0; i < GetNumberOfGridPoints(); ++i)
    {
        _data[i] = 0;
    }
}

void PressureFieldComplexSerial::Square()
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= _data[i];
    }
}

void PressureFieldComplexSerial::Sqrt()
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = std::sqrt(_data[i]);
    }
}

void PressureFieldComplexSerial::Reciprocal()
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        if(_data[i] == 0.0)
        {
            throw std::overflow_error("Reciprocal devides by zero");
        }

        _data[i] = 1.0 / _data[i];
    }
}

void PressureFieldComplexSerial::Conjugate()
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = std::conj(_data[i]);
    }
}

std::complex<double> PressureFieldComplexSerial::Summation() const
{
    std::complex<double> result = 0.0;
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        result += _data[i];
    }
    return result;
}

void PressureFieldComplexSerial::toBuffer(std::complex<double> *buffer) const
{
    for(int i = 0; i < GetNumberOfGridPoints(); ++i)
    {
        buffer[i] = _data[i];
    }
}

void PressureFieldComplexSerial::fromBuffer(const std::complex<double> *buffer)
{
    for(int i = 0; i < GetNumberOfGridPoints(); ++i)
    {
        _data[i] = buffer[i];
    }
}

void PressureFieldComplexSerial::toFile(const std::string &fileName) const
{
    std::ofstream file;
    file.open(fileName, std::ios::out | std::ios::trunc);
    if(!file)
    {
        L_(lerror) << "Unable to open the file in .toFile method of volComplexField_rect_2D_cpu class " << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        file << std::showpos << std::setprecision(17) << "(" << real(_data[i]) << imag(_data[i]) << "j)";
    }
    file.close();
}

void PressureFieldComplexSerial::fromFile(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::in);
    if(!file)
    {
        L_(lerror) << "Unable to open the file in .fromFile method of volComplexField_rect_2D_cpu class ";
        std::exit(EXIT_FAILURE);
    }
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        file >> _data[i];
    }
    file.close();
}

// Non virtual members
std::complex<double> PressureFieldComplexSerial::InnerProduct(const PressureFieldComplexSerial &rhs) const
{
    assert(GetGrid() == rhs.GetGrid());

    std::complex<double> prod(0.0, 0.0);
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        prod += _data[i] * conj(rhsData[i]);
    }
    return prod;
}

std::complex<double> PressureFieldComplexSerial::DotProduct(const PressureFieldComplexSerial &rhs) const
{
    assert(GetGrid() == rhs.GetGrid());

    std::complex<double> sum(0.0, 0.0);
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhsData[i];
    }
    return sum;
}

std::complex<double> PressureFieldComplexSerial::DotProduct(const PressureFieldSerial &rhs) const
{
    assert(GetGrid() == rhs.GetGrid());

    std::complex<double> sum(0.0, 0.0);
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhsData[i];
    }
    return sum;
}

PressureFieldSerial PressureFieldComplexSerial::GetRealPart() const
{
    PressureFieldSerial result(GetGrid());

    std::vector<double> realData(GetNumberOfGridPoints(), 0.0);
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        realData[i] = std::real(_data[i]);
    }
    result = realData;

    return result;
}

void PressureFieldComplexSerial::Random()
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = std::complex<double>(double(std::rand()) / double(RAND_MAX), double(std::rand()) / double(RAND_MAX));
    }
}

// Operators
PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const PressureFieldComplexSerial &rhs)
{
    if(this == &rhs)
    {
        throw std::logic_error("Assign operator with itself");
    }

    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const PressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const std::vector<std::complex<double>> &complexData)
{
    assert(GetNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = complexData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const std::vector<double> &data)
{
    assert(GetNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = data[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const std::complex<double> complexValue)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = complexValue;
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const double value)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = std::complex<double>(value, 0.0);
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const PressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const PressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const std::vector<std::complex<double>> &complexData)
{
    assert(GetNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += complexData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const std::vector<double> &data)
{
    assert(GetNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += data[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const std::complex<double> complexValue)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += complexValue;
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const double value)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += value;
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const PressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const PressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const std::vector<std::complex<double>> &complexData)
{
    assert(GetNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= complexData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const std::vector<double> &data)
{
    assert(GetNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= data[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const std::complex<double> complexValue)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= complexValue;
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const double value)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= value;
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const PressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const PressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const std::vector<std::complex<double>> &complexData)
{
    assert(GetNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= complexData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const std::vector<double> &data)
{
    assert(GetNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= data[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const std::complex<double> complexValue)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= complexValue;
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const double value)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= value;
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator/=(const PressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());

    const std::vector<std::complex<double>> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] /= rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator/=(const PressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] /= rhsData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator/=(const std::vector<std::complex<double>> &complexData)
{
    assert(GetNumberOfGridPoints() == (int)complexData.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] /= complexData[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator/=(const std::vector<double> &data)
{
    assert(GetNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] /= data[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator/=(const std::complex<double> complexValue)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] /= complexValue;
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator/=(const double value)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] /= value;
    }
    return *this;
}
