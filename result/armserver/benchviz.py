#! /usr/bin/python

import sys
import matplotlib.pyplot as plt
import pandas as pd

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print('usage: ', sys.argv[0], ' DATA_CSV [DATA_CSV...] OUTPUT_PNG\n')
        sys.exit()

    fig, ax = plt.subplots()
    ax.set_title('threads benchmark [128x128x128 cells, 128 ticks]')
    ax.set_xlabel('threads count')
    ax.set_ylabel('GCells/second')

    for idx in range(1, len(sys.argv) - 1):
        name = str(sys.argv[idx])
        bench = pd.read_csv(name)
        ax.plot(bench['threads'], bench['gcells'], label=name.split(".")[0])

    ax.set_ylim(bottom=0, top=None)
    ax.legend()

    plt.savefig(str(sys.argv[len(sys.argv) - 1]))

