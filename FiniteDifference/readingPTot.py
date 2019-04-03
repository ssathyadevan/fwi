import numpy as np
import matplotlib.pyplot as plt

a = np.genfromtxt('pTotTest.csv', delimiter='/n', dtype=complex)
imageCpp1 = a.reshape(32,64)
plt.imshow(np.real(imageCpp1))
plt.colorbar()