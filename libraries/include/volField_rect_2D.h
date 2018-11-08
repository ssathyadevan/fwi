#ifndef VOLFIELD_RECT_2D_H
#define VOLFIELD_RECT_2D_H

#include <iostream>
#include <string>
#include <functional>

#include <grid_rect_2D.h>

//Babak 2018 10 29: get rid of template for volField_rect_2D
//template <class T>
class volField_rect_2D {

protected:

    //const grid_rect_2D<T> &grid;// Babak 2018 10 29: get rid of template for grid_rect_2D
    const grid_rect_2D &grid;
    const int nGridPoints;
    //Babak 2018 10 29: get rid of template for volField_rect_2D
    //const T cellVolume;
    const double cellVolume;

    //Babak 2018 10 29: get rid of template for volField_rect_2D
    //volField_rect_2D(const volField_rect_2D<T>&) = delete;
    //volField_rect_2D<T>& operator=(const volField_rect_2D<T>&) = delete;
    volField_rect_2D(const volField_rect_2D&) = delete;
    volField_rect_2D& operator=(const volField_rect_2D&) = delete;

public:

    //volField_rect_2D(const grid_rect_2D<T> &grid_) :// Babak 2018 10 29: get rid of template for grid_rect_2D
    volField_rect_2D(const grid_rect_2D &grid_) :
        grid(grid_),
        nGridPoints(grid.GetNumberOfGridPoints()),
        cellVolume(grid.GetCellVolume()) {}

    virtual ~volField_rect_2D() {}

    //const grid_rect_2D<T> &GetGrid() const { return grid; } // Babak 2018 10 29: get rid of template for grid_rect_2D
    const grid_rect_2D &GetGrid() const { return grid; }

    int GetNumberOfGridPoints() const { return nGridPoints; }
    //Babak 2018 10 29: get rid of template for volField_rect_2D
    // T getCellVolume() const { return cellVolume; }
    double getCellVolume() const { return cellVolume; }

    virtual void Zero() = 0;
    virtual void Square() = 0;
    virtual void Sqrt() = 0;
    virtual void Random() = 0;

    // Babak 2018 10 29: get rid of template for volField_rect_2D
    // virtual void toBuffer(T *buffer) const = 0;
    // virtual void fromBuffer(const T *buffer) = 0;
    virtual void toBuffer(double *buffer) const = 0;
    virtual void fromBuffer(const double *buffer) = 0;

    virtual void toFile(const std::string &fileName) const = 0;

    // Babak 2018 10 29: get rid of template for volField_rect_2D
    //  virtual void SetField(const std::function< T(T,T) > func) = 0;
    //  virtual T Norm() const = 0;
    //  virtual T RelNorm() const = 0;
    //  virtual T Summation() const = 0;
    virtual void SetField(const std::function< double(double,double) > func) = 0;
    virtual double Norm() const = 0;
    virtual double RelNorm() const = 0;
    virtual double Summation() const = 0;


};

#endif /* VOLFIELD_RECT_2D_H */
