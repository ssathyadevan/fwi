#include "volComplexField_rect_2D_cpu.h"

volComplexField_rect_2D_cpu::volComplexField_rect_2D_cpu(const volComplexField_rect_2D_cpu &rhs)
    : volComplexField_rect_2D_cpu(rhs.GetGrid())
{
    memcpy(data, rhs.data, this->GetNumberOfGridPoints() * sizeof(std::complex<double>));
}

volComplexField_rect_2D_cpu::~volComplexField_rect_2D_cpu()
{
    delete[] data;
}

void volComplexField_rect_2D_cpu::SetField(const std::function< std::complex<double>(double,double) > func)
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

void volComplexField_rect_2D_cpu::toBuffer(std::complex<double> *buffer) const
{
    memcpy(buffer, data, sizeof(std::complex<double>)*this->GetNumberOfGridPoints());
}

void volComplexField_rect_2D_cpu::fromBuffer(const std::complex<double> *buffer)
{
    memcpy(data, buffer, sizeof(std::complex<double>)*this->GetNumberOfGridPoints());
}

void volComplexField_rect_2D_cpu::toFile(const std::string &fileName) const
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

void volComplexField_rect_2D_cpu::fromFile(const std::string &fileName)
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

void volComplexField_rect_2D_cpu::Zero()
{
    memset( data, 0, sizeof(std::complex<double>)*this->GetNumberOfGridPoints() );
}

void volComplexField_rect_2D_cpu::Square()
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++) {data[i] *= data[i];}
}


void volComplexField_rect_2D_cpu::Reciprocal()
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++) {data[i] = double(1.0)/data[i];}
}

void volComplexField_rect_2D_cpu::Conjugate()
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++) {data[i] = std::conj(data[i]);}
}

void volComplexField_rect_2D_cpu::Random()
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] = std::complex<double>( double(std::rand())/double(RAND_MAX), double(std::rand())/double(RAND_MAX) );
    }
}

double volComplexField_rect_2D_cpu::Norm() const
{
    return std::sqrt( std::real(InnerProduct(*this)) );
}

double volComplexField_rect_2D_cpu::RelNorm() const
{
    return std::sqrt( std::real(InnerProduct(*this)) ) / this->GetNumberOfGridPoints();
}

const std::complex<double>* volComplexField_rect_2D_cpu::GetDataPtr() const
{
    return data;
}

std::complex<double>* volComplexField_rect_2D_cpu::GetDataPtr()
{
    return data;
}

volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator=(const volComplexField_rect_2D_cpu& rhs)
{
    if (this != &rhs)
    {
        assert(&this->GetGrid() == &rhs.GetGrid());
        memcpy(data, rhs.data, sizeof(std::complex<double>)*this->GetNumberOfGridPoints() );
    }

    return *this;
}

volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator=(const volField_rect_2D_cpu& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    const double *rhs_data = rhs.GetDataPtr();
    memcpy(data, rhs_data, sizeof(std::complex<double>)*this->GetNumberOfGridPoints() );
    return *this;
}

volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator=(const double& rhs)
{
    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] = std::complex<double>(rhs, 0.0);

    return *this;
}

volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator+=(const volComplexField_rect_2D_cpu& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] += rhs.data[i];

    return *this;
}


volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator-=(const volComplexField_rect_2D_cpu& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] -= rhs.data[i];

    return *this;
}


volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator*=(const volComplexField_rect_2D_cpu& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] *= rhs.data[i];

    return *this;
}


volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator*=(const volField_rect_2D_cpu& rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());

    const double *rhs_data = rhs.GetDataPtr();
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] *= rhs_data[i];

    return *this;
}


volComplexField_rect_2D_cpu & volComplexField_rect_2D_cpu::operator+=(const double& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] += rhs;

    return *this;
}


volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator-=(const double& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] -= rhs;

    return *this;
}

volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator*=(const double& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] *= rhs;

    return *this;
}


volComplexField_rect_2D_cpu& volComplexField_rect_2D_cpu::operator/=(const double& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] /= rhs;

    return *this;
}


volComplexField_rect_2D_cpu & volComplexField_rect_2D_cpu::operator+=(const std::complex<double>& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] += rhs;

    return *this;
}


volComplexField_rect_2D_cpu & volComplexField_rect_2D_cpu::operator-=(const std::complex<double>& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] -= rhs;

    return *this;
}

volComplexField_rect_2D_cpu & volComplexField_rect_2D_cpu::operator*=(const std::complex<double>& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] *= rhs;

    return *this;
}


volComplexField_rect_2D_cpu & volComplexField_rect_2D_cpu::operator/=(const std::complex<double>& rhs)
{
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        data[i] /= rhs;

    return *this;
}


std::complex<double> volComplexField_rect_2D_cpu::InnerProduct(const volComplexField_rect_2D_cpu& rhs) const
{
    std::complex<double> prod(0.0,0.0);
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        prod += data[i]*conj(rhs.data[i]);
    return prod;
}

std::complex<double> volComplexField_rect_2D_cpu::Summation(const volComplexField_rect_2D_cpu& rhs) const
{
    std::complex<double> sum(0.0,0.0);
    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        sum += data[i] * rhs.data[i];
    return sum;
}


std::complex<double> volComplexField_rect_2D_cpu::Summation(const volField_rect_2D_cpu& rhs) const
{
    std::complex<double> sum(0.0,0.0);
    assert(&this->GetGrid() == &rhs.GetGrid());

    const double *rhs_data = rhs.GetDataPtr();
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
        sum += data[i] * rhs_data[i];
    return sum;
}


volField_rect_2D_cpu volComplexField_rect_2D_cpu::GetRealPart() const
{
    volField_rect_2D_cpu result(this->GetGrid());
    double *data_ptr = result.GetDataPtr();

    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
        data_ptr[i] = std::real(data[i]);

    return result;
}
