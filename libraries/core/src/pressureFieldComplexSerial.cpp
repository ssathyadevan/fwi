#include "pressureFieldComplexSerial.h"

pressureFieldComplexSerial::pressureFieldComplexSerial(const pressureFieldComplexSerial &rhs)
    : pressureFieldComplexSerial(rhs.GetGrid())
{
    memcpy(data, rhs.data, this->GetNumberOfGridPoints() * sizeof(std::complex<double>));
}

pressureFieldComplexSerial::~pressureFieldComplexSerial()
{
    delete[] data;
}

void pressureFieldComplexSerial::SetField(const std::function< std::complex<double>(double,double) > func)
{
    const std::array<int,2> &nx = this->GetGrid().GetGridDimensions();
    //const std::array<T,2> &dx = this->GetGrid().GetCellDimensions();// Babak 2018 10 29: get rid of template for grid_rect_2D
    //const std::array<T,2> &x_min = this->GetGrid().GetGridStart();// Babak 2018 10 29: get rid of template for grid_rect_2D
    const std::array<double,2> &dx = this->GetGrid().GetCellDimensions();// Babak 2018 10 29: get rid of template for grid_rect_2D
    const std::array<double,2> &x_min = this->GetGrid().GetGridStart();// Babak 2018 10 29: get rid of template for grid_rect_2D

    for (int j=0; j<nx[1]; j++)
    {
        double z = x_min[1] + (double(0.5) + j)*dx[1];
        int nx_j = nx[0]*j;

        for (int i=0; i<nx[0]; i++)
        {
            double x = x_min[0] + (double(0.5) + i)*dx[0];
            data[nx_j+i] = func(x,z);
        }
    }
}

void pressureFieldComplexSerial::toBuffer(std::complex<double> *buffer) const
{
    memcpy(buffer, data, sizeof(std::complex<double>)*this->GetNumberOfGridPoints());
}

void pressureFieldComplexSerial::fromBuffer(const std::complex<double> *buffer)
{
    memcpy(data, buffer, sizeof(std::complex<double>)*this->GetNumberOfGridPoints());
}

void pressureFieldComplexSerial::toFile(const std::string &fileName) const
{
    std::ofstream file;
    file.open(fileName, std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout<< "Unable to open the file in .toFile method of volComplexField_rect_2D_cpu class " << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        file << std::showpos << std::setprecision(17) << "(" << real(data[i]) << imag(data[i]) << "j)" << std::endl;
    }
    file.close();
}

void pressureFieldComplexSerial::fromFile(const std::string &fileName)
{

    std::ifstream file(fileName, std::ios::in);
    if (!file)
    {
        std::cout<< "Unable to open the file in .fromFile method of volComplexField_rect_2D_cpu class " << std::endl;
        std::exit(EXIT_FAILURE);
    }
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        file >> data[i];
    }
    file.close();

}

void pressureFieldComplexSerial::Zero()
{
    memset( data, 0, sizeof(std::complex<double>)*this->GetNumberOfGridPoints() );
}

void pressureFieldComplexSerial::Square()
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++) {data[i] *= data[i];}
}


void pressureFieldComplexSerial::Reciprocal()
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++) {data[i] = double(1.0)/data[i];}
}

void pressureFieldComplexSerial::Conjugate()
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++) {data[i] = std::conj(data[i]);}
}

void pressureFieldComplexSerial::Random()
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] = std::complex<double>( double(std::rand())/double(RAND_MAX), double(std::rand())/double(RAND_MAX) );
    }
}

double pressureFieldComplexSerial::Norm() const
{
    return std::sqrt( std::real(InnerProduct(*this)) );
}

double pressureFieldComplexSerial::RelNorm() const
{
    return std::sqrt( std::real(InnerProduct(*this)) ) / this->GetNumberOfGridPoints();
}

const std::complex<double>* pressureFieldComplexSerial::GetDataPtr() const
{
    return data;
}

std::complex<double>* pressureFieldComplexSerial::GetDataPtr()
{
    return data;
}

pressureFieldComplexSerial& pressureFieldComplexSerial::operator=(const pressureFieldComplexSerial& rhs)
{
    if (this != &rhs)
    {
        assert(&this->GetGrid() == &rhs.GetGrid());
        memcpy(data, rhs.data, sizeof(std::complex<double>)*this->GetNumberOfGridPoints() );
    }

    return *this;
}

pressureFieldComplexSerial& pressureFieldComplexSerial::operator=(const pressureFieldSerial& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    const double *rhs_data = rhs.GetDataPtr();
    memcpy(data, rhs_data, sizeof(std::complex<double>)*this->GetNumberOfGridPoints() );
    return *this;
}

pressureFieldComplexSerial& pressureFieldComplexSerial::operator=(const double& rhs)
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] = std::complex<double>(rhs, 0.0);

    return *this;
}

pressureFieldComplexSerial& pressureFieldComplexSerial::operator+=(const pressureFieldComplexSerial& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] += rhs.data[i];

    return *this;
}


pressureFieldComplexSerial& pressureFieldComplexSerial::operator-=(const pressureFieldComplexSerial& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] -= rhs.data[i];

    return *this;
}


pressureFieldComplexSerial& pressureFieldComplexSerial::operator*=(const pressureFieldComplexSerial& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] *= rhs.data[i];

    return *this;
}


pressureFieldComplexSerial& pressureFieldComplexSerial::operator*=(const pressureFieldSerial& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());

    const double *rhs_data = rhs.GetDataPtr();
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] *= rhs_data[i];

    return *this;
}


pressureFieldComplexSerial & pressureFieldComplexSerial::operator+=(const double& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] += rhs;

    return *this;
}


pressureFieldComplexSerial& pressureFieldComplexSerial::operator-=(const double& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] -= rhs;

    return *this;
}

pressureFieldComplexSerial& pressureFieldComplexSerial::operator*=(const double& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] *= rhs;

    return *this;
}


pressureFieldComplexSerial& pressureFieldComplexSerial::operator/=(const double& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] /= rhs;

    return *this;
}


pressureFieldComplexSerial & pressureFieldComplexSerial::operator+=(const std::complex<double>& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] += rhs;

    return *this;
}


pressureFieldComplexSerial & pressureFieldComplexSerial::operator-=(const std::complex<double>& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] -= rhs;

    return *this;
}

pressureFieldComplexSerial & pressureFieldComplexSerial::operator*=(const std::complex<double>& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] *= rhs;

    return *this;
}


pressureFieldComplexSerial & pressureFieldComplexSerial::operator/=(const std::complex<double>& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] /= rhs;

    return *this;
}


std::complex<double> pressureFieldComplexSerial::InnerProduct(const pressureFieldComplexSerial& rhs) const
{
    std::complex<double> prod(0.0,0.0);
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        prod += data[i]*conj(rhs.data[i]);
    return prod;
}

std::complex<double> pressureFieldComplexSerial::Summation(const pressureFieldComplexSerial& rhs) const
{
    std::complex<double> sum(0.0,0.0);
    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        sum += data[i] * rhs.data[i];
    return sum;
}


std::complex<double> pressureFieldComplexSerial::Summation(const pressureFieldSerial& rhs) const
{
    std::complex<double> sum(0.0,0.0);
    assert(&this->GetGrid() == &rhs.GetGrid());

    const double *rhs_data = rhs.GetDataPtr();
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        sum += data[i] * rhs_data[i];
    return sum;
}

pressureFieldSerial pressureFieldComplexSerial::GetRealPart() const
{
    pressureFieldSerial result(this->GetGrid());
    double *data_ptr = result.GetDataPtr();

    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
        data_ptr[i] = std::real(data[i]);

    return result;
}
