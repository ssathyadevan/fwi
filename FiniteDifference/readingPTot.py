import numpy as np
import matplotlib.pyplot as plt

a = np.genfromtxt('totalfield.csv', delimiter='/n', dtype=complex)
imageCpp1 = a.reshape(32,64)
ax=plt.imshow(np.real(imageCpp1))
plt.colorbar()

b = np.genfromtxt('totalfieldFD.csv', delimiter='/n', dtype=complex)
imageCpp2 = b.reshape(32,64)
#bx=plt.imshow(np.real(imageCpp1))
#plt.colorbar()

c = np.genfromtxt('totalfieldFDPoint.csv', delimiter='/n', dtype=complex)
imageCpp3 = c.reshape(32,64)
#ax=plt.imshow(np.real(imageCpp3))
#plt.colorbar()

d = np.genfromtxt('totalfieldNEWPML.csv', delimiter='/n', dtype=complex)
imageCpp4 = d.reshape(32,64)

#imageCpp1[16,31] = 0
#imageCpp2[16,31] = 0
#imageCpp3[16,31] = 0

#chiRef = np.genfromtxt('totalfield.csv', delimiter='/n', dtype=complex)

#        "x": -4.761904761904759,
#        "z": 154.83870967741936