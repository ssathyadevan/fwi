#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#include<iostream>
#include <string>
#include "CL/cl.hpp"
#include <array>
#include <fstream>
#include <ctime>
#include <chrono>

#define CL_HPP_MINIMUM_OPENCL_VERSION 110
#define CL_HPP_TARGET_OPENCL_VERSION 110

#include<Eigen/Dense>

void checkSuccess(cl_int success, std::string error)
{
   if (success != CL_SUCCESS)
   {
     std::cout << error  << ": " << success << std::endl;
     exit(-100);
   }
}

void product( Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1, Eigen::ColMajor>& dummyVector,
              const Eigen::Matrix< std::complex<double>, 1, Eigen::Dynamic, Eigen::RowMajor>& G_vol2,
              const Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1, Eigen::ColMajor>& dW_vec,
              int nx, int l1, int l2 )
{
    dummyVector.setZero();
    std::complex<double> dummy = .0;
    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < nx - i; j++)
        {
            dummy = G_vol2[ l1 + i ] * dW_vec[ l2 + i + j];
            dummyVector[ j ] += dummy;
            if ( (2*i + j < nx) && i != 0 )
            {
                dummyVector[ 2*i + j ] += dummy;
            }
        }
    }

    for (int i = 1; i < nx; i++)
    {
        if (i <= nx-i)
        {
            for (int j = 0; j < i; j++)
            {
                dummyVector[ i + j ] += G_vol2[ l1 + i ] * dW_vec[ l2 + j];
            }
        }
        else
        {
            for (int j = 0; j < nx - i; j++)
            {
                dummyVector[ i + j ] += G_vol2[ l1 + i ] * dW_vec[ l2 + j];
            }
        }
    }
}

void performMultiplicationOnCPU(const Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1, Eigen::ColMajor> &dW_vec,
                                const Eigen::Matrix< std::complex<double>, 1, Eigen::Dynamic, Eigen::RowMajor>& G_vol2,
                                Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1, Eigen::ColMajor> & eigprod,
                                int nx, int nz)
{
    int l1, l2, l3, l4;
    eigprod.setZero();
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1, Eigen::ColMajor> dummy;
    dummy.resize(nx,1);

    for(int i=0; i < nz; i++)
    {
        l1 = i*nx;
        for (int j=0; j < nz-i; j++)
        {
            l2 = j*nx;
            l3 = l1 + l2;

            product(dummy, G_vol2, dW_vec, nx, l1, l3);
            eigprod.block(l2,0,nx,1) += dummy;

            if ( (2*i + j < nz) && (i>0) )
            {
                l4 = 2*l1 + l2;
                eigprod.block(l4,0,nx,1) += dummy;
            }
        }
    }

    for(int i=1; i < nz; i++)
    {
        if (i <= nz-i)
        {
            l1 = i*nx;
            for (int j=0; j < i; j++)
            {
                l2 = j*nx;
                l3 = l1 + l2;

                product(dummy, G_vol2, dW_vec, nx, l1, l2);
                eigprod.block(l3,0,nx,1) += dummy;
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

                product(dummy, G_vol2, dW_vec, nx, l1, l2);
                eigprod.block(l3,0,nx,1) += dummy;
            }
        }
    }
    
}

void FWIMultiplication()
{
    int nx = 256;
    int nz = 16;
    int n = nx*nz;
    cl_double2 *matA = new cl_double2[ 2*n - nz ];
    cl_double2 *vectB = new cl_double2[ n ];
    cl_double2 *vectC = new cl_double2[ n ];

    Eigen::Matrix< std::complex<double>, 1, Eigen::Dynamic, Eigen::RowMajor> G_vol2;
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1, Eigen::ColMajor> dW_vec, eigprod;

    dW_vec.resize(nx*nz,1);
    eigprod.resize(nx*nz,1);
    G_vol2.resize(1,nx*nz);

    for (int i = 0; i < n; i++)
    {
       vectB[i].s[0] = i;
       vectB[i].s[1] = i/10.;
       dW_vec[i] = std::complex<double>(i, i/10.);
    }

    for (int i = 0; i < nz; i++)
    {
        for (int j = 0; j < nx; j++)
        {
            G_vol2( i * nx + j ) = std::complex<double>(i+j, i/5.);
        }
    }

    std::clock_t c_start2 = std::clock();
    auto t_start2 = std::chrono::high_resolution_clock::now();
    performMultiplicationOnCPU( dW_vec, G_vol2, eigprod, nx, nz );
    auto t_end2 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < nz; i++ )
    {
        int d1 = ( 2*nx - 1 ) * i + nx - 1;
        matA[ d1 ].s[0] = G_vol2[ i * nx ].real();
        matA[ d1 ].s[1] = G_vol2[ i * nx ].imag();
        for (int j = 1; j < nx; j++ )
        {
            matA[ d1 + j].s[0] = G_vol2[ i * nx + j].real();
            matA[ d1 - j].s[0] = G_vol2[ i * nx + j].real();
            matA[ d1 + j].s[1] = G_vol2[ i * nx + j].imag();
            matA[ d1 - j].s[1] = G_vol2[ i * nx + j].imag();
        }
    }


    //FPGA stuff
    std::vector<cl::Platform> platformList;
    cl_int success = cl::Platform::get(&platformList);
    checkSuccess(success, "no platform found");

    std::string platformName;
    success = platformList[0].getInfo( CL_PLATFORM_NAME, &platformName);
    checkSuccess(success, "no platform found");

    if ( platformName  != "Intel(R) FPGA SDK for OpenCL(TM)")
    {
       std::cout << "ERROR: Unable to find Intel(R) FPGA OpenCL platform." << std::endl;
    }

    std::vector<cl::Device> deviceList;
    success = platformList[0].getDevices(CL_DEVICE_TYPE_ALL, &deviceList);
    checkSuccess(success, "no device found");

    std::string deviceName;
    success = deviceList[0].getInfo( CL_DEVICE_NAME, &deviceName);

    cl::Context context( deviceList, nullptr, nullptr, nullptr, &success );
    checkSuccess(success, "no contex created");

    std::ifstream inputFile( "/home/root/try/matVecKernel2.aocx", std::ios::binary );
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inputFile), {});
    unsigned char* buf = new unsigned char[buffer.size()];
    for (int i = 0; i < buffer.size(); i++)
    {
        buf[i] = buffer[i];
    }

    cl::Program::Binaries binary{ { buf, buffer.size() } };

    size_t numDevices;
    context.getInfo(CL_CONTEXT_NUM_DEVICES, &numDevices);
    std::cout << platformName << "  " << deviceName << "  " << deviceList.size() << "  " << numDevices << "  " << binary.size() << std::endl;

    cl::Program program( context, deviceList, binary, nullptr, &success );
    checkSuccess(success, "error could not create program");

    success = program.build( deviceList, nullptr, nullptr, nullptr );
    checkSuccess(success, "error could not build program");

    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();

    cl::Buffer bufMatA( context, CL_MEM_READ_ONLY, (2*n - nz) * sizeof(cl_double2), nullptr, &success );
    checkSuccess(success, "error allocating matrix A");

    cl::Buffer bufVecB( context, CL_MEM_READ_ONLY, n * sizeof(cl_double2), nullptr, &success );
    checkSuccess(success, "error allocating vector B");

    cl::Buffer bufVecC( context, CL_MEM_WRITE_ONLY, n * sizeof(cl_double2), nullptr, &success );
    checkSuccess(success, "error allocating vector C");

    cl::Kernel kernel(program, "matVecKernel2", &success);
    checkSuccess(success, "error could not create kernel");

    success = kernel.setArg( 0, bufMatA );
    checkSuccess(success, "error could not set matA");
    success = kernel.setArg( 1, bufVecB );
    checkSuccess(success, "error could not set vectB");
    success = kernel.setArg( 2, bufVecC );
    checkSuccess(success, "error could not set vectC");
    success = kernel.setArg( 3, cl::__local(nx*sizeof(cl_double2)) );
    checkSuccess(success, "error could not set 4th argument (local memory)");
    success = kernel.setArg( 4, nx );
    checkSuccess(success, "error could not set nx");
    success = kernel.setArg( 5, nz );
    checkSuccess(success, "error could not set nz");

    int blockSize = 256;
    cl::NDRange range(n * blockSize);

    cl::CommandQueue queue(context, deviceList[0], CL_QUEUE_PROFILING_ENABLE, &success);

    std::vector<cl::Event> events;
    cl::Event event;
    success = queue.enqueueWriteBuffer( bufMatA, CL_FALSE, 0, (2*n - nz) * sizeof(cl_double2), matA, nullptr, &event );
    checkSuccess(success, "error could not enque write buffer matA");
    events.push_back(event);

    success = queue.enqueueWriteBuffer( bufVecB, CL_FALSE, 0, n * sizeof(cl_double2), vectB, nullptr, &event );
    checkSuccess(success, "error could not enque write buffer vecB");
    events.push_back(event);

    success = queue.enqueueNDRangeKernel( kernel, cl::NullRange, range, cl::NDRange(blockSize), &events, &event );
    checkSuccess(success, "error could not execute kernel");
    events.clear();
    events.push_back(event);

    success = queue.enqueueReadBuffer( bufVecC, CL_TRUE, 0, n * sizeof(cl_double2), vectC, &events, nullptr );
    checkSuccess(success, "error could not copy vectC");

	
    auto t_end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++)
    {
       if ( std::abs(vectC[i].s[0] - eigprod[i].real()) > 1e-5 || std::abs(vectC[i].s[1] - eigprod[i].imag()) > 1e-5 )
       {
          std::cout << "results wrong: " << i << "  gpu: " << "(" << vectC[i].s[0] << ", " << vectC[i].s[1] << ")" << "  CPU: " << eigprod[i] << std::endl;
          std::cout << " Time taken: " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms ----- vs" << std::chrono::duration<double, std::milli>(t_end2-t_start2).count() << " ms" << std::endl;
          exit(0);
       }
    }

    std::cout << " successful computation! Time taken: " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms ----- vs" << std::chrono::duration<double, std::milli>(t_end2-t_start2).count() << " ms" << std::endl;

    delete[] matA;
    delete[] vectB;
    delete[] vectC;
    delete[] buf;
}

int main()
{
    FWIMultiplication();
}
