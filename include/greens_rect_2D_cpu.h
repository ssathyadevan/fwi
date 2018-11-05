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
#include "ProfileCpu.h"
#include <eigen3/Eigen/Dense>
#include "input_parameters.h"

// Babak 2018 10 29: Get rid of template in grid_rect_2D class

using namespace Eigen;

template <typename T>
class Greens_rect_2D_cpu {

  std::function< std::complex<T>(T,T) > G_func;

  //const grid_rect_2D<T> &grid; // Babak 2018 10 29: Get rid of template in grid_rect_2D class
  const grid_rect_2D &grid;
  const Sources_rect_2D<T> &src;
  const Receivers_rect_2D<T> &recv;
  const T k;

  std::complex<T> *G_vol;  ////G_vol = G_xx
  std::vector< volComplexField_rect_2D_cpu<T> *> G_recv;

  Matrix<std::complex<T>, Dynamic, Dynamic, RowMajor> G_vol2; //G_vol2 = G_xx ankit style

  Greens_rect_2D_cpu(const Greens_rect_2D_cpu<T>&) = delete;
  Greens_rect_2D_cpu& operator=(const Greens_rect_2D_cpu<T>&) = delete;

public:

//  Greens_rect_2D_cpu(const grid_rect_2D<T> &grid_, const std::function< std::complex<T>(T,T) > G_func_, const Sources_rect_2D<T> &src_, const Receivers_rect_2D<T> &recv_, T k_)
//  : G_func(G_func_), grid(grid_), src(src_), recv(recv_), k(k_), G_vol(), G_recv() // Babak 2018 10 29: Get rid of template in grid_rect_2D class
  Greens_rect_2D_cpu(const grid_rect_2D &grid_, const std::function< std::complex<T>(T,T) > G_func_, const Sources_rect_2D<T> &src_, const Receivers_rect_2D<T> &recv_, T k_)
    : G_func(G_func_), grid(grid_), src(src_), recv(recv_), k(k_), G_vol(), G_recv()

{

    const std::array<int, 2> &nx = grid.GetGridDimensions();

    G_vol = new std::complex<T>[(2 * nx[1] - 1) * (2 * nx[0] - 1)];
    create_Greens_volume();

    create_Greens_recv();
    create_Greens_volume_ankit();

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

    //const grid_rect_2D<T> &grid = greens[0]->GetGrid();// Babak 2018 10 29: Get rid of template in grid_rect_2D class
    const grid_rect_2D &grid = greens[0]->GetGRid();

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

  //const grid_rect_2D<T>& GetGrid() const { return grid; }// Babak 2018 10 29: Get rid of template in grid_rect_2D class
  const grid_rect_2D &GetGrid() const { return grid; }


  // Babak 2018 10 25: This method generates the dot product of two matrices Greens function and contrast sources dW
  // Equation ID: "rel:buildField"


  volComplexField_rect_2D_cpu<T> dot1(const volComplexField_rect_2D_cpu<T> &dW) const
  {
      const std::array<int, 2> &nx1 = grid.GetGridDimensions();

      const int &nx = nx1[0];
      const int &nz = nx1[1];

      volComplexField_rect_2D_cpu<T> prod1(grid);
      int l1, l2, l3, l4;
      ProfileCpu profiler;

      std::complex<T> *p_prod = prod1.GetDataPtr();
      const std::complex<T> *p_dW = dW.GetDataPtr();

      assert(&grid == &dW.GetGrid());
      //profiler.StartRegion("dot_product");

      Matrix< std::complex<T>, Dynamic, 1, ColMajor> dW_vec, eigprod;
      Matrix< std::complex<T>, Dynamic, 1, ColMajor> dummy;

      dW_vec.resize(nx*nz,NoChange);
      eigprod.resize(nx*nz,NoChange);
      dummy.resize(nx,NoChange);

      for(int i=0; i < nx*nz; i++)
          dW_vec(i) = p_dW[i];


      for(int i=0; i < nx*nz; i++)
          eigprod(i) = T(0.0);

      for(int i=0; i < nz; i++)
      {
          l1 = i*nx;
          for (int j=0; j < nz-i; j++)
          {
              l2 = j*nx;
              l3 = l1 + l2;

              dummy = ( G_vol2.block(0,l1,nx,nx) ) * dW_vec.block(l1+l2,0,nx,1);
              eigprod.block(l2,0,nx,1) += dummy;

              if ( (2*i + j < nz) && (i>0) )
              {
                  l4 = 2*l1 + l2;
                  eigprod.block(l4,0,nx,1) += dummy;
              }
          }
      }
      //profiler.EndRegion();

      //profiler.StartRegion("dot_product");
      for(int i=1; i < nz; i++)
      {
          if (i <= nz-i)
          {
              l1 = i*nx;
              for (int j=0; j < i; j++)
              {
                  l2 = j*nx;
                  l3 = l1 + l2;

                  eigprod.block(l3,0,nx,1) += ( G_vol2.block(0,l1,nx,nx) ) * dW_vec.block(l2,0,nx,1);
              }
          }
          else
          {
              l1 = i*nx;
              int lnz_i = nz-i;
              for (int j=0; j < lnz_i; j++)
              {
                  l2 = j*nx;
                  l3 = l1 + l2;

                  eigprod.block(l3,0,nx,1) += ( G_vol2.block(0,l1,nx,nx) ) * dW_vec.block(l2,0,nx,1);
              }
          }
      }
//      std::cout << dW_vec.cols() << "  " << dW_vec.rows() << "hello" << std::endl;
      //profiler.EndRegion();
      for(int i=0; i < nx*nz; i++)
      {
          p_prod[i] = eigprod(i);
      }
      return prod1;

  }



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


  void create_Greens_volume_ankit()
  {
      T vol = grid.GetCellVolume();
      const std::array<int, 2> &nx1 = grid.GetGridDimensions();
      const std::array<T, 2> &dx = grid.GetCellDimensions();
      const std::array<T, 2> &x_min = grid.GetGridStart();

      const int &nx = nx1[0];
      const int &nz = nx1[1];

      G_vol2.resize(nx,nx*nz);

      T p2_z = x_min[1] + dx[1]/T(2.0);
      for(int i=0; i < nx; i++)
      {
          T p2_x = x_min[0] + (i + T(0.5) )*dx[0];
          volComplexField_rect_2D_cpu<T> G_x(grid);

          G_x.SetField( [this,vol,p2_x,p2_z] (const T &x, const T &y) {return vol*G_func( k, dist(x-p2_x, y-p2_z) ); } );

          for (int j=0; j<nx*nz; j++)
              G_vol2(i,j) = *(G_x.GetDataPtr() + j);
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
