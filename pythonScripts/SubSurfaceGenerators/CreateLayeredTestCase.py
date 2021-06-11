#!/bin/python

# This file creates a test case of layers with different carriage speeds
# The first layer always has background carriage speed

import numpy as np
import matplotlib.pyplot as plt

#Generate the following parameters
list = np.array([[30,15],[40,20],[50,25],[60,30],[70,35],[80,40],[90,45],[100,50],[110,66],[120,60],[130,65]])
for xz in list:
    # Output Properties
    # Number of horizontal grid points
    x = xz[0]
    # Number of vertical grid points
    z = xz [1]
    # Number of subsurface layers
    nlayer = 4
    # Height of background layer in grid points
    background_layer_height = int(0.15*z)
    layer_spacing = int(0.07*z)                                     # Spacing between layers
    filename = "layer_" + str(x) + "_" + str(z) + "_points" + str(x*z) + "_.txt"

    # Random layer height
    np.random.seed(4)
    layer_height = np.random.random_integers(int(0.1*z), int(0.2*z), nlayer)

    # Layer Carriage Speed
    background_c = 2000.
    # Vector of evenly spaced densities
    layer_c = np.linspace(2200., 2300., nlayer)

    # Randomise layers
    np.random.shuffle(layer_c)                              # Shuffle vector

    # Calculate Chi
    layer_chi = 1. - np.square(background_c / layer_c)

    # Create output
    output = np.zeros((z, x))

    start = background_layer_height
    end = background_layer_height

    # Put background layer of size 2 in between each layer
    for layer in range(1, nlayer+1):
        start += (layer > 1) * (layer_height[layer-2] + layer_spacing)
        end += layer_height[layer-1] + (layer > 1) * layer_spacing
        output[start:end, :] = layer_chi[layer-1]

        #print(output[start, 1])

    # Write output to file
    output = np.reshape(output, (x*z, 1))                    # Reshape array
    np.savetxt(filename, output)
    plt.imshow(output.reshape(z, x))
    plt.savefig(filename+'.png')
