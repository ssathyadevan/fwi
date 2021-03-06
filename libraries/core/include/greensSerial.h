#ifndef GREENS_RECT_2D_CPU_H
#define GREENS_RECT_2D_CPU_H

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

  Greens_rect_2D_cpu(const grid2D &grid_,
                     const std::function< std::complex<double>(double,double) > G_func_,
                     const sources &src_, const receivers &recv_,
                     double k_);

  ~Greens_rect_2D_cpu();

  const std::complex<double>* GetGreensVolume() const { return G_vol; }

  const pressureFieldComplexSerial* GetReceiverCont(int iRecv) const { return G_recv[iRecv];}

  pressureFieldComplexSerial ContractWithField(const pressureFieldComplexSerial &x) const;

  const grid2D &GetGrid() const { return grid; }

  // Babak 2018 10 25: This method generates the dot product of two matrices Greens function and contrast sources dW
  // Equation ID: "rel:buildField"

  pressureFieldComplexSerial dot1(const pressureFieldComplexSerial &dW) const;

private:

  void create_Greens_volume();

  void create_Greens_volume_ankit();

  void create_Greens_recv();

  void delete_Greens_recv();

  void checkSuccess(int success, std::string error) const;

  bool acceleratedDot( const Matrix<std::complex<double>, 1, Dynamic, RowMajor>& G_vol2, const std::complex<double> *dW, std::complex<double> *p_prod ) const;

  std::function< std::complex<double>(double,double) > G_func;

  const grid2D &grid;
  const sources &src;
  const receivers &recv;
  const double k;

  std::complex<double> *G_vol;
  std::vector< pressureFieldComplexSerial *> G_recv;

  Matrix<std::complex<double>, 1, Dynamic, RowMajor> G_vol2;

  Greens_rect_2D_cpu(const Greens_rect_2D_cpu&) = delete;
  Greens_rect_2D_cpu& operator=(const Greens_rect_2D_cpu&) = delete;

  void product(Matrix< std::complex<double>, Dynamic, 1, ColMajor>& dummy,
               const Matrix<std::complex<double>, 1, Dynamic, RowMajor>& G_line,
               const Matrix< std::complex<double>, Dynamic, 1, ColMajor>& dW_vec,
               int nx, int l1, int l2) const;
};

#endif /* GREENS_RECT_2D_CPU_H */
