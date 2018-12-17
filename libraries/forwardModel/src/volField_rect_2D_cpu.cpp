#include "volField_rect_2D_cpu.h"



volField_rect_2D_cpu::volField_rect_2D_cpu(const grid_rect_2D &grid) :
    volField_rect_2D(grid),
    data(new double[this->GetNumberOfGridPoints()]) {}

volField_rect_2D_cpu::volField_rect_2D_cpu(const volField_rect_2D_cpu &rhs) :
    volField_rect_2D_cpu(rhs.grid)
{

    memcpy(data, rhs.data, sizeof(double) * this->GetNumberOfGridPoints());

}

volField_rect_2D_cpu::~volField_rect_2D_cpu()
{
    delete[] data;
}

// Virtual overrides
void volField_rect_2D_cpu::Zero()
{
    memset(data, 0, sizeof(double) * this->GetNumberOfGridPoints());
}

void volField_rect_2D_cpu::Random()
{
    for(int i=0; i<this->GetGrid().GetNumberOfGridPoints(); i++)
    {
        data[i] = double(std::rand()) / double(RAND_MAX);
    }
}

void volField_rect_2D_cpu::toBuffer(double *buffer) const
{
    memcpy(buffer, data, sizeof(double) * this->GetNumberOfGridPoints());
}

void volField_rect_2D_cpu::fromBuffer(const double *buffer)
{
    memcpy(data, buffer, sizeof(double) * this->GetNumberOfGridPoints());
}

void volField_rect_2D_cpu::toFile(const std::string &fileName) const
{
    std::ofstream file;
    file.open (fileName, std::ios::out | std::ios::trunc);
    assert(file.is_open());

    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
    {
        file << std::setprecision(17) << data[i] << std::endl;
    }
    file.close();
}

void volField_rect_2D_cpu::fromFile(const Input& input)
{
    std::ifstream file(input.inputCardPath+input.fileName+".txt", std::ios::in);
    assert(file.is_open());

    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
    {
        file >> data[i];
    }
    file.close();
}

void volField_rect_2D_cpu::SetField(const std::function< double(double,double) > func)
{
    const std::array<int, 2> &nx = this->GetGrid().GetGridDimensions();
    const std::array<double, 2> &dx = this->GetGrid().GetCellDimensions();
    const std::array<double, 2> &x_min = this->GetGrid().GetGridStart();

    for(int i=0; i<nx[1]; i++)
    {
        double z = x_min[1] + (i + double(0.5)) * dx[1];
        for(int j=0; j<nx[0]; j++)
        {
            double x = x_min[0] + (j + double(0.5)) * dx[0];
            data[i * nx[0] + j] = func(x,z);
        }
    }
}

double volField_rect_2D_cpu::Norm() const { return std::sqrt(InnerProduct(*this)); }
double volField_rect_2D_cpu::RelNorm() const { return std::sqrt(InnerProduct(*this) / this->GetNumberOfGridPoints()); }

void volField_rect_2D_cpu::Square()
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= data[i];
    }
}

void volField_rect_2D_cpu::Sqrt()
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] = std::sqrt(data[i]);
    }
}

void volField_rect_2D_cpu::Reciprocal() {
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] = double(1.0) / data[i];
    }
}

double volField_rect_2D_cpu::Summation() const
{
    double result = double(0.0);
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        result += data[i];
    }
    return result;
}

// Non virtual members
double volField_rect_2D_cpu::InnerProduct(const volField_rect_2D_cpu &y) const
{
    assert(&this->GetGrid() == &y.GetGrid());
    double result = double(0.0);
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        result += data[i] * y.data[i];
    }
    return result;
}

double volField_rect_2D_cpu::Summation(const volField_rect_2D_cpu &rhs) const
{
    double sum = double(0.0);
    assert(&this->GetGrid() == &rhs.GetGrid());

    const double *rhs_data = rhs.GetDataPtr();
    for (int i=0; i<this->GetNumberOfGridPoints(); i++)
    {
           sum += data[i] * rhs_data[i];
    }
    return sum;
}

void volField_rect_2D_cpu::Gradient(volField_rect_2D_cpu **output)
{
    //note that the python script has the order reversed, so gradient(c++)[0] is gradient(python)[1] and vice versa, switch for clarity?

    const std::array<int, 2> &nx = this->GetGrid().GetGridDimensions();
    const std::array<double, 2> &dx = this->GetGrid().GetCellDimensions();

    for(int i=0; i<nx[1]; i++)
    {
        for(int j=0; j<nx[0]; j++)
        {
            //direction 1 dx
            if (j == 0)
            {
                output[0]->data[i * nx[0] + j] = (data[i * nx[0] + j + 2] - 4 * data[i * nx[0] + j + 1] + 3 * data[i * nx[0] + j]) / (-double(2.0)*dx[0]);
            }
            else if (j == nx[0]-1)
            {
                output[0]->data[i * nx[0] + j] = (data[i * nx[0] + j - 2] - 4 * data[i * nx[0] + j - 1] + 3 * data[i * nx[0] + j]) / (double(2.0)*dx[0]);
            }
            else
            {
                output[0]->data[i * nx[0] + j] = (data[i * nx[0] + j + 1] - data[i * nx[0] + j - 1]) / (double(2.0)*dx[0]);
            }

            //direction 2 dz
            if (i == 0)
            {
                output[1]->data[i * nx[0] + j] = (data[(i + 2) * nx[0] + j] - 4 * data[(i + 1) * nx[0] + j] + 3 * data[i * nx[0] + j]) / (-double(2.0)*dx[1]);
            }
            else if (i == nx[1]-1)
            {
                output[1]->data[i * nx[0] + j] = (data[(i - 2) * nx[0] + j] - 4 * data[(i - 1) * nx[0] + j] + 3 * data[i * nx[0] + j]) / (double(2.0)*dx[1]);
            }
            else
            {
                output[1]->data[i * nx[0] + j] = (data[(i + 1) * nx[0] + j] - data[(i - 1) * nx[0] + j]) / (double(2.0)*dx[1]);
            }
        }
    }
}

volField_rect_2D_cpu& volField_rect_2D_cpu::operator=(const volField_rect_2D_cpu& rhs)
{
    if (this != &rhs)
    {
        assert(&this->GetGrid() == &rhs.GetGrid());
        memcpy(data, rhs.data, sizeof(double) * this->GetNumberOfGridPoints());
    }

    return *this;
}

volField_rect_2D_cpu& volField_rect_2D_cpu::operator=(const double rhs)
{
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] = rhs;
    }
    return *this;
}


volField_rect_2D_cpu& volField_rect_2D_cpu::operator-=(const volField_rect_2D_cpu &rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] -= rhs.data[i];
    }
    return *this;
}

volField_rect_2D_cpu& volField_rect_2D_cpu::operator*=(const volField_rect_2D_cpu &rhs)
{

    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= rhs.data[i];
    }
    return *this;
}

volField_rect_2D_cpu& volField_rect_2D_cpu::operator/=(const volField_rect_2D_cpu &rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] /= rhs.data[i];
    }
    return *this;
}




volField_rect_2D_cpu& volField_rect_2D_cpu::operator-=(const double rhs)
{
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] -= rhs;
    }
    return *this;
}

volField_rect_2D_cpu& volField_rect_2D_cpu::operator*=(const double rhs)
{
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= rhs;
    }
    return *this;
}

volField_rect_2D_cpu& volField_rect_2D_cpu::operator/=(const double rhs)
{
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] /= rhs;
    }
    return *this;
}

void volField_rect_2D_cpu::CopyTo(volField_rect_2D_cpu &dest)
{
    dest = *this;
}

volField_rect_2D_cpu& volField_rect_2D_cpu::operator+=(const volField_rect_2D_cpu &rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] += rhs.data[i];
    }
    return *this;
}

volField_rect_2D_cpu& volField_rect_2D_cpu::operator+=(const double rhs)
{

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] += rhs;
    }

    return *this;

}

