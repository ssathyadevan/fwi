#!/usr/bin/python

import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 7:
    sys.exit("not enough input arguments, usage: ShowChi.py file1 file2 fileout nx nz bool_showfig")

filename1 = str(sys.argv[1])
filename2 = str(sys.argv[2])
filenameout = str(sys.argv[3])
nx = int(sys.argv[4])
nz = int(sys.argv[5])
bool_showfig = int(sys.argv[6])

chi1 = np.genfromtxt(filename1)
chi1 = chi1.reshape((nz, nx))

chi2 = np.genfromtxt(filename2)
chi2 = chi2.reshape((nz, nx))

v_min = chi1.min()
v_max = chi1.max()

plt.clf

plt.subplot(2,2,1)
plt.imshow(chi1, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.subplot(2,2,2)
plt.imshow(chi2, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.savefig(filenameout, dpi=400)

if bool_showfig == 1:
    plt.show()


