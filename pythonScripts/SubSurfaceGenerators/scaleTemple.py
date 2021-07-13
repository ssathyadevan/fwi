#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import numpy as np
import matplotlib.pyplot as plt
import cv2
import argparse


def CreateOutputFileName(filename, x, z):
    return filename + str(x) + 'x' + str(z) + ".txt";



# Configure the argument parser
parser = argparse.ArgumentParser(description="Upscale the given image.",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

# Input arguments
parser.add_argument("-x", type=int, required=False, default=64,
                    help="x-axis size of the input image")

parser.add_argument("-z", type=int, required=False, default=32,
                    help="z-axis size of the input image")
                    
parser.add_argument("-nx", type=int, required=False, default=128,
    help="x-axis size of the output image")     
    
parser.add_argument("-nz", type=int, required=False, default=64,
    help="z-axis size of the output image")    


# Parse the input arguments
arguments = parser.parse_args()

#Load and create image

fd = np.loadtxt('temple.txt', delimiter='\n')

image = fd.reshape(arguments.z, arguments.x)

# plt.imshow(image)

#Resize image

imagenew = cv2.resize(image, dsize=(arguments.nx, arguments.nz))

# plt.imshow(imagenew)


#Convert new image to txt file format

fd = imagenew.flatten()
for i in range(0, len(fd)):
    if (fd[i] > 0.1):
        fd[i] = 0.1869132054239393
    else:
        fd[i] = 0.0

# plt.imshow(fd.reshape(arguments.nz, arguments.nx))
# plt.show()

np.savetxt(CreateOutputFileName("temple", arguments.nx, arguments.nz), fd)
