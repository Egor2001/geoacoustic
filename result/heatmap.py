#! /usr/bin/python

import sys
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print('usage: ', sys.argv[0], ' INPUT_FILE OUTPUT_PNG\n')
        sys.exit()

    mtx = np.loadtxt(str(sys.argv[1]))
    row_cnt, col_cnt = mtx.shape

    x = np.linspace(0.0, 1.0, col_cnt)
    y = np.linspace(0.0, 1.0, row_cnt)

    fig, ax = plt.subplots()

    im = ax.imshow(mtx, cmap='hot', interpolation='nearest')
    ax.figure.colorbar(im, ax=ax)

    plt.savefig(str(sys.argv[2]))
