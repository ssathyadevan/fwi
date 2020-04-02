#include "dataGrid2D.h"
#include "log.h"

dataGrid2D::dataGrid2D(const grid2D &grid_) : _grid(grid_), _data(std::vector<double>(getNumberOfGridPoints(), 0.0)) {}

dataGrid2D::dataGrid2D(const dataGrid2D &rhs) : dataGrid2D(rhs._grid)
{
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = rhs._data[i];
    }
}

void dataGrid2D::zero()
{
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = 0;
    }
}

void dataGrid2D::square()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= _data[i];
    }
}

void dataGrid2D::sqrt()
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = std::sqrt(_data[i]);
    }
}

void dataGrid2D::reciprocal()
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

double dataGrid2D::summation() const
{
    double result = 0.0;
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        result += _data[i];
    }
    return result;
}

double dataGrid2D::innerProduct(const dataGrid2D &rhs) const
{
    assert(GetGrid() == rhs.GetGrid());

    double sum = 0.0;
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        sum += _data[i] * rhsData[i];
    }
    return sum;
}

void dataGrid2D::gradient(std::vector<dataGrid2D> &gradientField) const
{
    const std::array<int, 2> &nx = GetGrid().getGridDimensions();
    const std::array<double, 2> &dx = GetGrid().getCellDimensions();

    for(int i = 0; i < nx[1]; i++)
    {
        for(int j = 0; j < nx[0]; j++)
        {
            int index = i * nx[0] + j;

            // direction 1 dx
            double gradientDx;
            if(j == 0)
            {
                gradientDx = (_data[i * nx[0] + j + 2] - 4 * _data[i * nx[0] + j + 1] + 3 * _data[i * nx[0] + j]) / (-2.0 * dx[0]);
            }
            else if(j == nx[0] - 1)
            {
                gradientDx = (_data[i * nx[0] + j - 2] - 4 * _data[i * nx[0] + j - 1] + 3 * _data[i * nx[0] + j]) / (2.0 * dx[0]);
            }
            else
            {
                gradientDx = (_data[i * nx[0] + j + 1] - _data[i * nx[0] + j - 1]) / (2.0 * dx[0]);
            }
            gradientField[0].setValueAtIndex(gradientDx, index);

            // direction 2 dz
            double gradientDz;
            if(i == 0)
            {
                gradientDz = (_data[(i + 2) * nx[0] + j] - 4 * _data[(i + 1) * nx[0] + j] + 3 * _data[i * nx[0] + j]) / (-2.0 * dx[1]);
            }
            else if(i == nx[1] - 1)
            {
                gradientDz = (_data[(i - 2) * nx[0] + j] - 4 * _data[(i - 1) * nx[0] + j] + 3 * _data[i * nx[0] + j]) / (2.0 * dx[1]);
            }
            else
            {
                gradientDz = (_data[(i + 1) * nx[0] + j] - _data[(i - 1) * nx[0] + j]) / (2.0 * dx[1]);
            }
            gradientField[1].setValueAtIndex(gradientDz, index);
        }
    }
}

void dataGrid2D::toFile(const std::string &fileName) const
{
    std::ofstream file;
    file.open(fileName, std::ios::out | std::ios::trunc);
    if(!file)
    {
        L_(lerror) << "Unable to open the file in .toFile method of volField_rect_2D_cpu class " << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        file << std::setprecision(17) << _data[i] << std::endl;
    }
    file.close();
}

void dataGrid2D::fromFile(const genericInput &input)
{
    std::string inputFolder = input.inputFolder;

    std::ifstream file(inputFolder + input.fileName + ".txt", std::ios::in);
    if(!file)
    {
        L_(lerror) << "Looking for file " << inputFolder + input.fileName + ".txt";
        L_(lerror) << "Unable to open the file in .fromFile method of volField_rect_2D_cpu class ";
        std::exit(EXIT_FAILURE);
    }

    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        file >> _data[i];
    }
    file.close();
}

void dataGrid2D::random()
{
    for(int i = 0; i < GetGrid().getNumberOfGridPoints(); i++)
    {
        _data[i] = double(std::rand()) / double(RAND_MAX);
    }
}

void dataGrid2D::randomSaurabh()
{
    for(int i = 0; i < GetGrid().getNumberOfGridPoints(); i++)
    {
        int temp = rand() % 1800;
        _data[i] = (double)temp / 10000;
    }
}

void dataGrid2D::randomChild(const dataGrid2D &parent, std::default_random_engine &generator, std::normal_distribution<double> &distribution)
{
    for(int i = 0; i < GetGrid().getNumberOfGridPoints(); i++)
    {
        double temp = distribution(generator);
        _data[i] = parent._data[i] + temp;
        if(_data[i] > 0.18)
        {
            _data[i] -= 0.18;   // If larger than 0.18, loops back from 0
        }
        if(_data[i] < 0.00)
        {
            _data[i] += 0.18;   // If negative, loops back from 0.18
        }
    }
}

// Operators
dataGrid2D &dataGrid2D::operator=(const dataGrid2D &rhs)
{
    if(this == &rhs)
    {
        throw std::logic_error("Assign operator with itself");
    }

    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); ++i)
    {
        _data[i] = rhsData[i];
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = data[i];
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] = value;
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator+=(const dataGrid2D &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += rhsData[i];
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator+=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += data[i];
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator+=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] += value;
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator-=(const dataGrid2D &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= rhsData[i];
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator-=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= data[i];
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator-=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] -= value;
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator*=(const dataGrid2D &rhs)
{
    assert(GetGrid() == rhs.GetGrid());
    const std::vector<double> &rhsData = rhs.getData();
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= rhsData[i];
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator*=(const std::vector<double> &data)
{
    assert(getNumberOfGridPoints() == (int)data.size());
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= data[i];
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator*=(const double value)
{
    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] *= value;
    }
    return *this;
}

dataGrid2D &dataGrid2D::operator/=(const dataGrid2D &rhs)
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

dataGrid2D &dataGrid2D::operator/=(const std::vector<double> &data)
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

dataGrid2D &dataGrid2D::operator/=(const double value)
{
    if(value == 0.0)
    {
        throw std::overflow_error("Operator devides by zero");
    }

    for(int i = 0; i < getNumberOfGridPoints(); i++)
    {
        _data[i] /= value;
    }
    return *this;
}
