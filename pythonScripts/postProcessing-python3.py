#!/usr/bin/python
# -*- coding: utf-8 -*-
# MELISSEN 2018 10 25
#
# import (in Cpp this is hashtag include)...
# ...the numpy and matplotlib libraries for the math and imagery
# ...and sys library to quit
import csv
import sys
from skimage.transform import resize
from skimage import data, color
import matplotlib.pyplot as plt
import numpy as np
import matplotlib
# matplotlib.use('Agg')


def find(substr, whichin):
    from datetime import datetime
    lines = [x for x in open(whichin+"Process.out") if substr in x]
    line = lines[0]
    manip = line.replace(substr, '').replace("\n", '')
    start_or_finish = (datetime.strptime(manip, '%c'))
    return start_or_finish


# Enter here the name of the case folder you want to post process
outputPath = sys.argv[1]

g = open(outputPath + "/output/lastRunName.txt", "r")
contents = g.readlines()
runName = contents[0].rstrip()

# Open the file where we printed the settings used in Cpp, we'll reuse them
f = open(outputPath + "/output/" + runName + ".pythonIn", "r")
# Just dump the contents of this file into a big variable
contents = f.readlines()

# Take the first line entirely...
x = contents[1]
# Split it into its constituent words
y, u, v = x.split()
# Cast the third word to an integer, we know this is nxt, we built the .txt after all
nxt = int(v)

# The rest of this block repeats this process for nzt and other parameters needed (dirty non-loop)
x = contents[2]
y, u, v = x.split()
nzt = int(v)

# The rest of this block repeats this process for nzt and other parameters needed (dirty non-loop)
x = contents[3]
y, u, v = x.split()
nxt_original = int(v)

# The rest of this block repeats this process for nzt and other parameters needed (dirty non-loop)
x = contents[4]
y, u, v = x.split()
nzt_original = int(v)

# zerothfile="src/ShowChi.py"
filename1 = outputPath + "/output/chi_ref_" + runName + \
    ".txt"   # variable name for the original temple
filename2 = outputPath + "/output/chi_est_" + runName + \
    ".txt"   # variable name for the calculated temple
filenameout = outputPath + "/output/" + runName + \
    "Result.png"   # how we store the image

chi1 = np.genfromtxt(filename1)      # start image set up for original image
chi1 = chi1.reshape((nzt_original, nxt_original))

# start image set up for reconstructed image
chi2 = np.genfromtxt(filename2)
chi2 = chi2.reshape((nzt, nxt))

# set the minimum and maximum values to chi...
v_min = chi1.min()
v_max = chi1.max()                   # ...(just copy-pasted this)

# We upscale the smaller image to avoid information loss
if (nxt_original > nxt):
    chi2 = resize(chi2, (nzt_original, nxt_original), mode='reflect')
    nxt = nxt_original
    nzt = nzt_original
elif (nxt > nxt_original):
    chi1 = resize(chi2, (nzt, nxt), mode='reflect')
    nxt_original = nxt
    nzt_original = nzt

# We compute the error per pixel, the square mean of the original image and the mse
diff_chi = chi2-chi1
mse = (np.square(diff_chi)).mean()
square_mean_original = (np.square(chi1)).mean()
avg_relative_error = np.sqrt(mse)/square_mean_original*100
print("The MSE (mean square error) is:       "+str(mse))
print("The average relative error is:        "+str(avg_relative_error))

# We find the execution time in the logs
datetime_new_start = find("Starting at ", outputPath + "/output/" + runName)
datetime_new_finish = find("Finished at ", outputPath + "/output/" + runName)
new_total_seconds = (datetime_new_finish - datetime_new_start).seconds

print("Execution time in seconds:            "+str(new_total_seconds))


# Here we make and save the actual plots
plt.clf

plt.subplot(3, 1, 1)
plt.text(64., 38., 'Chi values in original reservoir')
plt.imshow(chi1, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.subplot(3, 1, 2)
plt.text(64., 38., 'Chi values in reconstructed reservoir')
plt.imshow(chi2, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.subplot(3, 1, 3)
plt.text(64., 38., 'Difference between chi values')
plt.imshow(diff_chi, interpolation='nearest')
plt.colorbar()

plt.savefig(filenameout, dpi=400, bbox_inches='tight')

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
plt.savefig(outputPath+"/output/"+runName+"Residual.png", dpi=400)


print("The pictures have been generated with Python")

sys.exit()
