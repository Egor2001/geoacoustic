#ifndef GEOACOUSTIC_STATIC_CONTEXT_HPP_
#define GEOACOUSTIC_STATIC_CONTEXT_HPP_

#include "types.hpp"
#include "volume_array.hpp"

namespace geo {

// TODO: to add PML params
struct Config
{
    int3_t grid_size;
    int_t steps_cnt;

    real_t dspace;
    real_t dtime;

    VolumeArray<real_t> bulk; // bulk modulus ~ K
};

struct Context
{
    VolumeArray<real_t> ampl, next_ampl; // amplitude 
    VolumeArray<real_t> x1, y1, z1; // axis derivatives
};

} // namespace geo

#endif // GEOACOUSTIC_STATIC_CONTEXT_HPP_
