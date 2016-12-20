#ifndef TESTS_HELPER_H
#define TESTS_HELPER_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <ctime>
#include <complex>


#ifndef D_NO_CUDA
#define CUDA_CHECK_RETURN(value) {                        \
  cudaError_t _m_cudaStat = value;                        \
  if (_m_cudaStat != cudaSuccess) {                       \
    fprintf(stderr, "Error %s at line %d in file %s\n",   \
    cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__); \
    exit(1);                                              \
  }                                                       \
}
#endif

template <typename T>
void fillRandom(std::complex<T> *buffer, int n) {

  std::srand(std::time(0));

  for(int i=0; i<n; i++) {
    T x = -1.0 + 2.0 * double(std::rand()) / double(RAND_MAX);
    T y = -1.0 + 2.0 * double(std::rand()) / double(RAND_MAX);
    buffer[i] = std::complex<T>(x,y);
  }

}

template <typename T>
T norm(const std::complex<T> *x, int nx, int nz) {

  T result = 0.0;

  for(int i=0; i<nz; i++) {
    for(int j=0; j<nx; j++) {
      result += std::norm(x[i * nx + j]);
    }
  }

  return std::sqrt(result);

}

template <typename T>
T norm(const std::complex<T> *x, int n) {

  T result = 0.0;

  for(int i=0; i<n; i++) {
    result += std::norm(x[i]);
  }

  return std::sqrt(result);

}

template <typename T>
T diff(const std::complex<T> *x, const std::complex<T> *y, int nx, int nz) {

  T result = 0.0;

  for(int i=0; i<nz; i++) {
    for(int j=0; j<nx; j++) {
      result += std::norm(x[i * nx + j] - y[i * nx + j]);
    }
  }

  return std::sqrt(result);

}

template <typename T>
T diff(const std::complex<T> *x, const std::complex<T> *y, int n) {

  T result = 0.0;

  for(int i=0; i<n; i++) {
  result += std::norm(x[i] - y[i]);
  }

  return std::sqrt(result);

}

template <typename T>
void complexToFile(std::complex<T> *buffer, int n, const char *fileName) {
      std::ofstream file;
      file.open (fileName, std::ios::out | std::ios::trunc);
      for(int i=0; i<n; i++) {
        file << std::showpos << std::setprecision(17) << "(" << std::real(buffer[i]) << std::imag(buffer[i]) << "j)" << std::endl;
      }
      file.close();
}

#endif // TESTS_HELPER_H
