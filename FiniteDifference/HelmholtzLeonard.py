import numpy as np
from matplotlib import pyplot as plt
from scipy import sparse
from scipy.sparse.linalg import spsolve
import time

nxr = 128 #number of gridpoints in resevoir in x direction
nyr = 64

sizeX = 600.
sizeY = 300.

hx = sizeX/(nxr-1)
hy = sizeY/(nyr-1)

widthEdge = 400
widthEdgeX = widthEdge/hx
widthEdgeY = widthEdge/hy

xSource = 480.
ySource = 305.

extraWidthLeft = 0.0
extraWidthRight = 0.0
extraWidthUp = 0.0
extraWidthBelow = 0.0

#if source left of domain
if (xSource < -sizeX/2):
    extraWidthLeft = (-xSource - sizeX/2)/hx
#if source right of domain
if (xSource > sizeX/2):
    extraWidthRight = (xSource - sizeX/2)/hx
#if source above domain
if (ySource < 0):
    extraWidthUp = -ySource/hy
#if source below domain
if (ySource > sizeY):
    extraWidthBelow = (ySource-sizeY)/hy
    
totalWidthX = nxr + 2*widthEdgeX + extraWidthLeft + extraWidthRight
totalWidthY = nyr + 2*widthEdgeY + extraWidthUp + extraWidthBelow

nx = round(totalWidthX)
ny = round(totalWidthY)

upperLeftXReservoir = round ( widthEdgeX + extraWidthLeft )
upperLeftYReservoir = round ( widthEdgeY + extraWidthUp )

xSrc = widthEdge
if (xSource > -sizeX/2):
    xSrc += xSource + sizeX/2.0
xSrc = round(xSrc/hx)

ySrc = widthEdge
if (ySource > 0):
    ySrc += ySource
ySrc = round(ySrc/hy)

N = nx * ny

freq = 10
omega = 2*np.pi*freq

# 1 / wave speed
def n(x,y):
    return 1.0/(2000.0)

# Sources
def f(x,y):
    if(x == xSrc and y == ySrc):
        return 1.0/(hx*hy)
    else:
        return 0.0

# x discretisation
xmain_diag = -2.0 * np.ones(nx, dtype = np.complex) / hx**2
xside_diag = np.ones(nx-1, dtype = np.complex) / hx**2
xdiagonals = [xmain_diag, xside_diag, xside_diag]
xdirection = sparse.diags(xdiagonals,[0,-1,1],format="csr")

# y discretisation
ymain_diag = -2.0 * np.ones(ny, dtype = np.complex) / hy**2
yside_diag = np.ones(ny-1, dtype = np.complex) / hy**2
ydiagonals = [ymain_diag, yside_diag, yside_diag]
ydirection = sparse.diags(ydiagonals,[0,-1,1],format="csr")

# x,y discretisation
A = sparse.kron(sparse.eye(ny),xdirection) + sparse.kron(ydirection,sparse.eye(nx))

# Add term
for x in range(0,nx):
    for y in range(0,ny):
        idx = x + y*nx
        A[idx,idx] += n(x,y) * n(x,y) * omega * omega

#omega = 2*omega  
#ABC
for y in range(0,ny):
    for x in range(0,nx):
        idx = y * nx + x
        if (y == 0):
            A[idx,idx] += -(2.0 * hy * 1.0j * n(x,y) * omega) / (hx * hy)
            A[idx,idx+nx] += 1.0 / (hx * hy)
        if (y == ny-1):
            A[idx,idx] += -(2.0 * hy * 1.0j * n(x,y) * omega) / (hx * hy)
            A[idx,idx-nx] += 1.0 / (hx * hy)
        if (x == 0):
            A[idx,idx] += -(2.0 * hx * 1.0j * n(x,y) * omega) / (hx * hy)
            A[idx,idx+1] += 1.0 / (hx * hy)
        if (x == nx-1):
            A[idx,idx] += -(2.0 * hx * 1.0j * n(x,y) * omega) / (hx * hy)
            A[idx,idx-1] += 1.0 / (hx * hy)          

# RHS vector
rhs = np.zeros(N,dtype=np.complex)
for x in range(0,nx):
    for y in range(0,ny):
        idx = y * nx + x
        rhs[idx] = f(x,y)       
    
# Solve and plot    
start = time.perf_counter() 
u = spsolve(A,rhs)
end = time.perf_counter() 
print("Time elapsed: %0.4f seconds" % (end-start))
u = np.reshape(u, (ny,nx))
#pTot = u[upperLeftYReservoir:upperLeftYReservoir+nyr, upperLeftXReservoir:upperLeftXReservoir+nxr]
#plt.imshow(np.real(pTot))
plt.imshow(np.real(u))
plt.colorbar()