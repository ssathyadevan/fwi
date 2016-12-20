#ifndef VOLFIELD_RECT_2D_CPU_H
#define VOLFIELD_RECT_2D_CPU_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <array>
#include <functional>
#include <cassert>
#include <string.h>

#include <volField_rect_2D.h>

template <class T>
class volField_rect_2D_cpu : public volField_rect_2D<T> {

  T * const data;

public:

  volField_rect_2D_cpu(const grid_rect_2D<T> &grid) :
    volField_rect_2D<T>(grid),
    data(new T[this->GetNumberOfGridPoints()]) {}

  volField_rect_2D_cpu(const volField_rect_2D_cpu<T> &rhs) :
    volField_rect_2D_cpu(rhs.grid) {

    memcpy(data, rhs.data, sizeof(T) * this->GetNumberOfGridPoints());

  }

  virtual ~volField_rect_2D_cpu() {
    delete[] data;
  }

  // Virtual overrides
  virtual void Zero() {
    memset(data, 0, sizeof(T) * this->GetNumberOfGridPoints());
  }

  virtual void Random() {
    for(int i=0; i<this->GetGrid().GetNumberOfGridPoints(); i++) {
      data[i] = T(std::rand()) / T(RAND_MAX);
    }
  }

  virtual void toBuffer(T *buffer) const {
    memcpy(buffer, data, sizeof(T) * this->GetNumberOfGridPoints());
  }

  virtual void fromBuffer(const T *buffer) {
    memcpy(data, buffer, sizeof(T) * this->GetNumberOfGridPoints());
  }

  virtual void toFile(const std::string &fileName) const {

    std::ofstream file;
    file.open (fileName, std::ios::out | std::ios::trunc);
    assert(file.is_open());

    for(int i=0; i<this->GetNumberOfGridPoints(); i++) {
      file << std::setprecision(17) << data[i] << std::endl;
    }
    file.close();

  }

  virtual void fromFile(const std::string &fileName) {

    std::ifstream file(fileName, std::ios::in);
    assert(file.is_open());

    for(int i=0; i<this->GetNumberOfGridPoints(); i++) {
      file >> data[i];
    }
    file.close();

  }

  virtual void SetField(const std::function< T(T,T) > func) {

    const std::array<int, 2> &nx = this->GetGrid().GetGridDimensions();
    const std::array<T, 2> &dx = this->GetGrid().GetCellDimensions();
    const std::array<T, 2> &x_min = this->GetGrid().GetGridStart();

    for(int i=0; i<nx[1]; i++) {
      T z = x_min[1] + (i + T(0.5)) * dx[1];
      for(int j=0; j<nx[0]; j++) {
        T x = x_min[0] + (j + T(0.5)) * dx[0];
        data[i * nx[0] + j] = func(x,z);
      }
    }
  }

  virtual T Norm() const { return std::sqrt(InnerProduct(*this)); }
  virtual T RelNorm() const { return std::sqrt(InnerProduct(*this) / this->GetNumberOfGridPoints()); }

  virtual void Square() {
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++) {
      data[i] *= data[i];
    }
  }

  virtual void Reciprocal() {
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++) {
      data[i] = T(1.0) / data[i];
    }
  }

  // Non virtual members
  T InnerProduct(const volField_rect_2D_cpu<T> &y) const {

    assert(&this->GetGrid() == &y.GetGrid());

    T result = T(0.0);
    for (int i = 0; i < this->GetNumberOfGridPoints(); i++) {
      result += data[i] * y.data[i];
    }
    return result;
  }

  volField_rect_2D_cpu<T>& operator=(const volField_rect_2D_cpu<T>& rhs) {

    if (this != &rhs) {
      assert(&this->GetGrid() == &rhs.GetGrid());
      memcpy(data, rhs.data, sizeof(T) * this->GetNumberOfGridPoints());
    }

    return *this;

  }

  volField_rect_2D_cpu<T>& operator=(const T rhs) {
    for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
      data[i] = rhs;
    }
    return *this;
  }

  volField_rect_2D_cpu<T>& operator+=(const volField_rect_2D_cpu<T> &rhs)  {

    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
      data[i] += rhs.data[i];
    }

    return *this;

  }

  volField_rect_2D_cpu<T>& operator-=(const volField_rect_2D_cpu<T> &rhs)  {

    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
      data[i] -= rhs.data[i];
    }

    return *this;

  }

  volField_rect_2D_cpu<T>& operator*=(const volField_rect_2D_cpu<T> &rhs)  {

    assert(&this->GetGrid() == &rhs.GetGrid());

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
      data[i] *= rhs.data[i];
    }

    return *this;

  }

  volField_rect_2D_cpu<T>& operator+=(const T rhs)  {

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
      data[i] += rhs;
    }

    return *this;

  }

  volField_rect_2D_cpu<T>& operator-=(const T rhs)  {

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
      data[i] -= rhs;
    }

    return *this;

  }

  volField_rect_2D_cpu<T>& operator*=(const T rhs)  {

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
      data[i] *= rhs;
    }

    return *this;

  }

  volField_rect_2D_cpu<T>& operator/=(const T rhs)  {

    for (int i = 0; i<this->GetNumberOfGridPoints(); i++) {
      data[i] /= rhs;
    }

    return *this;

  }

  const T *GetDataPtr() const { return data; }
  T *GetDataPtr() { return data; }

  void CopyTo(volField_rect_2D_cpu<T> &dest) {
    dest = *this;
  }

};

template <class T>
T InnerProduct(const volField_rect_2D_cpu<T> &x, const volField_rect_2D_cpu<T> &y) {
  return x.InnerProduct(y);
}

template <class T>
volField_rect_2D_cpu<T> operator+(const volField_rect_2D_cpu<T> &x, const volField_rect_2D_cpu<T> &y) {
  volField_rect_2D_cpu<T> result(x);
  result += y;
  return result;
}

template <class T>
volField_rect_2D_cpu<T> operator-(const volField_rect_2D_cpu<T> &x, const volField_rect_2D_cpu<T> &y) {
  volField_rect_2D_cpu<T> result(x);
  result -= y;
  return result;
}

template <class T>
volField_rect_2D_cpu<T> operator*(const volField_rect_2D_cpu<T> &x, const volField_rect_2D_cpu<T> &y) {
  volField_rect_2D_cpu<T> result(x);
  result *= y;
  return result;
}

template <class T>
volField_rect_2D_cpu<T> operator*(const T x, const volField_rect_2D_cpu<T> &y) {
  volField_rect_2D_cpu<T> result(y);
  result *= x;
  return result;
}


#endif /* VOLFIELD_RECT_2D_CPU_H */
