This is a step by step command to get a clean Ubuntu linux installation to work
with the fullWaveformInversion parallelization code. The following will help
you get started with the libraries needed to run the code on a CPU and not a
GPU. From the readme.txt file you see that the GPU can be turned off thus the
code can be ran using only the CPU. Furthermore it is adviced not to run the
code by a virtual box since the code runs extremely slow herein.

1. Install qtCreator:
$ sudo apt-get install qt5-default

2. Install openmp (mpich)
$ sudo apt-get install mpich

3. Install Eigen library for dense matrix population
$ sudo apt-get install libeigen3-dev

4. Install python2.7 and python-tk for gui
$ sudo apt-get install python2.7-dev
$ sudo apt-get install python-tk

You also need numpy and matplotlib from python.

5. Install openCL
$ sudo apt-get install opencl-headers
$ sudo apt-get install ocl-icd-libopencl1
$ sudo apt-get install ocl-icd-opencl-dev

6. Run ldconfig after installing libraries to configure the linkers
