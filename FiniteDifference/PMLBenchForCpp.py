import numpy as np
from scipy import sparse
from scipy.sparse.linalg import spsolve
import matplotlib.pyplot as plt

nxDomain = 64
nyDomain = 32
Lx = 600.0
Ly = 300.0

c0 = 2000.0
frequency = 10.

xSource = -480.
ySource = -5.

hx = Lx/(nxDomain-1)
hy = Ly/(nyDomain-1)

widthPML = round( (.8*c0/frequency)/hx +0.5)

omega = 2*np.pi*frequency

extraWidthLeft = 0
extraWidthRight = 0
extraWidthUp = 0
extraWidthBelow = 0

#if source left of domain
if (xSource < -Lx/2):
    extraWidthLeft = round((-xSource - Lx/2)/hx + 0.5)
#if source right of domain
if (xSource > Lx/2):
    extraWidthRight = round((xSource - Lx/2)/hx + 0.5)
#if source above domain
if (ySource < 0):
    extraWidthUp = round(-ySource/hy + 0.5)
#if source below domain
if (ySource > Ly):
    extraWidthBelow = round((ySource-Ly)/hy + 0.5)
    
nx = nxDomain + 2*widthPML + extraWidthLeft + extraWidthRight
ny = nyDomain + 2*widthPML + extraWidthUp + extraWidthBelow

upperLeftXReservoir = widthPML + extraWidthLeft
upperLeftYReservoir = widthPML + extraWidthUp

coordPMLLeft = -Lx/2 - hx*extraWidthLeft
coordPMLRight = Lx/2 + hx*extraWidthRight
coordPMLUp = -Ly/2 - hy*extraWidthUp
coordPMLDown = Ly/2 + hy*extraWidthBelow 

dim = nx*ny
b = np.zeros(dim, dtype=np.complex)
A = sparse.dok_matrix(((dim, dim)), dtype=np.complex)

def n(i, j):
    return 1.0/2000.0

idxSourceX = upperLeftXReservoir + round((xSource+Lx/2)/hx)
idxSourceY = upperLeftYReservoir + round((ySource)/hy)
def f(i, j):
    if (i == idxSourceX and j == idxSourceY):
        return 1.0/ (hx*hy)
    else:
        return 0.0

def sigmax(x):
    if(x > coordPMLRight ):
        return (x - coordPMLRight)**2
    elif (x < coordPMLLeft):
        return (x-coordPMLLeft)**2
    else:
        return 0.0

def Sx(x):
    return 1 + sigmax(x)/(omega*1.0j)

def dSx_dx(x):
    if (x > coordPMLRight):
        return 2.0*(x-coordPMLRight)/(omega*1.0j) 
    elif (x < coordPMLLeft):
        return 2.0*(x-coordPMLLeft)/(omega*1.0j)
    else:
        return 0.0
    
def sigmay(y):
    if (y > coordPMLDown):
        return (y-coordPMLDown)**2
    elif (y < coordPMLUp):
        return (y-coordPMLUp)**2
    else:
        return 0.0

def Sy(y):
    return 1 + sigmay(y)/(omega*1.0j)

def dSy_dy(y):
    if (y > coordPMLDown):
        return 2.0*(y-coordPMLDown)/(omega*1.0j) 
    elif (y < coordPMLUp):
        return 2.0*(y-coordPMLUp)/(omega*1.0j)
    else:
        return 0.0
        
def getIndex(i,j):
    return i + j*nx
   
for i in np.arange(0,nx):
    for j in np.arange(0,ny):
        idx1 = getIndex(i,j)
        
        x = hx*i - Lx/2.0 - upperLeftXReservoir*hx
        y = hy*j - Ly/2.0 - upperLeftYReservoir*hy

        s1 = Sx(x)
        s2 = Sy(y)
        
        A[idx1,idx1] = -2*(s2/s1)/(hx*hx) -2*(s1/s2)/(hy*hy) + s1*s2*n(i,j)*n(i,j)*omega*omega
              
        if(i != 0):
            idx2 = getIndex(i-1,j)
            A[idx1,idx2] = (s2/s1)/(hx*hx) + (s2*dSx_dx(x))/(2*hx*s1*s1)
            
        if(i != nx-1):
            idx2 = getIndex(i+1,j)
            A[idx1,idx2] = (s2/s1)/(hx*hx) - (s2*dSx_dx(x))/(2*hx*s1*s1)

        if(j != 0):
            idx2 = getIndex(i,j-1)
            A[idx1,idx2] = (s1/s2)/(hy*hy) + (s1*dSy_dy(y))/(2*hy*s2*s2)
       
        if(j != ny-1):
            idx2 = getIndex(i,j+1)
            A[idx1,idx2] = (s1/s2)/(hy*hy) - (s1*dSy_dy(y))/(2*hy*s2*s2)
               
        b[idx1] = f(i,j)
        
        if(i==100 and j==0):
            print(x, y, s1, s2)

sA = sparse.csr_matrix(A)
uFlat = spsolve(sA,b)

x = uFlat.reshape((ny,nx))
image = np.real(x)[upperLeftYReservoir:upperLeftYReservoir+nyDomain, upperLeftXReservoir:upperLeftXReservoir+nxDomain]

plt.imshow(image)
plt.colorbar()

# Save to csv
y = x[upperLeftYReservoir:upperLeftYReservoir+nyDomain, upperLeftXReservoir:upperLeftXReservoir+nxDomain]
np.savetxt("PythonBenchpTot.csv", y.flatten(), delimiter=",", fmt="(%.18f,%.18f)")