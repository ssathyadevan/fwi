
#include <iostream>
#include <cmath>
#include <string>
#include <ctime>

#include "calcField.h"

#include "grid_rect_2D.h"

#include "volField_rect_2D_cpu.h"
#include "volComplexField_rect_2D_cpu.h"
#include "greens_rect_2D_cpu.h"
#include "ProfileCpu.h"

#include "sources_rect_2D.h"
#include "receivers_rect_2D.h"
#include "frequencies.h"
#include "inversion.h"
#include "helper.h"

#include "tests_helper.h"


template <typename T>
int templeInversion(int nx, int nz, int nSrc, int nFreq, const std::string &fileName);

template <typename T>
int sineInversion(int nx, int nz, int nSrc, int nFreq);

#define REAL double

const int nItCreateFields = 30;
const int nItReconstructFields = 2; //number of iterations to reconstruct the image


int main()
{

    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
    const int nx = 64;
    const int nz = 32;

    int nFreq = 10;

    int nSrc = 17;


    //sine
//    int ret = sineInversion<REAL>(nx, nz, nSrc, nFreq);



    //Temple
    std::string filename = "./temple.txt";
    int ret = templeInversion<REAL>(nx, nz, nSrc, nFreq, filename);



    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
    return 0;
}


//temple from here
template <typename T>
int templeInversion(int nx, int nz, int nSrc, int nFreq, const std::string &fileName) {

    std::array<T,2> x_min = {-300.0, 0.0};
    std::array<T,2> x_max = {300.0, 300.0};
    std::array<int,2> ngrid = {nx, nz};

    grid_rect_2D<T> grid(x_min, x_max, ngrid);

    volField_rect_2D_cpu<T> chi(grid);
    chi.fromFile(fileName);

    std::array<T,2> x_src_min = {-480.0, -5.0};
    std::array<T,2> x_src_max = {480.0, -5.0};

    Sources_rect_2D<T> src(x_src_min, x_src_max, nSrc);
    src.Print();

    int nRecv = nSrc;
    Receivers_rect_2D<T> recv(src);
    recv.Print();

    T c_0 = 2000.0;
    T f_min = 10.0;
    T f_max = 40.0;
    Frequencies<T> freq(f_min, f_max, nFreq, c_0);
    freq.Print();

    Inversion<T> *inverse;
    ProfileInterface *profiler;


    profiler = new ProfileCpu();
    inverse = new InversionConcrete<T, volComplexField_rect_2D_cpu, volField_rect_2D_cpu, Greens_rect_2D_cpu>(grid, src, recv, freq, *profiler);


    chi.toFile("./src/chi.txt");

    std::cout << "Creating Greens function field..." << std::endl;
    inverse->createGreens();
    inverse->SetBackground(chi);
    std::cout << "Creating P0..." << std::endl;
    inverse->createP0();

    std::cout << "Creating total field..." << std::endl;
    inverse->createTotalField();

    std::complex<T> *p_data = new std::complex<T>[nFreq * nRecv * nSrc];
    inverse->calculateData(p_data);
    //complexToFile(p_data, nFreq * nRecv * nSrc, "p_data.txt");

    std::cout << "Estimating Chi..." << std::endl;
    volField_rect_2D_cpu<T> chi_est = inverse->Reconstruct(p_data);
    std::cout << "Done, writing to file" << std::endl;
    chi_est.toFile("./src/chi_est_temple.txt");

    delete[] p_data;

    MakeFigure("./src/chi.txt", "./src/chi_est_temple.txt", "./src/temple_result.png", nx, nz, interactive);

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
int sineInversion(int nx, int nz, int nSrc, int nFreq) {

    std::array<T,2> x_min = {-50.0, -50.0};
    std::array<T,2> x_max = {50.0, 50.0};
    std::array<int,2> ngrid = {nx, nz};

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

    Sources_rect_2D<T> src(x_src_min, x_src_max, nSrc);
    src.Print();

    int nRecv = nSrc;
    Receivers_rect_2D<T> recv(src);
    recv.Print();

    T c_0 = 3000.0;
    T f_min = 15.0;
    T f_max = 30.0;
    Frequencies<T> freq(f_min, f_max, nFreq, c_0);
    freq.Print();

    Inversion<T> *inverse;
    ProfileInterface *profiler;


    profiler = new ProfileCpu();
    inverse = new InversionConcrete<T, volField_rect_2D_cpu, volComplexField_rect_2D_cpu, Greens_rect_2D_cpu>(grid, src, recv, freq, *profiler);


    inverse->createGreens();
    inverse->SetBackground(chi);
    inverse->createP0();

    T tol = 1.0e-6;
    inverse->createTotalField(nItCreateFields, tol);

    //int iFreq = 0;
    //int iSrc = 0;
    //int iRecv = 0;

    //const volComplexField<REAL> &P_0 = inverse.GetP0(iFreq, iSrc);
    //const volComplexField<REAL> &P_tot = inverse.GetTotalField(iFreq, iSrc);

    std::complex<T> *p_data = new std::complex<T>[nFreq * nRecv * nSrc];
    inverse->calculateData(p_data);
    complexToFile(p_data, nFreq * nRecv * nSrc, "./src/p_data.txt");

    volField_rect_2D_cpu<T> chi_est = inverse->Reconstruct(p_data, nItReconstructFields, tol);

    chi_est.toFile("./chi_est_sine.txt");

    delete[] p_data;

    return 0;
}


