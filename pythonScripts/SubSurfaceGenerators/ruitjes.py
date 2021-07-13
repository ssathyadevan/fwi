import numpy as np
import matplotlib.pyplot as plt
import argparse


def CreateOutputFileName(filename, x, z):
    return filename + str(x) + 'x' + str(z) + ".txt";

def ctoChi(c):
    c0 = 500
    return 1 - (c0/c)*(c0/c)


# Configure the argument parser
parser = argparse.ArgumentParser(description="Create a checker patttern image.",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

# Input arguments
parser.add_argument("-nx", type=int, required=False, default=64,
                    help="x-axis size of the image")

parser.add_argument("-nz", type=int, required=False, default=32,
                    help="z-axis size of the image")
                    
parser.add_argument("-width", type=int, required=False, default=3,
                    help="width of the lines")


# Parse the input arguments
arguments = parser.parse_args()

nx = arguments.nx
nz = arguments.nz
width = arguments.width

plaatje = np.zeros((nz, nx))


for i in np.arange(0, nx):
    for j in np.arange(0, nz):
        x = i - (nx/2)
        y = j - (nz/2)

        if (x+y) % 15 <= width or (x-y) % 15 <= width:
            plaatje[j][i] = ctoChi(500)
        else:
            plaatje[j][i] = ctoChi(520)

plaatje.resize(nx*nz)


with open(CreateOutputFileName('ruitjes', nx, nz), 'w') as f:
    for entry in plaatje:
        f.write(str(entry))
        f.write('\n')

plaatje = plaatje.reshape((nz, nx))

plt.imshow(plaatje, interpolation='nearest', vmin=0, vmax=0.1)
#plt.show()
