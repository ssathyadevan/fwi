import numpy as np
import matplotlib.pyplot as plt

plaatje = np.zeros((64, 128))


def ctoChi(c):
    c0 = 500
    return 1 - (c0/c)*(c0/c)


for i in np.arange(0, 128):
    for j in np.arange(0, 64):
        x = i-32
        y = j-16

        if (x+y) % 30 <= 6 or (x-y) % 30 <= 6:
            plaatje[j][i] = ctoChi(500)
        else:
            plaatje[j][i] = ctoChi(520)

plaatje.resize(128*64)


with open('ruitjes.txt', 'w') as f:
    for entry in plaatje:
        f.write(str(entry))
        f.write('\n')

plaatje = plaatje.reshape((64, 128))

plt.imshow(plaatje, interpolation='nearest', vmin=0, vmax=0.1)
