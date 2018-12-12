#!/usr/bin/python
# -*- coding: utf-8 -*-

# MELISSEN 2018 10 25
# Modified according to new architecture, Saurabh 2018 11 20
#
# This program is the overall "wrapper" that first calls the C++ engine of FWI
# and then calls the image maker for imagery

# We import (in Cpp we would say hashtag include) the libraries allowing access to "the computer"

import subprocess, os, sys
os.chdir('bin')                    									# Go here and you find the preProcessing executable
run_fwi_cpp = subprocess.Popen('./FWI_PreProcess ../inputFiles/ ../Output/ default', shell = True)              	# Run the preProcessing executable
run_fwi_cpp.wait()                                              					# Wait for that process to finish

run_fwi_cpp = subprocess.Popen('./FWI_Process ../inputFiles/ ../Output/ default', shell = True)                       # Run the Processing executable
run_fwi_cpp.wait()                                                     					# Wait for that process to finish

os.chdir('../')												# Go to the home directory
run_visualizer = subprocess.call(['python', 'postProcessing.py Output']) 				# Run the image visualizer from here. It can run independently
sys.exit()                                                      					# Back to the terminal, we are done
