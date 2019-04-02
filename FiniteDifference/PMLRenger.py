import numpy as np
from scipy import sparse
from scipy.sparse.linalg import spsolve
import matplotlib.pyplot as plt

nxDomain = 64
nyDomain = 32
Lx = 600.0
Ly = 300.0

widthPML = 5

nx = nxDomain + 2*widthPML
ny = nyDomain + 2*widthPML

hx = Lx/(nxDomain-1)
hy = Ly/(nyDomain-1)

dim = nx*ny
b = np.zeros(dim, dtype=np.complex)
A = sparse.dok_matrix(((dim, dim)), dtype=np.complex)

frequency = 10
omega = 2*np.pi*frequency

def n(i, j):
    return 1.0/2000.0

def f(i, j):
    if (i == 32+widthPML and j == 16+widthPML):
        return 1.0/ (hx*hy)
    #if (i == 32 and j == 16):
    #    return 1.0/ (hx*hy)
    else:
        return 0.0

def sigmax(x):
    if (x > Lx/2):
        return (x-Lx/2)**2
    elif (x < -Lx/2):
        return (x+Lx/2)**2
    else:
        return 0.0

def Sx(x):
    return 1 + sigmax(x)/(omega*1.0j)

def dSx_dx(x):
    if (x > Lx/2):
        return 2.0*(x-Lx/2)/(omega*1.0j) 
    elif (x < -Lx/2):
        return 2.0*(x+Lx/2)/(omega*1.0j)
    else:
        return 0.0
    
def sigmay(y):
    if (y > Ly/2):
        return (y-Ly/2)**2
    elif (y < -Ly/2):
        return (y+Ly/2)**2
    else:
        return 0.0

def Sy(y):
    return 1 + sigmay(y)/(omega*1.0j)

def dSy_dy(y):
    if (y > Ly/2):
        return 2.0*(y-Ly/2)/(omega*1.0j) 
    elif (y < -Ly/2):
        return 2.0*(y+Ly/2)/(omega*1.0j)
    else:
        return 0.0
        
def getIndex(i,j):
    return i + j*nx
   
for i in np.arange(0,nx):
    for j in np.arange(0,ny):

        x = hx*i - Lx/2.0 - widthPML*hx
        y = hy*j - Ly/2.0 - widthPML*hy
        
        idx1 = getIndex(i,j)
        
        s1 = Sx(x)
        s2 = Sy(y)
        A[idx1,idx1] = -2*(s2/s1)/(hx*hx) -2*(s1/s2)/(hy*hy) + s1*s2*n(i,j)*n(i,j)*omega*omega
        
       
        if(i != 0):
            idx2 = getIndex(i-1,j)
            A[idx1,idx2] = (s2/s1)/(hx*hx) + (s2*dSx_dx(x))/(2*hx*s1*s1)
            
        if(i != nx-1):
            idx2 = getIndex(i+1,j)
            val2 = (s2/s1)/(hx*hx) - (s2*dSx_dx(x))/(2*hx*s1*s1)
            A[idx1,idx2] = val2

        if(j != 0):
            idx2 = getIndex(i,j-1)
            A[idx1,idx2] = (s1/s2)/(hy*hy) + (s1*dSy_dy(y))/(2*hy*s2*s2)
       
        if(j != ny-1):
            idx2 = getIndex(i,j+1)
            val1 = (s1/s2)/(hy*hy) - (s1*dSy_dy(y))/(2*hy*s2*s2)
            A[idx1,idx2] = val1
        
        
        b[idx1] = f(i,j)
        
        if(j == 0 and i == 1):
            print(x, y, s1, s2, val1, val2)

sA = sparse.csr_matrix(A)
uFlat = spsolve(sA,b)

x = uFlat.reshape((ny,nx))
image = np.real(x)[widthPML:ny-widthPML,widthPML:nx-widthPML]

plt.imshow(image)
plt.colorbar()

