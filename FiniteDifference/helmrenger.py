import numpy as np
from scipy import sparse
from scipy.sparse.linalg import spsolve
import matplotlib.pyplot as plt

nx = 64
ny = 32

h = 1.0/(nx-1)

A = np.zeros((nx*ny,nx*ny), dtype=np.complex)
b = np.zeros(nx*ny, dtype=np.complex)

omega = 40
#alpha = 2

def n(i, j):
    return 1.0

def f(i, j):
    if (i == 32 and j == 16):
        return 0.1
    elif(i == 64 and j == 50):
        return 0.0
    elif(i == 50 and j == 7):
        return 0.0
    elif(i == 70 and j == 7):
        return 0.0
    elif(i == 90 and j == 7):
        return 0.0
    else:
        return 0
    
def alpha(i, j):
    if (i < 6 or i > nx -7 or j < 6 or j > ny -7):
        return 20.0
    else:
        return 0.0
    
def g(i,j):
    return 1.0e-3
    
def getIndex(i,j):
    return i + j*nx
    
for x in np.arange(1,nx-1):
    for y in np.arange(1,ny-1):
        idx1 = getIndex(x,y)
        A[idx1][idx1] = -4/(h*h) + n(x,y)*n(x,y)*omega*omega + 1.0j*omega*alpha(x,y)
        
        idx2 = getIndex(x+1,y)
        A[idx1][idx2] = 1/(h*h) + 0.0j
        
        idx2 = getIndex(x-1,y)
        A[idx1][idx2] = 1/(h*h) + 0.0j
        
        idx2 = getIndex(x,y+1)
        A[idx1][idx2] = 1/(h*h) + 0.0j
        
        idx2 = getIndex(x,y-1)
        A[idx1][idx2] = 1/(h*h) + 0.0j
        
        b[idx1] = f(x,y)

        
for x in np.arange(0,nx):
    idx = getIndex(x,0)
    A[idx][idx] = g(x,0)
    
    idx = getIndex(x,ny-1)
    A[idx][idx] = g(x,ny-1)

for y in np.arange(0,ny):  
    idx = getIndex(0,y)
    A[idx][idx] = g(0,y)
    
    idx = getIndex(nx-1,y)
    A[idx][idx] = g(nx-1,y)

sA = sparse.csr_matrix(A)
uFlat = spsolve(sA,b)

#uFlat = np.linalg.solve(A,b)

x = uFlat.reshape((ny,nx))

plt.imshow(np.real(x)[6:ny-6,6:nx-6])