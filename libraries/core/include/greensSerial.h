#pragma once

#include <cassert>

#include "utilityFunctions.h"
#include "sources.h"
#include "receivers.h"
#include "grid2D.h"
#include "contraction.h"
#include <Eigen/Dense>
#include <pressureFieldComplexSerial.h>

using namespace Eigen;

class Greens_rect_2D_cpu
{
public:

  Greens_rect_2D_cpu(const Grid2D &grid_,
                     const std::function< std::complex<double>(double,double) > G_func_,
                     const Sources &src_, const Receivers &recv_,
                     double k_);

  ~Greens_rect_2D_cpu();

  const std::complex<double>* GetGreensVolume() const { return G_vol; }

  const PressureFieldComplexSerial* GetReceiverCont(int iRecv) const { return G_recv[iRecv];}

  PressureFieldComplexSerial ContractWithField(const PressureFieldComplexSerial &x) const;

  const Grid2D &GetGrid() const { return grid; }

  // Babak 2018 10 25: This method generates the dot product of two matrices Greens function and contrast sources dW
  // Equation ID: "rel:buildField"

  PressureFieldComplexSerial dot1(const PressureFieldComplexSerial &dW) const;

private:

  void create_Greens_volume();

  void create_Greens_volume_ankit();

  void create_Greens_recv();

  void delete_Greens_recv();

  std::function< std::complex<double>(double,double) > G_func;

  const Grid2D &grid;
  const Sources &src;
  const Receivers &recv;
  const double k;

  std::complex<double> *G_vol;
  std::vector< PressureFieldComplexSerial *> G_recv;

  Matrix<std::complex<double>, Dynamic, Dynamic, RowMajor> G_vol2;

  Greens_rect_2D_cpu(const Greens_rect_2D_cpu&) = delete;
  Greens_rect_2D_cpu& operator=(const Greens_rect_2D_cpu&) = delete;

};

