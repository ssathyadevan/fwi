#!/usr/bin/python
# -*- coding: utf-8 -*-

# MELISSEN 2018 10 25
#
# import (in Cpp this is hashtag include)...
# ...the numpy and matplotlib libraries for the math and imagery
# ...and sys library to quit
import csv
import sys
import matplotlib.pyplot as plt
import numpy as np
import matplotlib
matplotlib.use('Agg')

outputPath = sys.argv[1]  # Name of the test case you want to post process

g = open(outputPath+"/output//lastRunName.txt", "r")
contents = g.readlines()
runName = contents[0].rstrip()

# Open the file where we printed the settings used in Cpp, we'll reuse them
f = open(outputPath+"/output/"+runName+".pythonIn", "r")
# Just dump the contents of this file into a big variable
contents = f.readlines()
x = contents[1]                       			# Take the first line entirely...
y, u, v = x.split()                     			# Split it into its constituent words
# Cast the third word to an integer, we know this is nxt, we built the .txt after all
nxt = int(v)

# The rest of this block repeats this process for nzt and other parameters needed (dirty non-loop)
x = contents[2]
y, u, v = x.split()
nzt = int(v)

# zerothfile="src/ShowChi.py"
filename1 = outputPath + "/output/chi_ref_" + runName + \
    ".txt"   # variable name for the original temple
filename2 = outputPath + "/output/chi_est_" + runName + \
    ".txt"   # variable name for the calculated temple
filenameout = outputPath + "/output/" + runName + \
    "Result.png"   # how we store the image

chi1 = np.genfromtxt(filename1)      # start image set up for original temple
chi1 = chi1.reshape((nzt, nxt))

chi2 = np.genfromtxt(filename2)      # start image set up for calculated temple
chi2 = chi2.reshape((nzt, nxt))

# set the minimum and maximum values to chi...
v_min = chi1.min()
v_max = chi1.max()                   # ...(just copy-pasted this)

# Here we make and save the actual plots

plt.clf

plt.subplot(2, 2, 1)
plt.imshow(chi1, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.subplot(2, 2, 2)
plt.imshow(chi2, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.savefig(filenameout, dpi=400)

####################################
####  Create the residual plots ####
####################################

residualLogFileName = outputPath+"/output/"+runName + "Residual.log"
reader_residual = csv.reader(open(residualLogFileName), delimiter=',')
dummy_variable_reader_pr = list(reader_residual)
reader_residual_array = np.array(dummy_variable_reader_pr).astype("float")

plt.clf()
plt.plot(reader_residual_array[:, 0])
plt.ylabel("Residual")
plt.grid(True)
plt.savefig(outputPath + "/output/" + runName + "Residual.png", dpi=400)


print"The pictures have been generated with Python"

sys.exit()
