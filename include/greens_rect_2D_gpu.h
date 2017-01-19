#ifndef GREENS_RECT_2D_GPU_H
#define GREENS_RECT_2D_GPU_H

#include <cassert>

#include <helper.h>

#include <grid_rect_2D.h>

#include <volField_rect_2D_cpu.h>
#include <volComplexField_rect_2D_cpu.h>
#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "contraction.h"
#include "ProfileCpu.h"
#include <CL/cl.hpp>

template <typename T>
class Greens_rect_2D_gpu {

  std::function< std::complex<T>(T,T) > G_func;

  const grid_rect_2D<T> &grid;
  const Sources_rect_2D<T> &src;
  const Receivers_rect_2D<T> &recv;
  const T k;

  std::complex<T> *G_vol;  ////G_vol = G_xx
  std::vector< volComplexField_rect_2D_cpu<T> *> G_recv;
  cl_double2*  G_vol2; //G_vol2 = G_xx ankit style

  Greens_rect_2D_gpu(const Greens_rect_2D_gpu<T>&) = delete;
  Greens_rect_2D_gpu& operator=(const Greens_rect_2D_gpu<T>&) = delete;

public:

  Greens_rect_2D_gpu(const grid_rect_2D<T> &grid_, const std::function< std::complex<T>(T,T) > G_func_, const Sources_rect_2D<T> &src_, const Receivers_rect_2D<T> &recv_, T k_)
  : G_func(G_func_), grid(grid_), src(src_), recv(recv_), k(k_), G_vol(), G_recv()
  {

    const std::array<int, 2> &nx = grid.GetGridDimensions();

    G_vol = new std::complex<T>[(2 * nx[1] - 1) * (2 * nx[0] - 1)];
    create_Greens_volume();

    create_Greens_recv();
    create_Greens_volume_ankit();

  }

  ~Greens_rect_2D_gpu() {

    delete_Greens_recv();

    delete[] G_vol;

    delete[] G_vol2;
    G_vol2 = nullptr;
  }

  const std::complex<T>* GetGreensVolume() const { return G_vol; }

  const volComplexField_rect_2D_cpu<T>* GetReceiverCont(int iRecv) const {
    return G_recv[iRecv];
  }


  cl_double2* get_Gxx_Ptr() const { return G_vol2; }


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





  static void ContractWithField(Greens_rect_2D_gpu<T> **greens, volComplexField_rect_2D_cpu<T> **y, volComplexField_rect_2D_cpu<T> **x, int nFreq, int nSrc) {

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




  volComplexField_rect_2D_cpu<T> dot1(const volComplexField_rect_2D_cpu<T> &dW, const cl::CommandQueue &queue, std::vector<cl::Event> &events,
                                      cl::Kernel &kernel, cl::Kernel &k_copy, const cl::NDRange &global, const cl::NDRange &local, cl_double2 *dot, cl::Buffer buffer_dot) const
  {
      const std::array<int, 2> &nx1 = grid.GetGridDimensions();

      const int &nx = nx1[0];
      const int &nz = nx1[1];

      volComplexField_rect_2D_cpu<T> prod1(grid);
      int l1, l2, l3, l4;
      ProfileCpu profiler;

      prod1.Zero();
      std::complex<T> *p_prod = prod1.GetDataPtr();

      assert(&grid == &dW.GetGrid());
      //profiler.StartRegion("dot_product");

      cl_int ierr, sp_Gxx, sp_dW, sp_dot;
      cl::Event event_enq, event_enq2;

      for(int i=0; i < nz; i++)
      {
          l1 = i*nx;
          for (int j=0; j < nz-i; j++)
          {
              l2 = j*nx;
              l3 = l1 + l2;

              sp_Gxx = l1;
              sp_dW = l3;
              sp_dot = l2;

              ierr = kernel.setArg(5, sp_Gxx);
              if (ierr != 0)
                  std::cout << "error in setting argument 6th of kernel - error id  " << ierr << std::endl;
              ierr = kernel.setArg(6, sp_dW);
              if (ierr != 0)
                  std::cout << "error in setting argument 7th of kernel - error id  " << ierr << std::endl;
              ierr = kernel.setArg(7, sp_dot);
              if (ierr != 0)
                  std::cout << "error in setting argument 8th of kernel - error id  " << ierr << std::endl;

              queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local, &events, &event_enq);
              events.push_back(event_enq);

              if ( (2*i + j < nz) && (i>0) )
              {
                  l4 = 2*l1 + l2;

                  cl_int sp_dot1 = l4;
                  ierr = k_copy.setArg(1, sp_dot);
                  if (ierr != 0)
                      std::cout << "error in setting argument 2nd of copy kernel - error id  " << ierr << std::endl;
                  ierr = k_copy.setArg(2, sp_dot1);
                  if (ierr != 0)
                      std::cout << "error in setting argument 3rd of copy kernel - error id  " << ierr << std::endl;

                  queue.enqueueNDRangeKernel(k_copy, cl::NullRange, cl::NDRange(nx), local, &events, &event_enq2);

              }
          }
      }
      event_enq2.wait();
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

                  sp_Gxx = l1;
                  sp_dW = l2;
                  sp_dot = l3;

                  ierr = kernel.setArg(5, sp_Gxx);
                  if (ierr != 0)
                      std::cout << "error in setting argument 5th of kernel - error id  " << ierr << std::endl;
                  ierr = kernel.setArg(6, sp_dW);
                  if (ierr != 0)
                      std::cout << "error in setting argument 6th of kernel - error id  " << ierr << std::endl;
                  ierr = kernel.setArg(7, sp_dot);
                  if (ierr != 0)
                      std::cout << "error in setting argument 7th of kernel - error id  " << ierr << std::endl;

                  queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local, &events, &event_enq);
                  events.push_back(event_enq);

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

                  sp_Gxx = l1;
                  sp_dW = l2;
                  sp_dot = l3;

                  ierr = kernel.setArg(5, sp_Gxx);
                  if (ierr != 0)
                      std::cout << "error in setting argument 5th of kernel - error id  " << ierr << std::endl;
                  ierr = kernel.setArg(6, sp_dW);
                  if (ierr != 0)
                      std::cout << "error in setting argument 6th of kernel - error id  " << ierr << std::endl;
                  ierr = kernel.setArg(7, sp_dot);
                  if (ierr != 0)
                      std::cout << "error in setting argument 7th of kernel - error id  " << ierr << std::endl;

                  queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local, &events, &event_enq);
                  events.push_back(event_enq);
              }
          }
      }

      ierr = queue.enqueueReadBuffer(buffer_dot, CL_TRUE, 0, grid.GetNumberOfGridPoints() * sizeof(cl_double2), dot, &events, NULL);
      if (ierr != 0)
          std::cout << "error in reading from buffer_dot - error id  " << ierr << std::endl;

      //profiler.EndRegion();

      for(int i=0; i<grid.GetNumberOfGridPoints(); i++)
          *(p_prod + i) = std::complex<T>( (double)(dot[i].s[0]), (double)(dot[i].s[1]) );

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

      T p2_z = x_min[1] + dx[1]/T(2.0);
      G_vol2 = new cl_double2[nx*grid.GetNumberOfGridPoints()];
      for(int i=0; i < nx; i++)
      {
          T p2_x = x_min[0] + (i + T(0.5) )*dx[0];
          volComplexField_rect_2D_cpu<T> Gxx(grid);
          Gxx.SetField( [this,vol,p2_x,p2_z] (const T &x, const T &y) {return vol*G_func( k, dist(x-p2_x, y-p2_z) ); } );

          std::complex<T> *Gxx_ptr = Gxx.GetDataPtr();
          int l1 = i * grid.GetNumberOfGridPoints();

          for(int j=0; j<grid.GetNumberOfGridPoints(); j++)
          {
              G_vol2[l1+j].s[0] = (cl_double)( ( Gxx_ptr + j )->real() );
              G_vol2[l1+j].s[1] = (cl_double)( ( Gxx_ptr + j )->imag() );
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

#endif /* GREENS_RECT_2D_GPU_H */
