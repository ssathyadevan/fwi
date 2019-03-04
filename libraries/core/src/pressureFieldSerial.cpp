#include "pressureFieldSerial.h"


pressureFieldSerial::pressureFieldSerial(const grid2D &grid) :
    pressureField(grid),
    data(new double[this->GetNumberOfGridPoints()]) {}

pressureFieldSerial::pressureFieldSerial(const pressureFieldSerial &rhs) :
    pressureFieldSerial(rhs.grid)
{

    memcpy(data, rhs.data, sizeof(double) * this->GetNumberOfGridPoints());

}

pressureFieldSerial::~pressureFieldSerial()
{
    delete[] data;
}

// Virtual overrides
void pressureFieldSerial::Zero()
{
    memset(data, 0, sizeof(double) * this->GetNumberOfGridPoints());
}

void pressureFieldSerial::Random()
{
    for(int i=0; i<this->GetGrid().GetNumberOfGridPoints(); i++)
    {
        data[i] = double(std::rand()) / double(RAND_MAX);
    }
}

void pressureFieldSerial::RandomSaurabh()
{
    for(int i=0; i<this->GetGrid().GetNumberOfGridPoints(); i++)
    {
        int temp = rand() % 1800;
        data[i] = (double)temp/10000;
    }
}


void pressureFieldSerial::toBuffer(double *buffer) const
{
    memcpy(buffer, data, sizeof(double) * this->GetNumberOfGridPoints());
}

void pressureFieldSerial::fromBuffer(const double *buffer)
{
    memcpy(data, buffer, sizeof(double) * this->GetNumberOfGridPoints());
}

void pressureFieldSerial::toFile(const std::string &fileName) const
{
    std::ofstream file;
    file.open (fileName, std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout<< "Unable to open the file in .toFile method of volField_rect_2D_cpu class " << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
    {
        file << std::setprecision(17) << data[i] << std::endl;
    }
    file.close();
}

void pressureFieldSerial::fromFile(const genericInput& input)
{
    std::string inputFilePath = input.inputCardPath;
    inputFilePath = inputFilePath.substr(0,  inputFilePath.size()-1);
    unsigned int idx = inputFilePath.find_last_of('/');

    if (idx != std::string::npos)
    {
        inputFilePath = inputFilePath.substr(0,idx+1);
    }

    std::ifstream file(inputFilePath + input.fileName + ".txt", std::ios::in);
    if (!file)
    {
        std::cout << "Looking for file " << inputFilePath + input.fileName + ".txt" << std::endl;
        std::cout<< "Unable to open the file in .fromFile method of volField_rect_2D_cpu class " << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(int i=0; i<this->GetNumberOfGridPoints(); i++)
    {
        file >> data[i];
    }
    file.close();
}

void pressureFieldSerial::SetField(const std::function< double(double,double) > func)
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

double pressureFieldSerial::Norm() const { return std::sqrt(InnerProduct(*this)); }
double pressureFieldSerial::RelNorm() const { return std::sqrt(InnerProduct(*this) / this->GetNumberOfGridPoints()); }

void pressureFieldSerial::Square()
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= data[i];
    }
}

void pressureFieldSerial::Sqrt()
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] = std::sqrt(data[i]);
    }
}

void pressureFieldSerial::Reciprocal() {
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] = double(1.0) / data[i];
    }
}

double pressureFieldSerial::Summation() const
{
    double result = double(0.0);
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        result += data[i];
    }
    return result;
}

// Non virtual members
double pressureFieldSerial::InnerProduct(const pressureFieldSerial &y) const
{
    assert(&this->GetGrid() == &y.GetGrid());
    double result = double(0.0);
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        result += data[i] * y.data[i];
    }
    return result;
}

double pressureFieldSerial::Summation(const pressureFieldSerial &rhs) const
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

void pressureFieldSerial::Gradient(pressureFieldSerial **output)
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

pressureFieldSerial& pressureFieldSerial::operator=(const pressureFieldSerial& rhs)
{
    if (this != &rhs)
    {
        assert(&this->GetGrid() == &rhs.GetGrid());
        memcpy(data, rhs.data, sizeof(double) * this->GetNumberOfGridPoints());
    }

    return *this;
}

pressureFieldSerial& pressureFieldSerial::operator=(const double rhs)
{
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] = rhs;
    }
    return *this;
}


pressureFieldSerial& pressureFieldSerial::operator-=(const pressureFieldSerial &rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] -= rhs.data[i];
    }
    return *this;
}

pressureFieldSerial& pressureFieldSerial::operator*=(const pressureFieldSerial &rhs)
{

    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= rhs.data[i];
    }
    return *this;
}

pressureFieldSerial& pressureFieldSerial::operator/=(const pressureFieldSerial &rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] /= rhs.data[i];
    }
    return *this;
}




pressureFieldSerial& pressureFieldSerial::operator-=(const double rhs)
{
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] -= rhs;
    }
    return *this;
}

pressureFieldSerial& pressureFieldSerial::operator*=(const double rhs)
{
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= rhs;
    }
    return *this;
}

pressureFieldSerial& pressureFieldSerial::operator/=(const double rhs)
{
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] /= rhs;
    }
    return *this;
}

void pressureFieldSerial::CopyTo(pressureFieldSerial &dest)
{
    dest = *this;
}

pressureFieldSerial& pressureFieldSerial::operator+=(const pressureFieldSerial &rhs)
{
    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] += rhs.data[i];
    }
    return *this;
}

pressureFieldSerial& pressureFieldSerial::operator+=(const double rhs)
{

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++)
    {
        data[i] += rhs;
    }

    return *this;

}

