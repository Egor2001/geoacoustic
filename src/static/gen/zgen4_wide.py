#!/usr/bin/python3

def pack(x, y, z):
    zidx  = 0
    zidx = zidx | ((x & 1) << 0) | ((x & 2) << 2)
    zidx = zidx | ((y & 1) << 1) | ((y & 2) << 3)
    zidx = zidx | ((z & 1) << 2) | ((z & 2) << 4)
    return zidx

def unpack(idx): 
    x = ((idx & int("0b000001", 2)) >> 0) | ((idx & int("0b001000", 2)) >> 2)
    y = ((idx & int("0b000010", 2)) >> 1) | ((idx & int("0b010000", 2)) >> 3)
    z = ((idx & int("0b000100", 2)) >> 2) | ((idx & int("0b100000", 2)) >> 4)
    return x, y, z

def at(coord, idx):
    return "{},{}".format(coord, idx)

for idx in range(2 ** 6 - 1, -1, -1):
    x, y, z = unpack(idx)

    x_dec, x_inc, x_dec2, x_inc2 = str(), str(), str(), str()
    y_dec, y_inc, y_dec2, y_inc2 = str(), str(), str(), str()
    z_dec, z_inc, z_dec2, z_inc2 = str(), str(), str(), str()

    if (x == 0): 
        x_dec = at(-1, pack(3, y, z))
        x_inc = at(0, pack(1, y, z))
        x_dec2 = at(-1, pack(2, y, z))
        x_inc2 = at(0, pack(2, y, z))
    elif (x == 1): 
        x_dec = at(0, pack(0, y, z))
        x_inc = at(0, pack(2, y, z))
        x_dec2 = at(-1, pack(3, y, z))
        x_inc2 = at(0, pack(3, y, z))
    elif (x == 2): 
        x_dec = at(0, pack(1, y, z))
        x_inc = at(0, pack(3, y, z))
        x_dec2 = at(0, pack(0, y, z))
        x_inc2 = at(+1, pack(0, y, z))
    elif (x == 3): 
        x_dec = at(0, pack(2, y, z))
        x_inc = at(+1, pack(0, y, z))
        x_dec2 = at(0, pack(1, y, z))
        x_inc2 = at(+1, pack(1, y, z))
    else:
        raise Exception("x is out of range [0, 3]")

    if (y == 0): 
        y_dec = at(-1, pack(x, 3, z))
        y_inc = at(0, pack(x, 1, z))
        y_dec2 = at(-1, pack(x, 2, z))
        y_inc2 = at(0, pack(x, 2, z))
    elif (y == 1):
        y_dec = at(0, pack(x, 0, z))
        y_inc = at(0, pack(x, 2, z))
        y_dec2 = at(-1, pack(x, 3, z))
        y_inc2 = at(0, pack(x, 3, z))
    elif (y == 2):
        y_dec = at(0, pack(x, 1, z))
        y_inc = at(0, pack(x, 3, z))
        y_dec2 = at(0, pack(x, 0, z))
        y_inc2 = at(+1, pack(x, 0, z))
    elif (y == 3):
        y_dec = at(0, pack(x, 2, z))
        y_inc = at(+1, pack(x, 0, z))
        y_dec2 = at(0, pack(x, 1, z))
        y_inc2 = at(+1, pack(x, 1, z))
    else:
        raise Exception("y is out of range [0, 3]")

    if (z == 0): 
        z_dec = at(-1, pack(x, y, 3))
        z_inc = at(0, pack(x, y, 1))
        z_dec2 = at(-1, pack(x, y, 2))
        z_inc2 = at(0, pack(x, y, 2))
    elif (z == 1):                   
        z_dec = at(0, pack(x, y, 0))
        z_inc = at(0, pack(x, y, 2))
        z_dec2 = at(-1, pack(x, y, 3))
        z_inc2 = at(0, pack(x, y, 3))
    elif (z == 2):                   
        z_dec = at(0, pack(x, y, 1))
        z_inc = at(0, pack(x, y, 3))
        z_dec2 = at(0, pack(x, y, 0))
        z_inc2 = at(+1, pack(x, y, 0))
    elif (z == 3):                   
        z_dec = at(0, pack(x, y, 2))
        z_inc = at(+1, pack(x, y, 0))
        z_dec2 = at(0, pack(x, y, 1))
        z_inc2 = at(+1, pack(x, y, 1))
    else:
        raise Exception("z is out of range [0, 3]")

    print("PROC_STENCIL_({}, {},{},{},{}, {},{},{},{}, {},{},{},{});" \
            .format(idx, \
                    x_dec, x_inc, x_dec2, x_inc2, \
                    y_dec, y_inc, y_dec2, y_inc2, \
                    z_dec, z_inc, z_dec2, z_inc2))

