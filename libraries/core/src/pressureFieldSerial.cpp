#include "pressureFieldSerial.h"
#include "log.h"

PressureFieldSerial::PressureFieldSerial(const Grid2D &grid) : PressureField(grid),
                                                               data(new double[this->GetNumberOfGridPoints()]) {}

PressureFieldSerial::PressureFieldSerial(const PressureFieldSerial &rhs) : PressureFieldSerial(rhs.grid)
{
    for ( int i = 0; i< GetNumberOfGridPoints(); ++i)
    {
        data[i] = rhs.data[i];
    }
}

PressureFieldSerial::~PressureFieldSerial()
{
    delete[] data;
}

// Virtual overrides
void PressureFieldSerial::Zero()
{
    for ( int i = 0; i< GetNumberOfGridPoints(); ++i)
    {
        data[i] = 0;
    }
}

void PressureFieldSerial::Random()
{
    for (int i = 0; i < this->GetGrid().GetNumberOfGridPoints(); i++)
    {
        data[i] = double(std::rand()) / double(RAND_MAX);
    }
}

void PressureFieldSerial::RandomSaurabh()
{
    for (int i = 0; i < this->GetGrid().GetNumberOfGridPoints(); i++)
    {
        int temp = rand() % 1800;
        data[i] = (double)temp / 10000;
    }
}

void PressureFieldSerial::RandomChild(const PressureFieldSerial &parent, std::default_random_engine &generator, std::normal_distribution<double> &distribution)
{
    for (int i = 0; i < this->GetGrid().GetNumberOfGridPoints(); i++)
    {
        double temp = distribution(generator);
        data[i] = parent.data[i] + temp;
        if (data[i] > 0.18 ) data[i] -= 0.18; //If larger than 0.18, loops back from 0
        if (data[i] < 0.00 ) data[i] += 0.18; //If negative, loops back from 0.18
    }
}

void PressureFieldSerial::toBuffer(double *buffer) const
{
    for ( int i = 0; i< GetNumberOfGridPoints(); ++i)
    {
        buffer[i] = data[i];
    }
}

void PressureFieldSerial::fromBuffer(const double *buffer)
{
    for ( int i = 0; i< GetNumberOfGridPoints(); ++i)
    {
        data[i] = buffer[i];
    }
}

void PressureFieldSerial::toFile(const std::string &fileName) const
{
    std::ofstream file;
    file.open(fileName, std::ios::out | std::ios::trunc);
    if (!file)
    {
        L_(lerror) << "Unable to open the file in .toFile method of volField_rect_2D_cpu class " << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        file << std::setprecision(17) << data[i] << std::endl;
    }
    file.close();
}

void PressureFieldSerial::fromFile(const GenericInput &input)
{
    std::string inputFolder = input.inputFolder;

    std::ifstream file(inputFolder + input.fileName + ".txt", std::ios::in);
    if (!file)
    {
        L_(lerror) << "Looking for file " << inputFolder + input.fileName + ".txt" ;
        L_(lerror) << "Unable to open the file in .fromFile method of volField_rect_2D_cpu class " ;
        std::exit(EXIT_FAILURE);
    }

    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        file >> data[i];
    }
    file.close();
}

void PressureFieldSerial::SetField(const std::function<double(double, double)> func)
{
    const std::array<int, 2> &nx = this->GetGrid().GetGridDimensions();
    const std::array<double, 2> &dx = this->GetGrid().GetCellDimensions();
    const std::array<double, 2> &x_min = this->GetGrid().GetGridStart();

    for (int i = 0; i < nx[1]; i++)
    {
        double z = x_min[1] + (i + double(0.5)) * dx[1];
        for (int j = 0; j < nx[0]; j++)
        {
            double x = x_min[0] + (j + double(0.5)) * dx[0];
            data[i * nx[0] + j] = func(x, z);
        }
    }
}

double PressureFieldSerial::Norm() const { return std::sqrt(InnerProduct(*this)); }
double PressureFieldSerial::RelNorm() const { return std::sqrt(InnerProduct(*this) / this->GetNumberOfGridPoints()); }

void PressureFieldSerial::Square()
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= data[i];
    }
}

void PressureFieldSerial::Sqrt()
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] = std::sqrt(data[i]);
    }
}

void PressureFieldSerial::Reciprocal()
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] = double(1.0) / data[i];
    }
}

double PressureFieldSerial::Summation() const
{
    double result = double(0.0);
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        result += data[i];
    }
    return result;
}

// Non virtual members
double PressureFieldSerial::InnerProduct(const PressureFieldSerial &rhs) const
{
    double sum = double(0.0);
    assert(this->GetGrid() == rhs.GetGrid());

    const double *rhs_data = rhs.GetDataPtr();
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        sum += data[i] * rhs_data[i];
    }
    return sum;
}

void PressureFieldSerial::Gradient(PressureFieldSerial **output) const
{
    //note that the python script has the order reversed, so gradient(c++)[0] is gradient(python)[1] and vice versa, switch for clarity?

    const std::array<int, 2> &nx = this->GetGrid().GetGridDimensions();
    const std::array<double, 2> &dx = this->GetGrid().GetCellDimensions();

    for (int i = 0; i < nx[1]; i++)
    {
        for (int j = 0; j < nx[0]; j++)
        {
            //direction 1 dx
            if (j == 0)
            {
                output[0]->data[i * nx[0] + j] = (data[i * nx[0] + j + 2] - 4 * data[i * nx[0] + j + 1] + 3 * data[i * nx[0] + j]) / (-double(2.0) * dx[0]);
            }
            else if (j == nx[0] - 1)
            {
                output[0]->data[i * nx[0] + j] = (data[i * nx[0] + j - 2] - 4 * data[i * nx[0] + j - 1] + 3 * data[i * nx[0] + j]) / (double(2.0) * dx[0]);
            }
            else
            {
                output[0]->data[i * nx[0] + j] = (data[i * nx[0] + j + 1] - data[i * nx[0] + j - 1]) / (double(2.0) * dx[0]);
            }

            //direction 2 dz
            if (i == 0)
            {
                output[1]->data[i * nx[0] + j] = (data[(i + 2) * nx[0] + j] - 4 * data[(i + 1) * nx[0] + j] + 3 * data[i * nx[0] + j]) / (-double(2.0) * dx[1]);
            }
            else if (i == nx[1] - 1)
            {
                output[1]->data[i * nx[0] + j] = (data[(i - 2) * nx[0] + j] - 4 * data[(i - 1) * nx[0] + j] + 3 * data[i * nx[0] + j]) / (double(2.0) * dx[1]);
            }
            else
            {
                output[1]->data[i * nx[0] + j] = (data[(i + 1) * nx[0] + j] - data[(i - 1) * nx[0] + j]) / (double(2.0) * dx[1]);
            }
        }
    }
}

PressureFieldSerial &PressureFieldSerial::operator=(const PressureFieldSerial &rhs)
{
    if (this != &rhs)
    {
        assert(this->GetGrid() == rhs.GetGrid());
        for ( int i = 0; i< GetNumberOfGridPoints(); ++i)
        {
            data[i] = rhs.data[i];
        }
    }

    return *this;
}

PressureFieldSerial &PressureFieldSerial::operator=(const double rhs)
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] = rhs;
    }
    return *this;
}

PressureFieldSerial &PressureFieldSerial::operator-=(const PressureFieldSerial &rhs)
{
    assert(this->GetGrid() == rhs.GetGrid());
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] -= rhs.data[i];
    }
    return *this;
}

PressureFieldSerial &PressureFieldSerial::operator*=(const PressureFieldSerial &rhs)
{

    assert(this->GetGrid() == rhs.GetGrid());
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= rhs.data[i];
    }
    return *this;
}

PressureFieldSerial &PressureFieldSerial::operator/=(const PressureFieldSerial &rhs)
{
    assert(this->GetGrid() == rhs.GetGrid());
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] /= rhs.data[i];
    }
    return *this;
}

PressureFieldSerial &PressureFieldSerial::operator-=(const double rhs)
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] -= rhs;
    }
    return *this;
}

PressureFieldSerial &PressureFieldSerial::operator*=(const double rhs)
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] *= rhs;
    }
    return *this;
}

PressureFieldSerial &PressureFieldSerial::operator/=(const double rhs)
{
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] /= rhs;
    }
    return *this;
}

void PressureFieldSerial::CopyTo(PressureFieldSerial &dest)
{
    dest = *this;
}

PressureFieldSerial &PressureFieldSerial::operator+=(const PressureFieldSerial &rhs)
{
    assert(this->GetGrid() == rhs.GetGrid());

    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] += rhs.data[i];
    }
    return *this;
}

PressureFieldSerial &PressureFieldSerial::operator+=(const double rhs)
{

    for (int i = 0; i < this->GetNumberOfGridPoints(); i++)
    {
        data[i] += rhs;
    }

    return *this;
}
