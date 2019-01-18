#ifndef GREENS_RECT_2D_CPU_H
#define GREENS_RECT_2D_CPU_H

#include <cassert>

#include "utilityFunctions.h"
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "grid_rect_2D.h"
#include "contraction.h"
#include <Eigen/Dense>
#include <volComplexField_rect_2D_cpu.h>

using namespace Eigen;

class Greens_rect_2D_cpu
{

  std::function< std::complex<double>(double,double) > G_func;

  const grid_rect_2D &grid;
  const Sources_rect_2D &src;
  const Receivers_rect_2D &recv;
  const double k;

  std::complex<double> *G_vol;
  std::vector< volComplexField_rect_2D_cpu *> G_recv;

  Matrix<std::complex<double>, Dynamic, Dynamic, RowMajor> G_vol2;

  Greens_rect_2D_cpu(const Greens_rect_2D_cpu&) = delete;
  Greens_rect_2D_cpu& operator=(const Greens_rect_2D_cpu&) = delete;

public:

  Greens_rect_2D_cpu(const grid_rect_2D &grid_,
                     const std::function< std::complex<double>(double,double) > G_func_,
                     const Sources_rect_2D &src_, const Receivers_rect_2D &recv_,
                     double k_);

  ~Greens_rect_2D_cpu();

  const std::complex<double>* GetGreensVolume() const { return G_vol; }

  const volComplexField_rect_2D_cpu* GetReceiverCont(int iRecv) const { return G_recv[iRecv];}

  volComplexField_rect_2D_cpu ContractWithField(const volComplexField_rect_2D_cpu &x) const;

  const grid_rect_2D &GetGrid() const { return grid; }

  // Babak 2018 10 25: This method generates the dot product of two matrices Greens function and contrast sources dW
  // Equation ID: "rel:buildField"

  volComplexField_rect_2D_cpu dot1(const volComplexField_rect_2D_cpu &dW) const;

private:

  void create_Greens_volume();

  void create_Greens_volume_ankit();

  void create_Greens_recv();

  void delete_Greens_recv();

};

#endif /* GREENS_RECT_2D_CPU_H */
