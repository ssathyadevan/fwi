#!/usr/bin/python
# -*- coding: utf-8 -*-

# MELISSEN 2018 10 25
#
# import (in Cpp this is hashtag include)...
# ...the numpy and matplotlib libraries for the math and imagery
# ...and sys library to quit

import numpy as np
import matplotlib.pyplot as plt
import sys

f = open("outputfwi.txt","r")        # Open the file where we printed the settings used in Cpp, we'll reuse them
contents=f.readlines()               # Just dump the contents of this file into a big variable
x= contents[1]                       # Take the first line entirely...
y,u,v= x.split()                     # Split it into its constituent words
nxt = int(v)                         # Cast the third word to an integer, we know this is nxt, we built the .txt after all
x= contents[2]                       # The rest of this block repeats this process for nzt and interactive (dirty non-loop)
y,u,v= x.split()
nzt = int(v)
x= contents[3]
y,u,v= x.split()
interactive = int(v)

#zerothfile="src/ShowChi.py"
filename1="libraries/src/chi.txt"              # variable name for the original temple
filename2="libraries/src/chi_est_temple.txt"   # variable name for the calculated temple
filenameout="libraries/src/temple_result.png"  # how we store the image

chi1 = np.genfromtxt(filename1)      # start image set up for original temple
chi1 = chi1.reshape((nzt, nxt))      

chi2 = np.genfromtxt(filename2)      # start image set up for calculated temple
chi2 = chi2.reshape((nzt, nxt))      

v_min = chi1.min()                   # set the minimum and maximum values to chi...
v_max = chi1.max()                   # ...(just copy-pasted this)

# From here we make the actual plots, with relatively self-explanatory commands

plt.clf

plt.subplot(2,2,1)
plt.imshow(chi1, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.subplot(2,2,2)
plt.imshow(chi2, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

# After that, we save the plot,...
# ...show it automatically if interactive is on,...
# ...print ("cout")  that we're done and quit.

plt.savefig(filenameout, dpi=400)

if interactive == 1:
    plt.show()

print "The picture has been generated with Python" 

sys.exit()
