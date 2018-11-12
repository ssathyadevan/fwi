#!/usr/bin/python
# -*- coding: utf-8 -*-

# MELISSEN 2018 10 25
#
# This program is the overall "wrapper" that first calls the C++ engine of FWI
# and then calls the image maker for imagery

# We import (in Cpp we would say hashtag include) the libraries allowing access to "the computer"

import subprocess, os, sys

os.chdir('build')                                               # Go here and you find the executable
run_fwi_cpp = subprocess.Popen('./FWI')                      # Run the Cpp core of FWI. It can run independently
run_fwi_cpp.wait()                                              # Wait for that process to finish
os.chdir('../')                                                 # Go back to the original directory
run_visualizer = subprocess.call(['python', 'imageCreator_CMake.py']) # Run the visualizer from here. It can run independently
sys.exit()                                                      # Back to the terminal, we are done
