#include "pressureFieldComplexSerial.h"
#include "log.h"

PressureFieldComplexSerial ::PressureFieldComplexSerial(const Grid2D &grid) :
    PressureFieldComplex(grid), _data(std::vector<std::complex<double>>(GetNumberOfGridPoints()), 0.0), _dataPointer(&_data[0])
{
}

PressureFieldComplexSerial::PressureFieldComplexSerial(const PressureFieldComplexSerial &rhs) : PressureFieldComplexSerial(rhs.GetGrid())
{
    for(int i = 0; i < GetNumberOfGridPoints(); ++i)
    {
        _data[i] = rhs._data[i];
    }
}

PressureFieldComplexSerial::~PressureFieldComplexSerial() {}

void PressureFieldComplexSerial::SetField(const std::function<std::complex<double>(double, double)> func)
{
    const std::array<int, 2> &nx = GetGrid().GetGridDimensions();
    const std::array<double, 2> &dx = GetGrid().GetCellDimensions();   // Babak 2018 10 29: get rid of template for grid_rect_2D
    const std::array<double, 2> &x_min = GetGrid().GetGridStart();     // Babak 2018 10 29: get rid of template for grid_rect_2D

    for(int j = 0; j < nx[1]; j++)
    {
        double z = x_min[1] + (double(0.5) + j) * dx[1];
        int nx_j = nx[0] * j;

        for(int i = 0; i < nx[0]; i++)
        {
            double x = x_min[0] + (double(0.5) + i) * dx[0];
            _data[nx_j + i] = func(x, z);
        }
    }
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

void PressureFieldComplexSerial::Reciprocal()
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        if(_data[i] == 0.0) { throw std::overflow_error("Reciprocal devides by zero"); }
        _data[i] = double(1.0) / _data[i];
    }
}

void PressureFieldComplexSerial::Conjugate()
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = std::conj(_data[i]);
    }
}

void PressureFieldComplexSerial::Random()
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = std::complex<double>(double(std::rand()) / double(RAND_MAX), double(std::rand()) / double(RAND_MAX));
    }
}

double PressureFieldComplexSerial::Norm() const { return std::sqrt(std::real(InnerProduct(*this))); }

double PressureFieldComplexSerial::RelNorm() const { return std::sqrt(std::real(InnerProduct(*this))) / GetNumberOfGridPoints(); }

PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const PressureFieldComplexSerial &rhs)
{
    if(this != &rhs)
    {
        assert(GetNumberOfGridPoints() == rhs.GetNumberOfGridPoints());
        for(int i = 0; i < GetNumberOfGridPoints(); ++i)
        {
            _data[i] = rhs._data[i];
        }
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const PressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const double *rhs_data = rhs.GetDataPtr();
    for(int i = 0; i < GetNumberOfGridPoints(); ++i)
    {
        _data[i] = rhs_data[i];
    }
    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator=(const double &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] = std::complex<double>(rhs, 0.0);
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const PressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());

    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += rhs._data[i];
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const PressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= rhs._data[i];
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const PressureFieldComplexSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= rhs._data[i];
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const PressureFieldSerial &rhs)
{
    assert(GetGrid() == rhs.GetGrid());

    const double *rhs_data = rhs.GetDataPtr();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= rhs_data[i];
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const double &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += rhs;
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const double &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= rhs;
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const double &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= rhs;
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator/=(const double &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] /= rhs;
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator+=(const std::complex<double> &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] += rhs;
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator-=(const std::complex<double> &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] -= rhs;
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator*=(const std::complex<double> &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] *= rhs;
    }

    return *this;
}

PressureFieldComplexSerial &PressureFieldComplexSerial::operator/=(const std::complex<double> &rhs)
{
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        _data[i] /= rhs;
    }

    return *this;
}

std::complex<double> PressureFieldComplexSerial::InnerProduct(const PressureFieldComplexSerial &rhs) const
{
    std::complex<double> prod(0.0, 0.0);
    assert(GetGrid() == rhs.GetGrid());
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        prod += _data[i] * conj(rhs._data[i]);
    }

    return prod;
}

std::complex<double> PressureFieldComplexSerial::DotProduct(const PressureFieldComplexSerial &rhs) const
{
    std::complex<double> sum(0.0, 0.0);
    assert(GetGrid() == rhs.GetGrid());

    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhs._data[i];
    }

    return sum;
}

std::complex<double> PressureFieldComplexSerial::DotProduct(const PressureFieldSerial &rhs) const
{
    std::complex<double> sum(0.0, 0.0);
    assert(GetGrid() == rhs.GetGrid());

    const double *rhs_data = rhs.GetDataPtr();
    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhs_data[i];
    }

    return sum;
}

PressureFieldSerial PressureFieldComplexSerial::GetRealPart() const
{
    PressureFieldSerial result(GetGrid());
    double *data_ptr = result.GetDataPtr();

    for(int i = 0; i < GetNumberOfGridPoints(); i++)
    {
        data_ptr[i] = std::real(_data[i]);
    }

    return result;
}
