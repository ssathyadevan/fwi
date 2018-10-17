#ifndef CALCFIELD_GPU_H
#define CALCFIELD_GPU_H

#include <complex>
#include <vector>
#include <cassert>
#include <array>
#include "grid_rect_2D.h"
#include "input_parameters.h"
#include <eigen3/Eigen/Dense>
#include "ProfileCpu.h"
#include <string>
#include <CL/cl2.hpp>
#include <fstream>

using namespace Eigen;

template<typename T, template<typename> class volComplexField, template<typename> class volField, template<typename> class Green>
void calcField_gpu(const Green<T> &G, const volField<T> &chi, const volComplexField<T> *const *p_init, volComplexField<T> *const *p_tot, const int &rank1, const int &n_src)
{
    assert(&G.GetGrid() == &p_init[0]->GetGrid());
    const grid_rect_2D<T> &m_grid = G.GetGrid();
    volComplexField<T> chi_p(m_grid), chi_p_old(m_grid);
    volComplexField<T> dW(m_grid), f_rhs(m_grid), matA_j(m_grid);

    int n_cell = m_grid.GetNumberOfGridPoints();

    std::vector< volComplexField<T> > phi;
    Matrix< std::complex<T>, Dynamic, Dynamic, ColMajor > matA;
    Matrix< std::complex<T>, Dynamic, 1, ColMajor > b_f_rhs;
    Matrix< std::complex<T>, Dynamic, 1, ColMajor > alpha;

    std::complex<T> *rhs_data, *matA_j_data;
    ProfileCpu profiler, prof2;

    matA.conservativeResize(n_cell, 1);
    b_f_rhs.conservativeResize(n_cell, 1);

    const std::array<int,2> &nx = m_grid.GetGridDimensions();

 /*////////////////////////////////////////////////////////////GPU STUFF////////////////////////////////////////////////////////////*/
    cl_int ierr;

    //create platform//
    std::vector< cl::Platform > platform;
    ierr = cl::Platform::get(&platform);
    if (ierr!=0)
    {
        std::cout << "error in creating platform :: error id " << ierr << std::endl;
        exit(1);
    }

    //create device//
    std::vector< cl::Device > device;
    ierr = platform[0].getDevices(CL_DEVICE_TYPE_GPU, &device);
    if (ierr!=0)
    {
        std::cout << "error in finding device :: error id " << ierr << std::endl;
        exit(1);
    }

    //create context//
    cl_context_properties prop[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(platform[0])(), 0};
    cl::Context context(device, prop, NULL, NULL, &ierr);
    if (ierr!=0)
    {
        std::cout << "error in creating context, error id - " << ierr << std::endl;
        exit(1);
    }

    //creating source//
    std::string f_name = "../include/mat_vec.cl";
    std::fstream k_file;
    k_file.open(f_name);

    //assert if file is open//
    assert(k_file.is_open());

    std::string src_kernel, str_dummy;
    while(std::getline(k_file,str_dummy))
    {
        src_kernel += str_dummy;
        src_kernel.push_back('\n');
    }
    k_file.close();
    size_t k_len = src_kernel.length();

    cl::Program::Sources source;
    source.push_back({src_kernel.c_str(), k_len});

    //print the kernel//
    //std::cout << src_kernel << std::endl;

    //creating program//
    cl::Program program(context, source, &ierr);
    if (ierr!=0)
    {
        std::cout << "error in creating program, error id - " << ierr << std::endl;
        exit(1);
    }

    ierr = program.build(device, NULL, NULL, NULL);
    if (ierr!=0)
    {
        std::cout << "error in building program, error id - " << ierr << std::endl;
        exit(1);
    }

    //creating buffer for G_xx//
    cl::Buffer buffer_Gxx(context, CL_MEM_READ_WRITE, nx[0] * m_grid.GetNumberOfGridPoints() * sizeof(cl_double2), G.get_Gxx_Ptr(), &ierr);
    if (ierr != 0)
        std::cout << "error in creating buffer_Gxx - error id  " << ierr << std::endl;


    cl_double2 *const cl_dW = new cl_double2[m_grid.GetNumberOfGridPoints()];
    cl_double2 *const dot = new cl_double2[m_grid.GetNumberOfGridPoints()];


    //creating buffer for dW//
    cl::Buffer buffer_dW(context, CL_MEM_READ_WRITE, m_grid.GetNumberOfGridPoints() * sizeof(cl_double2), cl_dW, &ierr);
    if (ierr != 0)
        std::cout << "error in creating buffer_dW - error id  " << ierr << std::endl;

    //creating buffer for dot//
    cl::Buffer buffer_dot(context, CL_MEM_READ_WRITE, m_grid.GetNumberOfGridPoints() * sizeof(cl_double2), dot, &ierr);
    if (ierr != 0)
        std::cout << "error in creating buffer_dot - error id  " << ierr << std::endl;

    cl::Event event_Gxx, event_dW, event_enq;
    std::vector<cl::Event> events;

    //creating queue//
    cl_command_queue_properties prop2[] = {CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0};
    cl::CommandQueue queue(context, device[0], *prop2, &ierr);
    if (ierr!=0)
    {
        std::cout << "error in creating command queue, error id - " << ierr << std::endl;
        exit(1);
    }

    //sending G_xx/
    ierr = queue.enqueueWriteBuffer(buffer_Gxx, CL_FALSE, 0, nx[0] * m_grid.GetNumberOfGridPoints() * sizeof(cl_double2), G.get_Gxx_Ptr(), NULL, &event_Gxx);
    if (ierr != 0)
            std::cout << "error in writing to buffer_matA - error id  " << ierr << std::endl;

    //create kernel//
    std::string kernel_name = "mat_vec";

    cl::Kernel kernel(program, kernel_name.c_str(), &ierr);
    if (ierr!=0)
    {
        std::cout << "error in creating kernel, error id - " << ierr << std::endl;
        exit(1);
    }

    std::vector<size_t> block_size;
    kernel.getWorkGroupInfo(device[0], CL_KERNEL_WORK_GROUP_SIZE, &block_size);

    if (nx[0]*nx[1] < (int)(block_size[0]) ) ///if this is not true nx[0] must be a multiple of block_size[0](which is generally 1024)
    {
        block_size[0] = (size_t)(nx[0]*nx[1]);
    }

    cl::NDRange global( (size_t)(nx[0]*nx[1]*block_size[0]) );
    cl::NDRange local( (size_t)(block_size[0]) );

    //set kernel argument G_xx//
    ierr = kernel.setArg(0,buffer_Gxx);
    if (ierr != 0)
        std::cout << "error in setting argument 1st of kernel - error id  " << ierr << std::endl;

    //set kernel argument dot//
    ierr = kernel.setArg(2,buffer_dot);
    if (ierr != 0)
        std::cout << "error in setting argument 3rd of kernel - error id  " << ierr << std::endl;

    //set kernel argument nx//
    ierr = kernel.setArg(3,nx[0]);
    if (ierr != 0)
        std::cout << "error in setting argument 4th of kernel - error id  " << ierr << std::endl;

    //set kernel argument ny//
    ierr = kernel.setArg(4,nx[1]);
    if (ierr != 0)
        std::cout << "error in setting argument 5th of kernel - error id  " << ierr << std::endl;

    //set kernel argument l_sum//
    ierr = kernel.setArg( 5, cl::Local(block_size[0]*sizeof(cl_double2)) );
    if (ierr != 0)
        std::cout << "error in setting argument 9th of kernel - error id  " << ierr << std::endl;

    event_Gxx.wait();

 /*////////////////////////////////////////////////////////////GPU STUFF////////////////////////////////////////////////////////////*/




    for(int k=0; k<n_src; k++)
    {
        chi_p_old.Zero();
        T res = 0.0;

        *p_tot[k] = *p_init[k];
        std::complex<T> *data_ptr = p_tot[k]->GetDataPtr();

        int rank = rank1;
        std::string name = "full iter " + std::to_string(rank);
        int rank_print = 0;

      // if (rank1==rank_print)
      //      prof2.StartRegion(name);

        for(int it = 0; it < n_iter2; it++)
        {

            chi_p = (*p_tot[k]) * chi;

            dW = chi_p - chi_p_old;

            /*******************************************GPU*************************************************/
            //writing dW in a format which can be sent to GPU//
            const std::complex<T> *p_dW = dW.GetDataPtr();
            for(int i=0; i<m_grid.GetNumberOfGridPoints(); i++)
            {
                cl_dW[i].s[0] = (cl_double)( ( p_dW + i )->real() );
                cl_dW[i].s[1] = (cl_double)( ( p_dW + i )->imag() );
            }

            //write dW on GPU//
            ierr = queue.enqueueWriteBuffer(buffer_dW, CL_FALSE, 0, m_grid.GetNumberOfGridPoints() * sizeof(cl_double2), cl_dW, NULL, &event_dW);
            if (ierr != 0)
                    std::cout << "error in writing to buffer_matA - error id  " << ierr << std::endl;

            //set kernel argument//
            ierr = kernel.setArg(1, buffer_dW);
            if (ierr != 0)
                std::cout << "error in setting argument 2nd of kernel - error id  " << ierr << std::endl;

            events.push_back(event_dW);
            /*******************************************GPU*************************************************/

            T dWNorm = dW.Norm();
            T chi_pNorm = chi_p.Norm();

            res = dWNorm / chi_pNorm;
            //std::cout << "Residual = " << res << std::endl;

            if(res < tol2 && it != 0)
            {
	        if(rank1==rank_print){
                std::cout << "Convergence after " << it << " iterations." << " rank " << rank << std::endl;

		}
                break;
            }



            //calculate new phi
            if (calc_alpha == 1)
            {
                phi.push_back(G.ContractWithField(dW));
                f_rhs = G.ContractWithField(chi * (*p_init[k]) );

                rhs_data = f_rhs.GetDataPtr();  //calculate rhs

                matA_j = phi[it] - G.ContractWithField(chi*phi[it]);  //calculate new matrix column
                matA_j_data = matA_j.GetDataPtr();

                matA.conservativeResize(NoChange,it+1); //dynamic resizing of matrix matA
                alpha.conservativeResize(it+1);

                for(int i=0; i<n_cell; i++) //preparing the rhs and matA in the form required by the Eigen library to solve for alfa.
                {
                    matA(i,it) = matA_j_data[i];
                    b_f_rhs(i) = rhs_data[i];
                }

                alpha = matA.jacobiSvd(ComputeThinU | ComputeThinV).solve(b_f_rhs);

                (*p_tot[k]) = (*p_init[k]);
                for(int j=0; j<it+1; j++)
                    (*p_tot[k]) += alpha[j]*phi[j];
            }
            else if(calc_alpha==0)
            {
            //    profiler.StartRegion("contracting field");
                //p_tot += G.ContractWithField(dW);
                ierr = queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local, &events, &event_enq);
                if (ierr != 0)
                {
                    std::cout << "error in running kernel - error id  " << ierr << std::endl;
                    exit(1);
                }
                event_enq.wait();
                events.clear();

                ierr = queue.enqueueReadBuffer(buffer_dot, CL_TRUE, 0, nx[0]*nx[1] * sizeof(cl_double2), dot, NULL, NULL);
                if (ierr != 0)
                    std::cout << "error in reading from buffer_dot - error id  " << ierr << std::endl;

                for(int i=0; i<m_grid.GetNumberOfGridPoints(); i++)
                    data_ptr[i] += std::complex<T>( (double)(dot[i].s[0]), (double)(dot[i].s[1]) );

               // (*p_tot[k]) += G.dot1(dW, queue, events, kernel, global, local, dot, buffer_dot);
           //     profiler.EndRegion();
            }


            chi_p_old = chi_p;
            chi_p.Zero();

        }
        //if (rank1==rank_print)
        //    prof2.EndRegion();




        if((res >= tol2) && (rank1==rank_print))
        {
            std::cout << "No convergence after " <<  n_iter2 << " iterations." << " rank " << rank << "Res = " << res << std::endl;
        }

    }
    queue.finish();

}

#endif // CALCFIELD_GPU_H

