import numpy as np
import matplotlib.pyplot as plt

plaatje = np.zeros((32,64))

def ctoChi(c):
    c0 = 500
    return 1 - (c0/c)*(c0/c)

for i in np.arange(0,64):
    for j in np.arange(0,32):
        x = i-32
        y = j-16
        
        if (x+y)%15 <= 3 or  (x-y)%15 <= 3:
            plaatje[j][i] = ctoChi(500)
        else:
            plaatje[j][i] = ctoChi(520)
        
plaatje.resize(64*32)


with open('ruitjes.txt', 'w') as f:
    for entry in plaatje:
        f.write(str(entry))
        f.write('\n')

plaatje = plaatje.reshape((32,64))

plt.imshow(plaatje, interpolation='nearest', vmin=0, vmax=0.1)