import numpy as np
import matplotlib.pyplot as plt

plaatje = np.zeros((64, 128))


def ctoChi(c):
    c0 = 2000
    return 1 - (c0/c)*(c0/c)


for i in np.arange(0, 128):
    for j in np.arange(0, 64):
        x = i+20
        y = j

        if (x % 16 > 8 and not (x < 50 or x > 115) and not (y < 24 or y > 39)):
            plaatje[j][i] = ctoChi(2200)
        elif (i > 10 and i < 117 and y > 45 and y < 52):
            plaatje[j][i] = ctoChi(2200)
        elif (i > 30 and i < 97 and y > 5 and y < 21):
            plaatje[j][i] = ctoChi(2200)
        else:
            plaatje[j][i] = ctoChi(2000)
        if (i > 58 and i < 70 and j > 15 and j < 18):
            plaatje[j][i] = ctoChi(2000)
        if (i-3*j > 35):
            plaatje[j][i] = ctoChi(2000)
        if (i+3*j < 91):
            plaatje[j][i] = ctoChi(2000)


plaatje.resize(128*64)


with open('temple128x64.txt', 'w') as f:
    for entry in plaatje:
        f.write(str(entry))
        f.write('\n')

plaatje = plaatje.reshape((64, 128))

plt.imshow(plaatje, interpolation='nearest', vmin=0, vmax=0.2)
plt.colorbar()
