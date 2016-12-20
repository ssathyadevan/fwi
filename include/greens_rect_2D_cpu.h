#ifndef GREENS_RECT_2D_CPU_H
#define GREENS_RECT_2D_CPU_H

#include <cassert>

#include <helper.h>

#include <grid_rect_2D.h>

#include <volField_rect_2D_cpu.h>
#include <volComplexField_rect_2D_cpu.h>
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "contraction.h"

template <typename T>
class Greens_rect_2D_cpu {

  std::function< std::complex<T>(T,T) > G_func;

  const grid_rect_2D<T> &grid;
  const Sources_rect_2D<T> &src;
  const Receivers_rect_2D<T> &recv;
  const T k;

  std::complex<T> *G_vol;  ////G_vol = G_xx
  std::vector< volComplexField_rect_2D_cpu<T> *> G_recv;

  Greens_rect_2D_cpu(const Greens_rect_2D_cpu<T>&) = delete;
  Greens_rect_2D_cpu& operator=(const Greens_rect_2D_cpu<T>&) = delete;

public:

  Greens_rect_2D_cpu(const grid_rect_2D<T> &grid_, const std::function< std::complex<T>(T,T) > G_func_, const Sources_rect_2D<T> &src_, const Receivers_rect_2D<T> &recv_, T k_)
  : G_func(G_func_), grid(grid_), src(src_), recv(recv_), k(k_), G_vol(), G_recv() {

    const std::array<int, 2> &nx = grid.GetGridDimensions();

    G_vol = new std::complex<T>[(2 * nx[1] - 1) * (2 * nx[0] - 1)];
    create_Greens_volume();

    create_Greens_recv();

  }

  ~Greens_rect_2D_cpu() {

    delete_Greens_recv();

    delete[] G_vol;
  }

  const std::complex<T>* GetGreensVolume() const { return G_vol; }

  const volComplexField_rect_2D_cpu<T>* GetReceiverCont(int iRecv) const {
    return G_recv[iRecv];
  }





  volComplexField_rect_2D_cpu<T> ContractWithField(const volComplexField_rect_2D_cpu<T> &x) const
  {
    // Assure we are working on the same grid
    assert(&grid == &x.GetGrid());

    volComplexField_rect_2D_cpu<T> y(grid);

    std::complex<T> *y_data = y.GetDataPtr();

    const std::array<int, 2> &nx = grid.GetGridDimensions();

    contract_Greens_rect_2D(G_vol, x.GetDataPtr(), y_data, nx, 2 * nx[0] - 1);

    return y;
  }





  static void ContractWithField(Greens_rect_2D_cpu<T> **greens, volComplexField_rect_2D_cpu<T> **y, volComplexField_rect_2D_cpu<T> **x, int nFreq, int nSrc) {

    // Assure we are working on the same grid
#ifndef NDEBUG
    for(int iFreq=0; iFreq<nFreq; iFreq++) {
      for(int iSrc=0; iSrc<nSrc; iSrc++) {
        assert(&greens[iFreq]->GetGrid() == &x[iFreq * nSrc + iSrc]->GetGrid());
        assert(&greens[iFreq]->GetGrid() == &y[iFreq * nSrc + iSrc]->GetGrid());
      }
    }
#endif

    const grid_rect_2D<T> &grid = greens[0]->GetGrid();

    const std::complex<T> **y_data = new const std::complex<T>*[nFreq * nSrc];
    std::complex<T> **x_data = new std::complex<T>*[nFreq * nSrc];
    const std::complex<T> **G_vol = new const std::complex<T>*[nFreq];

    for(int i=0; i<nFreq * nSrc; i++) {
      y_data[i] = y[i]->GetDataPtr();
      x_data[i] = x[i]->GetDataPtr();
    }

    for(int i=0; i<nFreq; i++) {
      G_vol[i] = greens[i]->GetGreensVolume();
    }

    const std::array<int, 2> &nx = grid.GetGridDimensions();

    contract_Greens_rect_2D_array(G_vol, y_data, x_data, nFreq, nSrc, nx, 2 * nx[0] - 1);

    delete[] y_data;
    delete[] x_data;

  }

  const grid_rect_2D<T>& GetGrid() const { return grid; }

private:

  void create_Greens_volume() {   ////creating G_xx weird algorithm//////

    T vol = grid.GetCellVolume();
    const std::array<int, 2> &nx = grid.GetGridDimensions();
    const std::array<T, 2> &dx = grid.GetCellDimensions();

    for(int i=-nx[1]+1; i <= nx[1]-1; i++) {
      T z = i * dx[1];
      for(int j=-nx[0]+1; j <= nx[0]-1; j++) {
        T x = j * dx[0];

        T r = dist(z, x);

        std::complex<T> val = G_func(k, r);

        G_vol[(nx[1] + i - 1) * (2 * nx[0] - 1) + (nx[0] + j - 1)] = val * vol;

      }
    }
  }

  void create_Greens_recv() {

    T vol = grid.GetCellVolume();

    volComplexField_rect_2D_cpu<T> G_bound_cpu(grid);

    for(int i=0; i < recv.nRecv; i++) {
      T x_recv = recv.xRecv[i][0];
      T z_recv = recv.xRecv[i][1];

      volComplexField_rect_2D_cpu<T> *G_bound = new volComplexField_rect_2D_cpu<T>(grid);
      G_bound->SetField(
          [this, vol, x_recv, z_recv](const T x, const T z)
          { return vol * G_func(k, dist(x - x_recv, z - z_recv)); });

      G_recv.push_back(G_bound);

    }
  }

  void delete_Greens_recv() {
    for(int i=0; i < recv.nRecv; i++) {
      delete G_recv[i];
    }
  }

};

#endif /* GREENS_RECT_2D_CPU_H */
