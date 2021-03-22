#!/usr/bin/python

import sys
import timeit as tm
import subprocess as subpc

if __name__ != "__main__" or len(sys.argv) != 2:
    print("USAGE: ", sys.argv[0], " BINDIR")

bindir = sys.argv[1]

def bench(name, grid, steps):
    start = tm.default_timer()
    subpc.run([bindir + "/" + name, str(grid), str(steps)])
    end = tm.default_timer()
    return end - start

ncells = 2 ** 26;
# grids = [
#     16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256
#     ]

grids = [16, 32, 64, 80, 96, 112, 128, 144, 160]

# static_cfg = [
#         ("linear", 0), ("linear", 1), ("linear", 2), ("linear", 3), 
#         ("linear", 4), ("vector", 0), ("vector", 1), ("vector", 2),
#         ("zcube2", 0), ("zcube2", 1), ("zcube2", 2), ("zcube2", 3)
#         ]

# static_cfg = [("vector", 0), ("linear", 4), ("zcube2", 0), ("zcube2", 3)]
# static_cfg = [("linear", 4), ("vector", 2), ("zcube2", 3)]
static_cfg = [("linear", 0),]

for (tile, rank) in static_cfg:
    name = tile + "_" + str(rank)
    print(name)
    for grid in grids:
        print(grid, " -> ", bench(name, grid, ncells / (grid ** 3)))
