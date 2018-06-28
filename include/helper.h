#ifndef HELPER_H
#define HELPER_H

#include <cmath>
#include <iostream>
#include <string>
#include <python2.7/Python.h>

template <typename T>
inline T dist(T x, T z) {
  return std::pow(std::pow(z, 2.0) + std::pow(x, 2.0), 0.5);
}

inline void MakeFigure(const std::string &file1, const std::string &file2, const std::string &outputfile, const int nx, const int nz, const int interactive) {

    std::string pythonfile = "../src/ShowChi.py";
    int nargs = 7;
    FILE* file;
    char * argv[nargs];

    argv[0] = strdup(pythonfile.c_str());

    // file name of chi input file
    argv[1] = strdup(file1.c_str());

    // file name of the reconstructed chi file
    argv[2] = strdup(file2.c_str());

    // output image file name
    argv[3] = strdup(outputfile.c_str());

    argv[4] = strdup(std::to_string(nx).c_str());
    argv[5] = strdup(std::to_string(nz).c_str());

    // interactive mode, 0 to not show the figure, 1 to show the figu0=;cgptre and block further execution
    argv[6] = strdup(std::to_string(interactive).c_str());


    Py_Initialize();
    PySys_SetArgv(nargs, argv);
    file = fopen(argv[0], "r");
    PyRun_SimpleFile(file, argv[0]);
    Py_Finalize();
}

#endif
