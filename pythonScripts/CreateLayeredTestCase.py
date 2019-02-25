#!/bin/python

## This file creates a test case of layers with different carriage speeds
## The first layer always has background carriage speed

import numpy as np

# Output Properties
x = 64                                                  # Number of horizontal grid points
z = 32                                                  # Number of vertical grid points
nlayer = 6                                              # Number of subsurface layers
background_layer_height = 4                             # Height of background layer in grid points
filename = "layers.txt"

# Random layer height
np.random.seed(4)
layer_height = np.random.random_integers(1, 5, nlayer)

# Layer Carriage Speed
background_c = 2000.
layer_c = np.linspace(2100., 2218., nlayer)             # Vector of evenly spaced densities

# Randomise layers
np.random.shuffle(layer_c)                              # Shuffle vector

# Calculate Chi
layer_chi = 1 - np.square(background_c / layer_c)

# Create output
output = np.zeros((z, x))

start = background_layer_height
end = background_layer_height

# Put background layer of size 2 in between each layer
for layer in range(1,nlayer+1):
    start += (layer > 1) * (layer_height[layer-2] + 2)
    end += layer_height[layer-1] + (layer > 1) * 2
    output[start:end, :] = layer_chi[layer-1]
    
    print(output[start,1])

# Write output to file
output = np.reshape(output, (x*z,1))                    # Reshape array
np.savetxt(filename, output)
