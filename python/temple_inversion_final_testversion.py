#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Spyder Editor

This temporary script file is located here:
/home/mdereus/.spyder2/.temp.py
"""

from numpy import linspace, pi, meshgrid, zeros, dot, random, copy, save, load, ones, einsum, real, conj, gradient, roots, array, sqrt, cos, sin, mean, diff, size, asarray
from numpy.linalg import norm, lstsq
from scipy.special import hankel2, hankel1
from sympy import nsolve, Symbol
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import time

# templefile = './chi_soil.txt'
temple = load('./temple_small.npy')
nx = 128
nz = 64

#temple = load('./temple_thesissize.npy')
#nx = 120
#nz = 60

# templefile = './temple_coarse.npy'
# nx = 32
# nz = 16

# n_max determines how many extra functions we add to the equation for the final chi
n_max = 3
n_freq = 10 #  amount of frequencies to explore

# these are the max iterations and the tolerance of the inner loop, added by Ankit
maxit2 = 30
tol2 = 1.0e-6

maxit2nd = 25
eps_2 = 5.0e-5

nsrc = 17
regularisation = 1  # flag to do the regularization, if 0, use original method implemented by Michiel
recalculateinput = 1  # flag to recalculate the p_data and p_0

read_chi = 0 #if flag==1 then chi_est and p_est is read from input file 
chi_file = "chi_est_4.npy"

pp = PdfPages('multipage.pdf')

# TODO other greens function
def Greens(k_wave, r, omega):
    # 3D Helmholtz Green's function
    #return 0.25 / pi / r * exp(1j * k * r)

    # 2D Helmholtz Green's function
    # is given at p1 of the pdf of Michiel de Reus.
    return -0.25 * 1j * hankel1(0.0, k_wave * r)
    # return 0.25 * 1j * hankel2(0.0, k_wave * r)

# Function that follows the procedure in paragraph 2.4
# given a known contrast function chi,which has been calculated from the input temple, so not a real scenario
def calcField(G_xx, chi, p_init, maxIt, tol):

    phi = list()
    chi_p_old = zeros(chi.shape)
    p_tot_old = copy(p_init)
    #start_time = time.time()
    for it in range(maxIt):
        #start_time2 = time.time()
        chi_p = chi * p_tot_old
        dW = chi_p - chi_p_old

        #print 'Norm dW = ' + str(norm(dW))
        res = norm(dW)/norm(chi_p)
        if res < tol:
            print 'Convergence after ' + str(it) + ' iterations'
            break
	#start_time = time.time()
        
        
        
        G_sm = G_xx[0:nx,:]
        prod = zeros((nz*nx),complex)
        dummy = zeros((nx),complex)
     
        for i in range(nz):
            for j in range(0,nz-i):
    	        dummy1 = dot( G_sm[ : , i*nx : (i+1)*nx ] , dW[ (j+i)*nx : (j+i+1)*nx ] );
    	        
                prod[ j*nx : (j+1)*nx ] += dummy1
                if ( 2*i + j < nz and i>0 ): 
                    prod[ (j+2*i)*nx : (j+2*i+1)*nx ] += dummy1
                    
        for i in range(1,nz):
            if (i <= nz-i):
                for j in range(i):
                    prod[ (i+j)*nx : (i+j+1)*nx ] += dot( G_sm[ : , i*nx : (i+1)*nx ], dW[ j*nx : (j+1)*nx ] )
            else:
                for j in range(nz-i):
                    prod[ (j+i)*nx : (j+i+1)*nx ] += dot( G_sm[ : , i*nx : (i+1)*nx ], dW[ j*nx : (j+1)*nx ] )
            

        tmp = prod
        #import ipdb; ipdb.set_trace()
 #       tmp = dot(G_xx, dW)
        phi.append(tmp)


        # we calculate the rhs vector
 #       f = dot(G_xx, chi * p_init)

        # We construct the lhs least-squares matrix,
        # not very efficient
 #       A = zeros((n,i+1), complex)
 #       for j in range(i+1):
 #           A[:,j] = phi[j] - dot(G_xx, chi * phi[j])

 #       alpha = lstsq(A, f)
 #       alpha = alpha[0]


	alpha = ones(it+1)
        p_tot = copy(p_init)

        for j in range(it+1):
            p_tot += alpha[j] * phi[j]

        chi_p_old = chi_p
        chi_p = None
        p_tot_old = p_tot
        p_tot = None
	
	#print max(prod-tmp), min(prod-tmp)        
    #print("--- %s seconds ---" % (time.time() - start_time))
    if not res < tol:
        print 'No convergence after ' + str(maxIt) + ' iterations, res = ' + str(res)

    return p_tot_old

# Specify the domain
# x is the lateral position in m, see p53
x_min = -300.0
x_max = 300.0

dx = (x_max - x_min) / nx

#z is the depth (in m)
z_min = 0.0
z_max = 300.0
domain_area = (x_max-x_min)*(z_max-z_min)

dz = (z_max - z_min) / nz

# n is the amount of data points in the temple example
n = nx * nz

x = linspace(x_min + 0.5 * dx, x_max - 0.5 * dx, nx)
z = linspace(z_min + 0.5 * dz, z_max - 0.5 * dz, nz)


# set the source parameters
c_0 = 2000.0 # background velocity m s^-1
f = linspace(10, 40, n_freq)
df = mean(diff(f))

omega = 2.0 * pi * f  # temporal angular frequency in rad, 94.2 s^-1 for 15 hz
k_wave = omega / c_0  # angular wavenumber, radians per m?, used for the greens function, 0.0471 for 15 hz and c0=2000 m/s


# create mesh grids of x and z coordinates
xv, zv = meshgrid(x, z)
xv = xv.reshape(nx * nz)
zv = zv.reshape(nx * nz)

# Create the sources
x_src = linspace(-480.0, 480.0, nsrc)
# x_src = linspace(-960.0, 960.0, 17) #thesis?
# x_src = linspace(-120.0, 120.0, 17)
dx_scr = mean(diff(x_src))
n_src = x_src.size
z_src = -5.0 * ones(x_src.shape)
# z_src = -50.0 * ones(x_src.shape)

# Create the receivers, equal to sources for now
x_recv = x_src
# x_recv = linspace(-900.0, 1020.0, 17)
dx_recv = mean(diff(x_recv))
n_recv = x_recv.size
z_recv = -5.0 * ones(x_recv.shape)
# z_recv = -50.0 * ones(x_recv.shape)

# # Create sources as a circle around the object, works, and gives better results
# R = 500.0
# n_src = 20
# dtheta = 2 * pi / n_src
# theta = linspace(0.0, 2.0 * pi - dtheta, n_src)
# x_src = R * cos(theta)
# z_src = 150.0 + R * sin(theta)
#
# # Create the receivers, equal to sources for now
# x_recv = x_src
# z_recv = z_src
# n_recv = x_src.size

# temple[0:10*nx] = 2000.0
# if 1:
#     temple[:] = 2000.0
#     temple[5:10][:] = 1900.0
#     temple[10:15][:] = 2100.0
#     temple[15:20] = 2200.0
#     # temple[20*nx:25*nx] = 2250.0
#     temple[25:30][:] = 1850.0


temple_v = temple.reshape(n)
chi = 1.0 - (c_0 / temple_v)**2 #calculate the contrast, this is the true contrast which we want to find using FWI

#save the colorbar limits for proper visualization
v_min = chi.min()
v_max = chi.max()

# plt.figure
# plt.clf()
# plt.subplot(3,1,1)
# plt.imshow(chi.reshape(nz,nx), interpolation='nearest', vmin=v_min, vmax=v_max)
# plt.colorbar()
# plt.title('chi (contrast)')


"""
matA = ones((nz*nx,nz*nx))
b = ones(nz*nx)
prod1 = zeros(nz*nx)
prod2 = zeros(nz*nx)

start_time = time.time()
for i in range(nz*nx):
    for j in range(nz*nx):
    	prod2[i] += matA[i,j]*b[j]
print("--- %s seconds ---" % (time.time() - start_time))

start_time = time.time()
prod1 = dot(matA,b)
print("--- %s seconds ---" % (time.time() - start_time))


G_sm = matA[0:nx,:]
prod = zeros((nz*nx),complex)
dummy1 = zeros((nx),complex)
dW = b

start_time = time.time()
for i in range(nz):
    for j in range(0,nz-i):
        #start_time1 = time.time()
    	dummy1 = dot( G_sm[ : , i*nx : (i+1)*nx ] , dW[ (j+i)*nx : (j+i+1)*nx ] );
    	#print("--- %s seconds ---" % (time.time() - start_time1))
    	#print i, j, dummy1
        prod[ j*nx : (j+1)*nx ] += dummy1
        if ( 2*i + j < nz and i>0 ): 
            prod[ (j+2*i)*nx : (j+2*i+1)*nx ] += dummy1

for i in range(1,nz):
    if (i <= nz-i):
        for j in range(i):
            dummy1 = dot( G_sm[ : , i*nx : (i+1)*nx ], dW[ j*nx : (j+1)*nx ] )
 #           print i, j, dummy1
            prod[ (i+j)*nx : (i+j+1)*nx ] += dummy1
    else:
        for j1 in range(nz-i):
            prod[ (j1+i)*nx : (j1+i+1)*nx ] += dot( G_sm[ : , i*nx : (i+1)*nx ], dW[ j1*nx : (j1+1)*nx ] )
    
print("--- %s seconds ---" % (time.time() - start_time))
"""


# create volume Greens function per frequency,
# including the volume differential
# this is the Greens function for every frequency from every grid point to all other grid points.
# therefore, if there are 100 grid points, the G_xx is 100x100 for every frequency
# this is calculated with the background c and the spatial coordinates of every grid point.
# no prior measurements needed.
# note that this assumes a homogeneous background medium, which is unrealistic
# TODO: inhomogeneous background medium, see thesis 2.5
print "Calculating Green's functions per frequency"
G_xx = zeros((n_freq,n,n), complex)
for i in range(n_freq):
    print "Frequency " + str(i + 1) + " of " + str(n_freq) + " (" + str(f[i]) + " Hz)"
    for k in range(n):
        r = ((xv[k] - xv)**2 + (zv[k] - zv)**2)**0.5
        G_xx[i,k,:] = Greens(k_wave[i], r, omega[i]) * dx * dz * k_wave[i]**2
        # Michiel:
        # Set the Green's function element r=0 to 0.
        # This introduces a small error of order O(dx*dz),
        # since, as far as I can tell, the integrals are only
        # weakly singular and do not have principal values.
        # (Note that this is true for this particular 2D Greens function)
        G_xx[i,k,k] = 0.0
        



if recalculateinput == 1:
    # First we calculate the fields given the known contrast.
    # This we use to extract the recorded dataset afterwards
    p_0 = zeros((n_freq, n_src, n), complex)
    p_tot = zeros((n_freq, n_src, n), complex)

    for i in range(n_freq):

        print "Creating fields for frequency " + str(i+1) + " of " + str(n_freq) + " (" + str(f[i]) + " Hz)"

        for k in range(n_src):
            # calculate p_0 for each source (and frequency)
            rv = ((xv - x_src[k])**2 + (zv - z_src[k])**2)**0.5
            p_0[i,k,:] = Greens(k_wave[i], rv, omega[i])
            # p_0[i, k, :] = Greens(k_wave[i], rv, omega[i]) * ampfactors[i] #* k_wave[i]**2 # * dx * dz
    
    start_time = time.time()      
    for i in range(n_freq):

        print "Creating fields for frequency " + str(i+1) + " of " + str(n_freq) + " (" + str(f[i]) + " Hz)"

        for k in range(n_src):
            
            p_tot[i, k, :] = calcField(G_xx[i, :, :], chi, p_0[i, k, :], maxit2nd, tol=eps_2)
    print("--- %s seconds ---" % (time.time() - start_time))
                        
    # Calculate the total fields at the receivers p_data
    # to calculate this, we need to know the actual pressure field p_tot
    # p_tot describes all sources, and takes all scatterers in the subsurface into account
    # At each place with a secondary source, chi is non-zero
    # The secondary sources then transmit energy through the smooth background medium back at the receivers
    # this energy is represented by the greens function (G_r).
    # create the Greens function
    G_r = zeros((n_recv, n), complex)
    p_data = zeros((n_freq, n_recv, n_src), complex)
    for i in range(n_freq):

        # Create the Greens function for reconstruction, per source
        for j in range(n_recv):
            r = ((x_recv[j] - xv)**2 + (z_recv[j] - zv)**2)**0.5
            G_r[j, :] = Greens(k_wave[i], r, omega[i]) * dx * dz * k_wave[i]**2

        # Calculate the data points
        p_chi = p_tot[i,:,:] * chi
        p_data[i, :, :] = einsum('jl,kl->jk', G_r, p_chi)

    save('./p_data.npy', p_data)
    save('./p_0.npy', p_0)
    save('./p_tot.npy',p_tot)

# new
# chi_p_true = chi * p_tot[n_freq/2,n_src/2,:]
# plt.subplot(3,2,2)
# plt.imshow(real(G_r[n_src/2, :]).reshape(nz,nx), interpolation='nearest')
# plt.colorbar()
# plt.title('G_r')

# This is where the fun starts. In theory everything before this point was measured or given. Some values are chosen.
# p_data = p_data[n_freq, n_recv, n_src]
p_data = load('./p_data.npy')



# p_data = p_data[n_freq, n_src, n]
p_0 = load('./p_0.npy')

p_tot = load('./p_tot.npy')


#save the colorbar limits for proper visualization
# p_min = real(p_0[n_freq/2,n_src/2,:]).min()
# p_max = real(p_0[n_freq/2,n_src/2,:]).max()
# plt.subplot(3,2,4)
# plt.imshow(real(p_0[n_freq/2,n_src/2,:]).reshape(nz,nx), interpolation='nearest', vmin=p_min, vmax=p_max)
# plt.colorbar()
# plt.title('p_0')

# eta scaling factor, Eq. 2.10
eta = 1.0 / norm(p_data)**2
# eta = 1.0 / (norm(p_data)**2 * dx_recv * dx_scr * df)

# initial field is p_0
chi_est = zeros((n))
p_est = copy(p_0)

# p_est and chi_est are the output, so the result of the FWI algorithm
if (read_chi==1):
	chi_est = load(chi_file)
	for i in range(n_freq):
		print "Creating fields for frequency " + str(i+1) + " of " + str(n_freq) + " (" + str(f[i]) + " Hz)"
		print " "
		for k in range(n_src):
			p_est[i, k, :] = calcField(G_xx[i, :, :], chi_est, p_0[i, k, :], maxIt=maxit2nd, tol=eps_2)

	plt.subplot(3,1,1)
	plt.imshow(chi.reshape(nz,nx), interpolation='nearest', vmin=v_min, vmax=v_max)
	plt.colorbar()
	plt.title('chi (contrast)')

chi_p_old = zeros(p_est.shape, complex)
phi = [None] * n_freq
# phi holds the basis functions which increase with each iteration, see eq. 2.29
for i in range(n_freq):
    phi[i] = [list()] * n_src
#so phi contains list of lenght n_src for every frequency

# create the Greens function matrix for reconstruction
# we absorb the volume differential
G_r = zeros((n_freq, n_recv, n), complex)
for i in range(n_freq):
    for j in range(n_recv):
        r = ((x_recv[j] - xv)**2 + (z_recv[j] - zv)**2)**0.5
        G_r[i,j,:] = Greens(k_wave[i], r, omega[i]) * dx * dz * k_wave[i]**2

g_old = None
zeta_old = None

r_old = None  # should never be actually used
res_old = 0.0
chi_old = chi_est

residual_firstit = zeros(maxit2)
residual_innerloop = zeros(maxit2)
b_gradient_zeta = zeros((2,nz,nx))

p_est_old = 0.0*p_est
print norm(p_est_old)

for it in range(n_max):

	# print ' '
	print 'iteration ' + str(it+1) + ' of ' + str(n_max)
	if regularisation == 0:

		K = einsum('ijl,ikl->ijkl', G_r, p_est) #part of eq. 2.6, namely the int( G * p_tot )

		for it2 in range(maxit2):
			r = p_data - einsum('ijkl,l->ijk', K, chi_est)
			#eq 2.7 with the chi added? The einsum would be the K_chi operator

			res = eta * norm(r)**2 #Eq. 2.9, to calculate F^data_n, which is the error functional
			print 'inner loop residual = ' + str(res) + '   ' + str(abs(residual_firstit[it2-1] - res)) + '(' +str(it2+1) + '/' + str(maxit2) + ')'
			residual_firstit[it2] = res
			if it2 > 0 and (res < tol2 or abs(residual_firstit[it2-1] - res) < tol2):
				break

			#now we find the gradient
			# see formula in the full waveform inversion document of Michiel
			tmp = einsum('ijkl,ijk->l', conj(K), r)
			g = 2.0 * eta * real(tmp)

			if it2 == 0:
				zeta = copy(g) #eq. 2.12, first part, this is g1 and zeta1, this is the update direction
			else:
				gamma = dot(g, g - g_old) / (norm(g_old) ** 2)  # eq. 2.14, polak-ribiere direction, note that dot(g, g-g_old) is the same as <g, g-g_old>_D
				zeta = g + gamma * zeta_old  # eq. 2.12, update direction, or eq. 19 from van den berg 2004

			# now calculate K_zeta for use in eq. 2.17
			K_zeta = einsum('ijkl,l->ijk', K, zeta)

			alpha_num = real(einsum('ijk,ijk', conj(r), K_zeta))
			alpha_den = real(einsum('ijk,ijk', K_zeta, conj(K_zeta))) # eq. 2.17

			alpha = alpha_num / alpha_den #eq 2.19, optimal step size
			#import ipdb; ipdb.set_trace()
			#print 'alpha = ' + str(alpha) + '\t(' + str(alpha_num) + '/' + str(alpha_den) + ')'
			chi_est = chi_est + alpha * zeta

			zeta_old = zeta
			g_old = g

		# new for the regularization scheme
		Fdata_old = 1 #res
		plt.figure(it)
		plt.clf()
		plt.subplot(3, 1, 1)
		plt.imshow(chi.reshape(nz, nx), interpolation='nearest', vmin=v_min, vmax=v_max)
		plt.colorbar()
		plt.title('chi (contrast), nfreq: '+ str(n_freq))

		plt.subplot(3, 1, 2)
		plt.imshow(chi_est.reshape(nz, nx), interpolation='nearest', vmin=v_min, vmax=v_max)
		# plt.imshow(real(p_est[:,:,1]), interpolation='nearest')
		plt.colorbar()
		plt.title('chi new ' + 'it: ' + str(it+1))
		# plt.title('p')

		# plt.subplot(3, 1, 3)
		# plt.plot(linspace(1, maxit2, maxit2), residual_innerloop)
		# plt.ylabel('residual inner loop')
		# plt.xlabel('iteration')


		plt.subplot(3, 1, 3)
		plt.imshow(real(G_xx[n_freq / 2]), interpolation='nearest')
		plt.title('G_xx freq ' + str(f[n_freq/2]))
		plt.colorbar()


		plt.show(block=False)
		plt.savefig('./Chi_est_it' + str(it).zfill(2) + '.png', dpi=400)
		
		string1 = "chi_est" + "_" + str(it)
		save(string1, chi_est)
	else:
		# TODO from here, add multiplicative regularisation from section 2.3.3 of the thesis
		
		deltasquared_old = 0.0
		bsquared_old = 0.0
		Freg_old = 1.0
		
		# const = 10.0/(30.0*it+1.0)
		const = 100.0 / (10.0 * it + 1.0)

							
		K = einsum('ijl,ikl->ijkl', G_r, p_est) # part of eq. 2.6, namely the int( G * p_tot )
		for it2 in range(maxit2):

			print str(it2 + 1) + '/' + str(maxit2) + ' (' + str(it + 1) + '/' + str(n_max) + ')'
			r = p_data - einsum('ijkl,l->ijk', K, chi_est)  # eq 2.7
			# this is also the first equation on p. 2 of Michiels document
			r_old = copy(r)
			res = (real(eta * einsum('ijk,ijk', conj(r), r)))  # eq. 2.9, res = Fdata_n	
			Fdata_old = res
			print 'residual = ' + str(res)
			residual_innerloop[it2] = res
			
#			if it2 > 1:
#				if it2 > 10 and (res < tol2 or ( abs(residual_innerloop[it2-2] - res) < tol2 and abs(residual_innerloop[it2-1] - res) < tol2 )):# or res > 1.0:
#					break

			if it2 > 50 and (res < tol2 or ( abs(residual_innerloop[it2-2] - res) < tol2 and abs(residual_innerloop[it2-1] - res) < tol2 )):# or res > 1.0:
				break

			gradient_chi_old = gradient(chi_old.reshape(nz, nx), dz, dx, edge_order=2)
			gradient_chi_old_normsquared = gradient_chi_old[0] ** 2 + gradient_chi_old[1] ** 2

			if it2 == 0:
				g = real(eta * (einsum('ijkl,ijk', conj(K), r)))
				zeta = copy(g) #eq. 2.12, first part, this is g1 and zeta1, this is the update direction
				bsquared = zeros((nz, nx))
				Fdata_old = copy(res)
				# now calculate K_zeta for use in eq. 2.17
				K_zeta = einsum('ijkl,l->ijk', K, zeta)

				alpha_num = real(einsum('ijk,ijk', conj(r), K_zeta))
				alpha_den = real(einsum('ijk,ijk', K_zeta, conj(K_zeta)))  # eq. 2.17

				alpha = alpha_num / alpha_den  # eq 2.19, optimal step size
				chi_est = chi_est + alpha * zeta
				chi_old = copy(chi_est)
				zeta_old = copy(zeta)
				
				g_old = copy(g)
				#import ipdb; ipdb.set_trace()
			else:
				# first apply eq 2.24 to get g, which uses b^2, defined in eq. 2.22, which uses deltasquared, defined in eq. 2.23
				# according to abubakar, delta squared is a positive steering parameter, if large, Freg should become 1
				bsquared = (1.0 / domain_area) * (1.0 / (gradient_chi_old_normsquared + deltasquared_old * ones((nz, nx))))  # eq. 2.22, positive weight TODO
				b = sqrt(bsquared)# according to Abubakar b is a positive weight

				deltasquared = const*0.5*sum( sum( (b*gradient_chi_old[0])**2 + (b*gradient_chi_old[1])**2 ) ) / ( sum( sum(bsquared) ) )#const*(1.0 / domain_area ) * einsum('ij,ij', gradient_chi_old_normsquared, ones((nz, nx))) * dx * dz   #   #  # eq. 2.23, thesis method, used in next iteration TODO
				
				# print 'delta**2=' + str(deltasquared)

				g_reg = gradient(bsquared_old * gradient_chi_old[0], dz, dx, edge_order=2)[0] + gradient(bsquared_old*gradient_chi_old[1], dz, dx, edge_order=2)[1]  # eq. 2.24 TODO
				g = real(eta * (einsum('ijkl,ijk', conj(K), r)) * Freg_old + (Fdata_old * g_reg).reshape(nx*nz))  # eq. 2.25 TODO factor 2 here?
				# g = real(eta * (einsum('ijkl,ijk', conj(K), r))) + (Fdata_old / Freg_old) * g_reg.reshape(nx * nz)  # eq. 2.25 TODO factor 2 here?
				gamma = dot(g, g - g_old) / (norm(g_old) ** 2)  # eq. 2.14, polak-ribiere direction, note that dot(g, g-g_old) is the same as <g, g-g_old>_D
				zeta = g + gamma * zeta_old  # eq. 2.12, update direction, or eq. 19 from van den berg 2004

				# then we calculate F_n^tot' = 0, which is the derivative of eq. 2.26 (expanded in 2.27)
				K_zeta = einsum('ijkl,l->ijk', K, zeta)  # used in A2 and A1 of eq. 2.27

				# now we want to calculate the new alpha
				# first: calculate A2, A1, A0, B2, B1, B0 of eq. 2.27
				A2 = eta * real(einsum('ijk,ijk', K_zeta, conj(K_zeta)))  # real? should be correct, TODO
				A1 = - 2.0 * eta * real(einsum('ijk,ijk', conj(r_old), K_zeta))  # TODO must always be negative?
				A0 = copy(Fdata_old)  # or instead of norm: einsum('ijk,ijk', r, conj(r)) with a real TODO
				# A0 = real(Fdata_old) # TODO: check if it's the same
		
				gradient_zeta = gradient(zeta.reshape(nz, nx), dz, dx, edge_order=2)
				b_gradient_zeta[0] = (b*gradient_zeta[0])
				b_gradient_zeta[1] = b*gradient_zeta[1]

				B2 = sum(sum(b_gradient_zeta[0]**2 + b_gradient_zeta[1]**2))*dx*dz  # real(einsum('ij,ij', bsquared,gradient_zeta_normsquared))  # same as sum( ((b*gradient_zeta[0])**2+(b*gradient_zeta[1])**2).reshape(nz*nx) )
			   
				b_gradient_chi_old = einsum('ij,lij->lij', b, gradient_chi_old)
				B1 = 2.0 * einsum('lij,lij', b_gradient_chi_old, b_gradient_zeta )*dx*dz# 2.0*sum( sum( b_gradient_chi_old[0]*b_gradient_zeta[0] + b_gradient_chi_old[1]*b_gradient_zeta[1] ) )*dx*dz #
			
				B0 = ( sum( sum(b_gradient_chi_old[0]**2 + b_gradient_chi_old[1]**2 ) ) + deltasquared_old*sum(sum(bsquared)) ) * dx * dz
				
				# 2.26 expanded is A2*B2*alpha^4 + A2*B1*alpha^3 + A1*B2*alpha^3 + A2*B0*alpha^2 + A1*B1*alpha^2 + A0*B2*alpha^2 + A1*B0*alpha + A0*B1*alpha + A0*B0
				# derivative: 4*A2*B2*alpha^3 + 3*A2*B1*alpha^2 + 3*A1*B2*alpha^2 + 2*A2*B0*alpha + 2*A1*B1*alpha + 2*A0*B2*alpha + A1*B0 + A0*B1
				# simplified derivative: 4*A2*B2*alpha^3 + ( 3*A2*B1 + 3*A1*B2 )*alpha^2 + ( 2*A2*B0 + 2*A1*B1 + 2*A0*B2 )*alpha + A1*B0 + A0*B1
				# or in other words a * alpha^3 + b * alpha^2 + c * alpha + d
				# with
				der_a = 4.0 * A2 * B2
				der_b = 3.0 * (A2 * B1 + A1 * B2)
				der_c = 2.0 * (A2 * B0 + A1 * B1 + A0 * B2)
				der_d = A1 * B0 + A0 * B1
				# next we want to calculate a * alpha^3 + b * alpha^2 + c * alpha + d = 0
				# this has 3 roots, 2 imaginary and 1 real, and the real one is desired
				talpha = roots([der_a, der_b, der_c, der_d])
				talpha2 = copy(talpha)
				talpha = (real(talpha)[2])
				# print 'alpha regularisation = ' + str(talpha)
				# !

				alpha = copy(talpha)
				chi_est = chi_est + alpha * zeta
				chi_old = copy(chi_est)
				
				r = p_data - einsum('ijkl,l->ijk', K, chi_est)  # eq 2.7
				# this is also the first equation on p. 2 of Michiels document
				res = (real(eta * einsum('ijk,ijk', conj(r), r)))  # eq. 2.9, res = Fdata_n
			
				gradient_chi = gradient(chi_est.reshape(nz, nx), dz, dx, edge_order=2)
				gradient_chi_normsquared = gradient_chi[0] ** 2 + gradient_chi[1] ** 2

				Fdata_old = copy(res)
				print 'Fdata = ' + str(Fdata_old)
				division = ((gradient_chi_normsquared + deltasquared_old) / (gradient_chi_old_normsquared + deltasquared_old) )
				Freg_old = (1.0 / domain_area ) * einsum('ij,ij', division, ones((nz, nx))) * dx * dz
				# print 'Freg = ' + str(Freg_old) + '  ' + str(norm(chi_est))
				
#				print B2, B1, B0, domain_area, B2*alpha**2 + B1*alpha + B0, A2*alpha**2 + A1*alpha + A0, norm(bsquared)**2, deltasquared_old 
				# print 12.0*A2*B2*talpha2**2 + 6.*(A2*B1+A1*B2)*talpha2 + 2.*(A0*B2+A1*B1+B0*A2)
				#ipdb.set_trace()

				zeta_old = copy(zeta)
				g_old = copy(g)
				r_old = copy(r)
				bsquared_old = copy(bsquared)
				# without noise, the reg factor should be large in the beginning and approach 1 over the iterations
				deltasquared_old = copy(deltasquared)

		plt.figure(it)
		plt.clf()
		plt.subplot(3, 1, 1)
		plt.imshow(chi.reshape(nz, nx), interpolation='nearest', vmin=v_min, vmax=v_max)
		plt.colorbar()
		plt.title('chi (contrast), nfreq: '+ str(n_freq))

		plt.subplot(3, 1, 2)
		plt.imshow(chi_est.reshape(nz, nx), interpolation='nearest', vmin=v_min, vmax=v_max)
		# plt.imshow(real(p_est[:,:,1]), interpolation='nearest')
		plt.colorbar()
		plt.title('chi new ' + 'it: ' + str(it+1))
		# plt.title('p')

		# plt.subplot(3, 1, 3)
		# plt.plot(linspace(1, maxit2, maxit2), residual_innerloop)
		# plt.ylabel('residual inner loop')
		# plt.xlabel('iteration')


		plt.subplot(3, 1, 3)
		plt.imshow(real(G_xx[n_freq / 2]), interpolation='nearest')
		plt.title('G_xx freq ' + str(f[n_freq/2]))
		plt.colorbar()


		plt.show(block=False)
		plt.savefig('./Chi_est_it' + str(it).zfill(2) + '.png', dpi=400)
		
		string1 = "chi_est" + "_" + str(it)
		save(string1, chi_est)


    # update field, per frequency and source
    # we now have the update direction and size, and have applied it to get the new chi
    # time to update the total field following paragraph 2.4
	for i in range(n_freq):
		print "Creating fields for frequency " + str(i+1) + " of " + str(n_freq) + " (" + str(f[i]) + " Hz)"
		print " "
		for k in range(n_src):
			p_est[i, k, :] = calcField(G_xx[i, :, :], chi_est, p_0[i, k, :], maxIt=maxit2nd, tol=eps_2)
				

    # update p_tot field using the non linear field update
    # from paragraph 2.4



# plt.subplot(3,2,2)
# # plt.imshow(real(p_est[n_freq/2,n_src/2,:]).reshape(nz,nx), interpolation='nearest', vmin=p_min, vmax=p_max)
# # plt.imshow(real(chi_p_old[1,n_src/2,:]).reshape(nz,nx), interpolation='nearest')
# plt.imshow(real(r[1,:,:]), interpolation='nearest')
# plt.colorbar()
# # plt.title('p_tot new')
# plt.title('r')
#
# plt.subplot(3,2,4)
# # plt.imshow(real(p_est[n_freq/2,n_src/2,:]).reshape(nz,nx), interpolation='nearest', vmin=p_min, vmax=p_max)
# # plt.imshow(real(chi_p_old[n_freq/2,n_src/2,:]).reshape(nz,nx), interpolation='nearest')
# plt.imshow(real(r[n_freq/2,:,:]), interpolation='nearest')
# plt.colorbar()
# # plt.title('p_tot new')
# plt.title('r')
#
# plt.subplot(3,2,6)
# # plt.imshow(real(p_est[n_freq/2,n_src/2,:]).reshape(nz,nx), interpolation='nearest', vmin=p_min, vmax=p_max)
# # plt.imshow(real(chi_p_old[-1,n_src/2,:]).reshape(nz,nx), interpolation='nearest')
# plt.imshow(real(r[-1,:,:]), interpolation='nearest')
# plt.colorbar()
# # plt.title('p_tot new')
# plt.title('r')




# plt.savefig(pp, format='pdf') #doesn't work
plt.savefig('./result.png', dpi=400)
pp.close()
save('./chi_est.npy', chi_est)
save('./p_est.npy', p_est)
plt.show()





# plt.savefig(pp, format='pdf')

