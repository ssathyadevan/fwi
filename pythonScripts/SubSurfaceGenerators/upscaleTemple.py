#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr  4 15:43:15 2019

@author: leonard
"""

import numpy as np
import matplotlib.pyplot as plt
import cv2

nx = 64
nz = 32

scalefactor = 2

fd = np.loadtxt('temple.txt', delimiter='\n')

image = fd.reshape(nz, nx)

# plt.imshow(image)

imagenew = cv2.resize(image, dsize=(scalefactor*nx, scalefactor*nz))

# plt.imshow(imagenew)

fd = imagenew.flatten()
for i in range(0, len(fd)):
    if (fd[i] > 0.1):
        fd[i] = 0.1869132054239393
    else:
        fd[i] = 0.0

# plt.imshow(fd.reshape(scalefactor*nz,scalefactor*nx))

np.savetxt("largetemple.txt", fd)
