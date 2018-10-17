
#include <iostream>
#include <cmath>
#include <string>
#include <ctime>

#define CL_HPP_MINIMUM_OPENCL_VERSION 110
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include <CL/cl2.hpp>

#include "calcField.h"

#include "grid_rect_2D.h"

#include "volField_rect_2D_cpu.h"
#include "volComplexField_rect_2D_cpu.h"
#include "greens_rect_2D_cpu.h"
#include "greens_rect_2D_gpu.h"
#include "ProfileCpu.h"

#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "frequencies_group.h"
#include "frequencies_alternate.h"

#include "inversion.h"
#include "inversion_cpu.h"
#include "inversion_gpu.h"
#include "helper.h"

#include "tests_helper.h"
#include "mpi.h"
#include "input_parameters.h"

/* 
MELISSEN 2018-10-16
Introducing global variable (code base broad) g_ (global) verbosity
It controls the couts, because the couts in the original code are time consuming.
0 means silent (production mode: fast), 10 means verbose (diagnosis mode: slow)
In the future we can add more flavors of verbosity.
*/
const int g_verbosity = 10;

template <typename T, template<typename> class Frequencies>
int templeInversion(int nFreq, const std::string &fileName, const int &rank, const int &nop, const int& nxt);

template <typename T>
int sineInversion(int nFreq, const int& nxt);

#define REAL double


const int nItReconstructFields = 2; //number of iterations to reconstruct the image


int main(int argc, char* argv[])
{
   // if g_verbosity is 0, print everything to a file program_output.txt, else if g_verbosity = 10, print everything on screen
   if (g_verbosity == 0)
   {
	std::cout << "Printing the program output onto a file named program_output.txt" << std::endl;

	if (freopen("program_output.txt","w", stdout)) {}
   }



    const int nxt = 32;
  //const int nSrct = 17;
  //const int nFreq_Total = 20;
  //const int calc_alpha = 0;
  //const int n_max = 5;
  //const int n_iter1 = 50;
  //const int n_iter2 = 100;
  //const double tol1 = 1e-8;
  //const double tol2 = 5e-05;
  //const int do_reg = 1;
  //const int interactive = 0;
  //const double F_min1 = 10.0;
  //const double F_max1 = 40.0;
  //const int freq_dist_group = 1;
  //const int nFreq_input[] = {1};
  //const int gpu = 0;
  //const double delta_amplification_start = 100.0;
  //const double delta_amplification_slope = 10.0;
 
    
    
    MPI_Init(&argc, &argv);
    int rank, nop;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nop);

    // Ludwig
    if(rank == 0)
    {
        std::time_t start = std::time(nullptr);
        std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
    }

    //check if nx_t and nz_t are multiples of 32 required only for gpu//
    if( (gpu==1) && ( (nxt%32 != 0) || (nzt%32 != 0) ) )
    {
        std::cout << "nx_t and nz_t must be multiple of 32 to run on GPU" << std::endl;
        exit(1);

        if( (nxt>2048) && (nxt%1024!=0) )
        {
            std::cout << "If nxt > 2048, then nx_t must be a factor of 1024 to run on GPU" << std::endl;
            exit(1);
        }
    }

    int nFreq;
    if (sizeof(nFreq_input) == sizeof(int))
    {
        nFreq = nFreq_Total/nop; //distributing frequencies

        int freq_left = nFreq_Total - nFreq*nop;

        if (rank < freq_left) //distributing the left over frequencies
            nFreq += 1;
    }
    else
    {
        nFreq = nFreq_input[rank];
    }


    // Ludwig debug
    if(rank == 0)
    {
        std::cout << "nFreq= " << nFreq << std::endl;
    }

    //sine
//    int ret = sineInversion<REAL>(nFreq);

    int ret;
    //Temple
    std::string filename = "../temple.txt";
    if (freq_dist_group == 1)
    {
            ret = templeInversion<REAL,Frequencies_group>(nFreq, filename, rank, nop, nxt);
    }
    else if (freq_dist_group == 0)
    {
            ret = templeInversion<REAL,Frequencies_alternate>(nFreq, filename, rank, nop, nxt);
    }

    if(rank==0)
        std::cout << ret << std::endl;

    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
    MPI_Finalize();
    return 0;
}


//temple from here
template <typename T, template<typename> class Frequencies>
int templeInversion(int nFreq, const std::string &fileName, const int &rank, const int &nop, const int& nxt)
{
    std::array<T,2> x_min = {-300.0, 0.0}; // Rectangle in meters Comment by MELISSEN 2018 10 16 see PhD Haffinger p 53
    std::array<T,2> x_max = {300.0, 300.0};// Rectangle in meters Comment by MELISSEN see above
    std::array<int,2> ngrid = {nxt, nzt};

    grid_rect_2D<T> grid(x_min, x_max, ngrid);

    volField_rect_2D_cpu<T> chi(grid);

    if (rank==0)
        chi.fromFile(fileName);

    if ( sizeof(x_min[0]) == sizeof(double) )
        MPI_Bcast(chi.GetDataPtr(), grid.GetNumberOfGridPoints(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    else
        MPI_Bcast(chi.GetDataPtr(), grid.GetNumberOfGridPoints(), MPI_FLOAT, 0, MPI_COMM_WORLD);

    std::array<T,2> x_src_min = {-480.0, -5.0};
    std::array<T,2> x_src_max = {480.0, -5.0};

    Sources_rect_2D<T> src(x_src_min, x_src_max, nSrct);
    if (rank==0)
        src.Print();

    int nRecv = nSrct;
    Receivers_rect_2D<T> recv(src);
    if (rank==0)
        recv.Print();

    T c_0 = 2000.0; // Speed of sound through the material being studied???
    T f_min, d_freq_proc;

    //use this section for alternate frequency distribution//
    if (freq_dist_group == 0)
    {
        T d_freq = (F_max1 - F_min1)/(nFreq_Total -1);

        d_freq_proc = d_freq*nop;
        f_min = F_min1 + rank*d_freq;
    }
    //use this section for alternate frequency distribution//


    //use this section for group frequency distribution//
    else if (freq_dist_group == 1)
    {
        T d_freq = (F_max1 - F_min1)/(nFreq_Total -1);
        int freq_left = nFreq_Total - (nFreq_Total/nop)*nop;

        if (sizeof(nFreq_input) == sizeof(int))
        {
            if (rank<freq_left)
                f_min = F_min1 + d_freq*(nFreq)*rank;
            else
                f_min = F_min1 + freq_left*d_freq*(nFreq+1) + d_freq*(nFreq)*(rank-freq_left);
        }
        else
        {
            int sum = 0;
            for (int i=0; i<rank; i++)
                sum += nFreq_input[i];
            f_min = F_min1 + sum*d_freq;
        }
        d_freq_proc = d_freq;
    }
     //use this section for group frequency distribution//


    Frequencies<T> freq(f_min, d_freq_proc, nFreq, c_0);
    freq.Print();


    ProfileInterface *profiler;
    profiler = new ProfileCpu();

    std::complex<T> *p_data = new std::complex<T>[nFreq * nRecv * nSrct];

    if (rank==0)
        chi.toFile("../src/chi.txt");

    if(gpu==1)
    {
        Inversion<T, volComplexField_rect_2D_cpu, volField_rect_2D_cpu, Greens_rect_2D_gpu, Frequencies> *inverse;
        inverse = new InversionConcrete_gpu<T, volComplexField_rect_2D_cpu, volField_rect_2D_cpu, Greens_rect_2D_gpu, Frequencies>(grid, src, recv, freq, *profiler);

        if (rank==0)
            std::cout << "Creating Greens function field..." << std::endl;
        inverse->createGreens();
        inverse->SetBackground(chi);


        if (rank==0)
            std::cout << "Creating P0..." << std::endl;
        inverse->createP0();


        if (rank==0)
            std::cout << "Creating total field..." << std::endl;
        inverse->createTotalField(rank);

        inverse->calculateData(p_data);
        //complexToFile(p_data, nFreq * nRecv * nSrc, "p_data.txt");


        if (rank==0)
            std::cout << "Estimating Chi..." << std::endl;
        volField_rect_2D_cpu<T> chi_est = inverse->Reconstruct(p_data,rank);

        if (rank==0)
        {
            std::cout << "Done, writing to file" << std::endl;
            chi_est.toFile("../src/chi_est_temple.txt");
        }
    }
    else
    {
        Inversion<T, volComplexField_rect_2D_cpu, volField_rect_2D_cpu, Greens_rect_2D_cpu, Frequencies> *inverse;
        inverse = new InversionConcrete_cpu<T, volComplexField_rect_2D_cpu, volField_rect_2D_cpu, Greens_rect_2D_cpu, Frequencies>(grid, src, recv, freq, *profiler);

        if (rank==0)
            std::cout << "Creating Greens function field..." << std::endl;
        inverse->createGreens();
        inverse->SetBackground(chi);


        if (rank==0)
            std::cout << "Creating P0..." << std::endl;
        inverse->createP0();


        if (rank==0)
            std::cout << "Creating total field..." << std::endl;
        inverse->createTotalField(rank);


        inverse->calculateData(p_data);
        //complexToFile(p_data, nFreq * nRecv * nSrc, "p_data.txt");


        if (rank==0)
            std::cout << "Estimating Chi..." << std::endl;
        volField_rect_2D_cpu<T> chi_est = inverse->Reconstruct(p_data,rank);

        if (rank==0)
        {
            std::cout << "Done, writing to file" << std::endl;
            chi_est.toFile("../src/chi_est_temple.txt");
        }
    }

    delete[] p_data;



    if (rank==0)
        MakeFigure("../src/chi.txt", "../src/chi_est_temple.txt", "../src/temple_result.png", nxt, nzt, interactive);
    return 0;
}

//sine from here
template <typename T>
T chiSine(T x, T z) {
    return 0.015 * std::sin(2.0 * M_PI / 100.0 * z) * std::sin(2.0 * M_PI / 100.0 * x);
}

template <typename T>
T zeroEdge(T x, T z) {
    if((x >= -45.0) && (x <= 45.0) && (z >= -45.0) && (z <= 45.0)) {
        return 1.0;
    }
    return 0.0;
}

template <typename T>
T zeroEdgeSine(T x, T z) {
    return zeroEdge(x, z) * chiSine(x, z);
}

template <typename T>
int sineInversion(int nFreq, const int& nxt) {

    std::array<T,2> x_min = {-50.0, -50.0};
    std::array<T,2> x_max = {50.0, 50.0};
    std::array<int,2> ngrid = {nxt, nzt};

    grid_rect_2D<T> grid(x_min, x_max, ngrid);

    volField_rect_2D_cpu<T> chi(grid);
    chi.SetField(zeroEdgeSine<T>);
    chi.toFile("./chi.txt");

    /*REAL x_src_min = -150.0;
  REAL x_src_max = 150.0;
  REAL z_src_min = 75.0;
  REAL z_src_max = 75.0;
  int n_src = 21;
  Sources<REAL> src(x_src_min, x_src_max, z_src_min, z_src_max, n_src);*/

    std::array<T,2> x_src_min = {-150.0, 75.0};
    std::array<T,2> x_src_max = {150.0, 75.0};

    Sources_rect_2D<T> src(x_src_min, x_src_max, nSrct);
    src.Print();

    int nRecv = nSrct;
    Receivers_rect_2D<T> recv(src);
    recv.Print();

    T c_0 = 3000.0;
    T f_min = 15.0;
    T f_max = 30.0;
    Frequencies_group<T> freq(f_min, f_max, nFreq, c_0);
    freq.Print();

    Inversion<T, volComplexField_rect_2D_cpu, volField_rect_2D_cpu, Greens_rect_2D_cpu, Frequencies_group> *inverse;
    ProfileInterface *profiler;


    profiler = new ProfileCpu();
    inverse = new InversionConcrete_cpu<T, volField_rect_2D_cpu, volComplexField_rect_2D_cpu, Greens_rect_2D_cpu, Frequencies_group>(grid, src, recv, freq, *profiler);


    inverse->createGreens();
    inverse->SetBackground(chi);
    inverse->createP0();

    T tol = 1.0e-6;
    inverse->createTotalField(0);

    //int iFreq = 0;
    //int iSrc = 0;
    //int iRecv = 0;

    //const volComplexField<REAL> &P_0 = inverse.GetP0(iFreq, iSrc);
    //const volComplexField<REAL> &P_tot = inverse.GetTotalField(iFreq, iSrc);

    std::complex<T> *p_data = new std::complex<T>[nFreq * nRecv * nSrct];
    inverse->calculateData(p_data);
    complexToFile(p_data, nFreq * nRecv * nSrct, "./src/p_data.txt");

    volField_rect_2D_cpu<T> chi_est = inverse->Reconstruct(p_data, nItReconstructFields, tol);

    chi_est.toFile("./chi_est_sine.txt");

    delete[] p_data;

    return 0;
}


