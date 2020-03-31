#pragma once

#include <array>

class Grid2D
{
private:
    const std::array<double, 2> _xMin;
    const std::array<double, 2> _xMax;
    const std::array<int, 2> _nx;
    std::array<double, 2> _dx;

    int _nGridPoints;
    double _cellVolume;

    Grid2D &operator=(const Grid2D &) = delete;

public:
    Grid2D(const std::array<double, 2> &x_min_, const std::array<double, 2> &x_max_, const std::array<int, 2> &nx_);

    const std::array<int, 2> &GetGridDimensions() const { return _nx; }
    const std::array<double, 2> &GetCellDimensions() const { return _dx; }
    const std::array<double, 2> &GetGridStart() const { return _xMin; }
    const std::array<double, 2> &GetGridEnd() const { return _xMax; }

    int GetNumberOfGridPoints() const { return _nGridPoints; }
    double GetCellVolume() const { return _cellVolume; }
    double GetDomainArea() const { return (_xMax[0] - _xMin[0]) * (_xMax[1] - _xMin[1]); }

    bool operator==(const Grid2D &rhs) const;
};
