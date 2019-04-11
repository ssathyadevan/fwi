#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 11 14:18:28 2019

@author: leonard
"""

import numpy as np
from matplotlib import pyplot as plt
from scipy import sparse
from scipy.sparse.linalg import spsolve
import time

xmax = 600.
ymax = 300.
nx = 64
ny = 32
N = nx * ny
hx = xmax / (nx - 1)
hy = ymax / (ny - 1)

nxDomain = nx
nyDomain = ny

freq = 10.
omega = freq * 2. * np.pi

# 1 / wave speed
def n(x,y):
    return 1. / (2000.)

# Sources
def f(x,y):
    if(x == round(nx/2)-1 and y == ny/2):
        return 1. / (hx*hy)
    else:
        return 0.0

def solveHelmholtz(ABCorder):
    alpha = 0.0

    # Laplacian
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
    rhs = np.zeros(N,dtype=np.complex)
       
    # Omega term + ABC + RHS
    # NOTE: Making the ABC inhomogeneous simulates a wave incoming from infinity
    for y in range(0,ny):
        for x in range(0,nx):
            i = y * nx + x
            A[i,i] += n(x,x) * n(x,x) * omega * omega + 1.0j * alpha * omega
            rhs[i] = f(x,y)  
                    
            if (ABCorder == 1): #First order ABC
                if (y == 0):
                    A[i,i] += (2.0j * n(x,y) * omega) / hy
                    A[i,i+nx] += 1.0 / (hy * hy)
    #               if (x == nx / 2): # Incoming wave
    #                   rhs[i] = 2.0j * n(x,y) * omega * 10
                if (y == ny-1):
                    A[i,i] += (2.0j * n(x,y) * omega) / hy
                    A[i,i-nx] += 1.0 / (hy * hy)
                if (x == 0):
                    A[i,i] += (2.0j * n(x,y) * omega) / hx
                    A[i,i+1] += 1.0 / (hx * hx)
                if (x == nx-1):
                    A[i,i] += (2.0j * n(x,y) * omega) / hx
                    A[i,i-1] += 1.0 / (hx * hx)    

            elif (ABCorder == 2): # Second order ABC  (corner central difference in x
                if (y == 0):
                    if (x == 0):
                        A[i,i] += (3.0j * n(x,y) * omega - 2.0 / hy) / hy
                        A[i,i+1] += 1.0 / (hx * hy) 
                        A[i,i+nx] += 2.0 / (hy * hy)
                    elif (x == nx - 1):
                        A[i,i] += (3.0j * n(x,y) * omega - 2.0 / hy) / hy
                        A[i,i-1] += 1.0 / (hx * hy) 
                        A[i,i+nx] += 2.0 / (hy * hy)
                    else:
                        A[i,i] += (hy * 2.0j * n(x,y) * omega) / (hx * hy) + \
                                  -2.0j / (omega * n(x,y) * hx * hx * hx)
                        A[i,i+nx] += 1.0 / (hx * hy)
                        A[i,i-1] += 1.0j / (omega * n(x,y) * hx * hx * hx)
                        A[i,i+1] += 1.0j / (omega * n(x,y) * hx * hx * hx)
                if (y == ny-1):
                    if (x == 0):
                        A[i,i] += (3.0j * n(x,y) * omega - 2.0 / hy) / hy
                        A[i,i+1] += 1.0 / (hx * hy)
                        A[i,i-nx] += 2.0 / (hy * hy)
                    elif (x == nx - 1):
                        A[i,i] += (3.0j * n(x,y) * omega - 2.0 / hy) / hy
                        A[i,i-1] += 1.0 / (hx * hy)
                        A[i,i-nx] += 2.0 / (hy * hy)
                    else:
                        A[i,i] += (hy * 2.0j * n(x,y) * omega) / (hx * hy) + \
                                  -2.0j / (omega * n(x,y) * hx * hx * hx)
                        A[i,i-nx] += 1.0 / (hx * hy)
                        A[i,i-1] += 1.0j / (omega * n(x,y) * hx * hx * hx)
                        A[i,i+1] += 1.0j / (omega * n(x,y) * hx * hx * hx)
                if (x == 0 and y != 0 and y != ny - 1):
                    A[i,i] += (hx * 2.0j * n(x,y) * omega) / (hx * hy) + \
                              -2.0j / (omega * n(x,y) * hy * hy * hy)
                    A[i,i+1] += 1.0 / (hx * hy)
                    A[i,i+nx] += 1.0j / (omega * n(x,y) * hy * hy * hy)
                    A[i,i-nx] += 1.0j / (omega * n(x,y) * hy * hy * hy)
                if (x == nx-1 and y != 0 and y != ny - 1):
                    A[i,i] += (hx * 2.0j * n(x,y) * omega) / (hx * hy) + \
                              -2.0j / (omega * n(x,y) * hy * hy * hy)
                    A[i,i-1] += 1.0 / (hx * hy)
                    A[i,i+nx] += 1.0j / (omega * n(x,y) * hy * hy * hy)
                    A[i,i-nx] += 1.0j / (omega * n(x,y) * hy * hy * hy)                                
                    
    # Solve and plot    
    start = time.perf_counter() 
    u = spsolve(A,rhs)
    end = time.perf_counter() 
    print("Time elapsed: %0.4f seconds" % (end-start))
    return np.reshape(u, (ny,nx))

def solveExact(): # For constant wave velocity!
    from scipy.special import hankel1
    
    X = np.linspace(0,xmax,nx)
    Y = np.linspace(0,ymax,ny)
    
    hx = X[1] - X[0]
    hy = Y[1] - Y[0]
    
    xgrid, ygrid = np.meshgrid(X,Y)
    
    sol = np.zeros((ny,nx),dtype=np.complex)
    
    for i in range(0,nx):
        for j in range(0,ny):
            if (f(i,j) != 0.0):
                x0 = xgrid[j,i]
                y0 = ygrid[j,i]
                
                z0 = omega * n(i,j) * np.sqrt(np.square(xgrid-x0)+np.square(ygrid-y0))
                sol += -0.25j * hankel1(0,z0) * f(i,j) * hx * hy
                sol[j,i] = -0.25j * hankel1(0,1e-15) * f(i,j) * hx * hy
    return sol

def solvePML():
    Lx = xmax
    Ly = ymax
    
    widthPML = 5
    
    nxl = nx + 2*widthPML
    nyl = ny + 2*widthPML
    
    dim = nxl*nyl
    b = np.zeros(dim, dtype=np.complex)
    A = sparse.dok_matrix(((dim, dim)), dtype=np.complex)
    
    def sigmax(x):
        if (x > Lx/2):
            return (x-Lx/2) 
        elif (x < -Lx/2):
            return (x+Lx/2)
        else:
            return 0.0
    
    def Sx(x):
        return 1 + -sigmax(x)*sigmax(x)/(omega*1.0j*hx)
    
    def dSx(x):
        if (x > Lx/2):
            return -2.0*sigmax(x)/(omega*1.0j*hx)
        elif (x < -Lx/2):
            return -2.0*sigmax(x)/(omega*1.0j*hx)
        else:
            return 0.0
        
    def sigmay(y):
        if (y > Ly/2):
            return (y-Ly/2)
        elif (y < -Ly/2):
            return (y+Ly/2)
        else:
            return 0.0
    
    def Sy(y):
        return 1 + -sigmay(y)*sigmay(y)/(omega*1.0j*hy)
    
    def dSy(y):
        if (y > Ly/2):
            return -2.0*sigmay(y)/(omega*1.0j*hy) 
        elif (y < -Ly/2):
            return -2.0*sigmay(y)/(omega*1.0j*hy)
        else:
            return 0.0

    for i in np.arange(0,nxl):
        for j in np.arange(0,nyl):
    
            x = hx*i - Lx/2.0 - widthPML*hx
            y = hy*j - Ly/2.0 - widthPML*hy
            
            idx1 = i + j*nxl
            
            s1 = Sx(x)
            s2 = Sy(y)
            A[idx1,idx1] = -2.*(s2/s1)/(hx*hx) -2.*(s1/s2)/(hy*hy) + s1*s2*n(i,j)*n(i,j)*omega*omega
            
            if(i != nxl-1):
                idx2 = i+1 + j*nxl
                A[idx1,idx2] += (s2/s1)/(hx*hx) - (s2*dSx(x))/(2*hx*s1*s1)
            
            if(i != 0):
                idx2 = i-1 + j*nxl
                A[idx1,idx2] += (s2/s1)/(hx*hx) + (s2*dSx(x))/(2*hx*s1*s1)
            
            if(j != nyl-1):
                idx2 = i + (j+1)*nxl
                A[idx1,idx2] += (s1/s2)/(hy*hy) - (s1*dSy(y))/(2*hy*s2*s2)
            
            if(j != 0):
                idx2 = i + (j-1)*nxl
                A[idx1,idx2] += (s1/s2)/(hy*hy) + (s1*dSy(y))/(2*hy*s2*s2)
            
            b[idx1] = f(i-widthPML,j-widthPML)
    
    A = sparse.csr_matrix(A)
    u = spsolve(A,b)
    
    u = u.reshape((nyl,nxl))
    return u[widthPML:nyl-widthPML,widthPML:nxl-widthPML]
    
exactsol = solveExact()
u = solveHelmholtz(1)
v = solveHelmholtz(2)
w = solvePML()

print(exactsol[16,31])
print(u[16,31])
print(w[16,31])

ax = plt.imshow(np.real(w)) 
plt.colorbar()

#nrowplot = 3
#ncolplot = 2
#shrinkage = 0.6
#fig = plt.figure(figsize=(12,12))
#
#ax = fig.add_subplot(nrowplot,ncolplot,1)
#plt.imshow(np.real(u), vmin=-0.2, vmax=0.15) 
#plt.colorbar(shrink=shrinkage)
#ax.set_title("1st order")
#
#ax = fig.add_subplot(nrowplot,ncolplot,2)
#plt.imshow(np.real(v), vmin=-0.2, vmax=0.15)
#plt.colorbar(shrink=shrinkage)
#ax.set_title("2nd order")
#
#ax = fig.add_subplot(nrowplot,ncolplot,3)
#plt.imshow(np.real(w), vmin=-0.2, vmax=0.15)
#plt.colorbar(shrink=shrinkage)
#ax.set_title("PML")
#
#ax = fig.add_subplot(nrowplot,ncolplot,4)
#plt.imshow(np.real(exactsol-u), vmin=-0.06, vmax=0.06)
#plt.colorbar(shrink=shrinkage)
#ax.set_title("diff exact-1st order")
#
#ax = fig.add_subplot(nrowplot,ncolplot,5)
#plt.imshow(np.real(exactsol-v), vmin=-0.06, vmax=0.06)
#plt.colorbar(shrink=shrinkage)
#ax.set_title("diff exact-2nd order")
#
#ax = fig.add_subplot(nrowplot,ncolplot,6)
#plt.imshow(np.real(exactsol-w), vmin=-0.06, vmax=0.06)
#plt.colorbar(shrink=shrinkage)
#ax.set_title("diff exact-PML")

#plt.tight_layout()

diff_exact_u = np.real(exactsol-u).flatten()
diff_exact_v = np.real(exactsol-v).flatten()
diff_exact_w = np.real(exactsol-w).flatten()

exactsoltemp = exactsol.flatten()

print("")
print("2-Norm diff exact 1st:   %0.2f" %  (np.linalg.norm(diff_exact_u) / np.linalg.norm(exactsol)))
print("2-Norm diff exact 2nd:   %0.2f" %  (np.linalg.norm(diff_exact_v) / np.linalg.norm(exactsoltemp)))
print("2-Norm diff exact PML:   %0.2f" %  (np.linalg.norm(diff_exact_w) / np.linalg.norm(exactsoltemp)))

print("")
print("1-Norm diff exact 1st:   %0.2f" %  (np.linalg.norm(diff_exact_u, ord=1) / np.linalg.norm(exactsoltemp, ord=1)))
print("1-Norm diff exact 2nd:   %0.2f" %  (np.linalg.norm(diff_exact_v, ord=1) / np.linalg.norm(exactsoltemp, ord=1)))
print("1-Norm diff exact PML:   %0.2f" %  (np.linalg.norm(diff_exact_w, ord=1) / np.linalg.norm(exactsoltemp, ord=1)))