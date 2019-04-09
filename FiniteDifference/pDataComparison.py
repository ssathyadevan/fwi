#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr  4 10:08:35 2019

@author: leonard
"""

import numpy as np
import matplotlib.pyplot as plt

nsrc  = 17
nrecv = 17
nfreq = 10

fd = np.loadtxt('FiniteDifferencedefaultInvertedChiToPressure.txt', delimiter=',')
fd = fd[:,0] + 1.0j*fd[:,1]

normal = np.genfromtxt('IntegraldefaultInvertedChiToPressure.txt', delimiter=',')
normal = normal[:,0] + 1.0j*normal[:,1]

print("Relative   2-norm normal vs. fd: %0.2f%%" % (100.*np.linalg.norm(normal-fd) / np.linalg.norm(normal)))
print("Relative   1-norm normal vs. fd: %0.2f%%" % (100.*np.linalg.norm(normal-fd, ord=1) / np.linalg.norm(normal, ord=1)))
print("Relative Inf-norm normal vs. fd: %0.2f%%" % (100.*np.linalg.norm(normal-fd, ord=np.inf) / np.linalg.norm(normal, ord=np.inf)))

plt.plot(np.real(fd))
plt.plot(np.real(normal))
plt.legend(['FD','Normal'])