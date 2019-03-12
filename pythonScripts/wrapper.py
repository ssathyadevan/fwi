#!/usr/bin/python
# -*- coding: utf-8 -*-

# MELISSEN 2018 10 25
# Modified according to new architecture, Saurabh 2018 11 20
#
# This program is the overall "wrapper" that first calls the C++ engine of FWI
# and then calls the image maker for imagery

# We import (in Cpp we would say hashtag include) the libraries allowing access to "the computer"

import subprocess, sys

caseFolder = ''

if (len(sys.argv)!= 2):
    print("please supply the directory of a case folder")
    sys.exit()
else:
    caseFolder = sys.argv[1]

#run_fwi_cpp = subprocess.Popen('bin/FWI_PreProcess ' + caseFolder, shell = True)             	# Run the preProcessing executable
#run_fwi_cpp.wait()                                              				               	# Wait for that process to finish

#run_fwi_cpp = subprocess.Popen('bin/FWI_Process ' + caseFolder, shell = True)                # Run the Processing executable
#run_fwi_cpp.wait()                                                     					      # Wait for that process to finish

run_visualizer = subprocess.check_call(['python', 'postProcessing.py', caseFolder]) 				# Run the image visualizer from here. It can run independently
sys.exit()                           



