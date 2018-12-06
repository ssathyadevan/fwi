#!/usr/bin/python
# -*- coding: utf-8 -*-

# MELISSEN 2018 10 25
# Modified according to new architecture, Saurabh 2018 11 20
#
# This program is the overall "wrapper" that first calls the C++ engine of FWI
# and then calls the image maker for imagery

# We import (in Cpp we would say hashtag include) the libraries allowing access to "the computer"

import subprocess, os, sys
os.chdir('../build/applications/preProcessing')                    	# Go here and you find the preProcessing executable
run_fwi_cpp = subprocess.Popen('./FWI_PreProcess')              	# Run the preProcessing executable
run_fwi_cpp.wait()                                              	# Wait for that process to finish

os.chdir('../processing')                                               # Go to the processing directory
run_fwi_cpp = subprocess.Popen('./FWI_Process')                         # Run the Processing executable
run_fwi_cpp.wait()                                                      # Wait for that process to finish

os.chdir('../../../parallelized-fwi/')					# Go to the home directory
run_visualizer = subprocess.call(['python', 'imageCreator_CMake.py']) 	# Run the image visualizer from here. It can run independently
sys.exit()                                                      	# Back to the terminal, we are done
