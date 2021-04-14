#!/usr/bin/python3

TYPE_A, TYPE_B, TYPE_C, TYPE_I = 0, 1, 2, 3

TO = [
    [TYPE_I, TYPE_A, TYPE_B],
    [TYPE_B, TYPE_B, TYPE_B],
    [TYPE_B, TYPE_C, TYPE_I],
    [TYPE_I, TYPE_I, TYPE_I],
]

def gen(out, rank, x, y, z, xt, yt, zt, tm=0):
    sz = 1 << rank
    hf = sz // 2
    less = rank - 1

    if xt == TYPE_I or yt == TYPE_I or zt == TYPE_I:
        return

    if rank == 0:
        if xt == TYPE_A or yt == TYPE_A or zt == TYPE_A:
            return

        if tm % 2 == 1:
            out.write("PROC_NEG_CELL_({}, {}, {});\n".format(x, y, z))
        else:
            out.write("PROC_POS_CELL_({}, {}, {});\n".format(x, y, z))
    else:
        tm1 = tm

        gen(out, less, x+hf, y+hf, z+hf, TO[xt][1], TO[yt][1], TO[zt][1], tm1)
        gen(out, less, x   , y+hf, z+hf, TO[xt][0], TO[yt][1], TO[zt][1], tm1)
        gen(out, less, x+hf, y   , z+hf, TO[xt][1], TO[yt][0], TO[zt][1], tm1)
        gen(out, less, x+hf, y+hf, z   , TO[xt][1], TO[yt][1], TO[zt][0], tm1)

        gen(out, less, x+hf, y   , z   , TO[xt][1], TO[yt][0], TO[zt][0], tm1)
        gen(out, less, x   , y+hf, z   , TO[xt][0], TO[yt][1], TO[zt][0], tm1)
        gen(out, less, x   , y   , z+hf, TO[xt][0], TO[yt][0], TO[zt][1], tm1)
        gen(out, less, x   , y   , z   , TO[xt][0], TO[yt][0], TO[zt][0], tm1)

        tm2 = tm + hf

        gen(out, less, x+sz, y+sz, z+sz, TO[xt][2], TO[yt][2], TO[zt][2], tm2)
        gen(out, less, x+hf, y+sz, z+sz, TO[xt][1], TO[yt][2], TO[zt][2], tm2)
        gen(out, less, x+sz, y+hf, z+sz, TO[xt][2], TO[yt][1], TO[zt][2], tm2)
        gen(out, less, x+sz, y+sz, z+hf, TO[xt][2], TO[yt][2], TO[zt][1], tm2)
                                                                       
        gen(out, less, x+sz, y+hf, z+hf, TO[xt][2], TO[yt][1], TO[zt][1], tm2)
        gen(out, less, x+hf, y+sz, z+hf, TO[xt][1], TO[yt][2], TO[zt][1], tm2)
        gen(out, less, x+hf, y+hf, z+sz, TO[xt][1], TO[yt][1], TO[zt][2], tm2)
        gen(out, less, x+hf, y+hf, z+hf, TO[xt][1], TO[yt][1], TO[zt][1], tm2)

for rank in [1, 2]:
    for xt in [TYPE_A, TYPE_B, TYPE_C]:
        for yt in [TYPE_A, TYPE_B, TYPE_C]:
            for zt in [TYPE_A, TYPE_B, TYPE_C]:
                with open("proc_tile_{}{}{}_rank{}.hpp" \
                        .format(xt, yt, zt, rank), 'w') as out:
                    gen(out, rank, 0, 0, 0, xt, yt, zt)

