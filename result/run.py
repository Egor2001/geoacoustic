#!/usr/bin/python

import sys
import timeit as tm
import subprocess as subpc
import numpy as np
import matplotlib.pyplot as plt

if __name__ != "__main__" or len(sys.argv) != 3:
    print("USAGE: ", sys.argv[0], " BINDIR FILENAME")

bindir = sys.argv[1]
filename = sys.argv[2]

def bench(name, grid, steps):
    start = tm.default_timer()
    subpc.run([bindir + "/" + name, str(grid), str(steps)])
    end = tm.default_timer()
    return end - start

ncells = 2 ** 29;
# grids = np.array([
#     16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256
#     ], dtype=np.uint)

grids = np.array([16, 32, 64, 80, 96, 112, 128, 144, 160], dtype=np.uint)

# static_cfg = [
#         ("linear", 0), ("linear", 1), ("linear", 2), ("linear", 3), 
#         ("linear", 4), ("vector", 0), ("vector", 1), ("vector", 2),
#         ("zcube2", 0), ("zcube2", 1), ("zcube2", 2), ("zcube2", 3)
#         ]

static_cfg = [("vector", 0), ("vector", 2), ("zcube2", 0), ("zcube2", 3), ("linear", 0), ("linear", 4)]
# static_cfg = [("linear", 4), ("vector", 2), ("zcube2", 3)]
# static_cfg = [("vector", 2),]

fig, ax = plt.subplots()
ax.set_title("time benchmark, " + str(ncells) + " cells")

for (tile, rank) in static_cfg:
    name = tile + "_" + str(rank)
    times = np.array(
            [bench(name, grid, ncells / (grid ** 3)) for grid in grids])
    ax.plot(grids, times, label=name)

ax.set_xlabel("grid, nodes")
ax.set_ylabel("time, sec")
ax.set_ylim([0, None])

ax.legend()
plt.savefig(filename)
