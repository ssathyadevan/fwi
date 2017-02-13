Steps to run the code.

1. Create a QT project (with the given try_qt.pro) file.
2. Make sure that build-try_qt-Desktop-Release and build-try_qt-Desktop-Debug folders are in the same folder as the main.cpp (change QT settings for this).
3. Open a terminal and navigate to the GUI folder and type "python gui.py".
4. GUI will be opened, modify the GUI to run the simulation.
5. If get compilation error due to library linking or file not found, change the Paths in try_qt.pro to the Paths which points to files/libraries on your computer.

NOTE: if running on a remote computer (which has display problems)
1. Clone this git repository on local computer.
2. Follow above mentioned steps from 1-5.
3. Clone git repository on the remote computer.
4. Copy Makefile from build-try_qt-Desktop-Release/build-try_qt-Desktop-Debug folder on local computer to build-try_qt-Desktop-Release/build-try_qt-Desktop-Debug folder on remote computer.
5. Try builing the application through GUI.
6. If get compiling errors, change paths in Makefile to the Paths which points to files/libraries on remote computer.

TIP: instead of using librarypath for OpenCL just type -lOpenCL.



Progress of the project:

1. Initially, the mathematical model in the Python code was fixed so as to achieve correct results.
2. Then we moved on to the C++ code, and modified the data structure of G_xx (Greens function). This data structure was much lighter (in memory) than the one implemented in Python but is heavier than the one implemented by Michiel. But, since the new data structure is faster we currently it (although Michiel's original data structure is also available).
3. Further, we parallelized the code for clusters (using openMPI) and considerable speedup is achieved by it (Nearly equal to the number of processors). The parallelization is done by splitting frequencies on different processors and there are three ways available in the code in which the frequencies can be distributed.
4. Finally, the matrix vector multiplication in CalcField.h (used to find p_tot) was ported to the GPU.

It is interesting to see that CalcField with GPU on a single processor gives the best performance. CPU parallelized code porting CalcField Matrix vector multiplication to GPU is a little slower (for bigger grid sizes). This is due to the overloading of GPU as multiple CPUs port calculations to it. But this can be really efficient if we have multiple GPU cards available on the cluster. 

To give a perspective of speedup, the big temple for 20 frequencies takes around 5 mins instead of a full night taken by the Python code.


