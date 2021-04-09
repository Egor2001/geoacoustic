#! /usr/bin/python

import sys
import matplotlib.pyplot as plt
import pandas as pd

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print('usage: ', sys.argv[0], ' DATA_CSV OUTPUT_PNG\n')
        sys.exit()

    name = str(sys.argv[1])
    bench = pd.read_csv(name)

    fig, ax = plt.subplots()
    ax.set_title('threads benchmark [512x512x512 cells, 512 ticks]')
    ax.set_xlabel('threads count')
    ax.set_ylabel('GCells/second')

    bound = bench['threads'] * (512**4 / (1e9 * bench['time'][0]))
    ax.plot(bench['threads'], bound, color='grey', label='bound')

    ax.plot(bench['threads'], 512**4 / (1e9 * bench['time']), 
            color='red', label=name.split(".")[0])

    ax.legend()

    plt.savefig(str(sys.argv[2]))

