#ifndef GEOACOUSTIC_STATIC_CONFIG_HPP_
#define GEOACOUSTIC_STATIC_CONFIG_HPP_

#include <iostream>
#include <functional>

#include "types.hpp"
#include "volume_array.hpp"

namespace geo {

// TODO: to add PML params
template<typename TC>
struct Config
{
public:
    using TCell = TC;

    Config(int3_t real_grid_size, int_t real_steps_cnt):
        grid_size{ 
            real_grid_size.x >> TCell::NRankX, 
            real_grid_size.y >> TCell::NRankY, 
            real_grid_size.z >> TCell::NRankZ
        },
        steps_cnt{real_steps_cnt},
        bulk(grid_size),
        rho(grid_size)
    {}

public:
    int3_t grid_size{};
    int_t steps_cnt{};

    real_t dspace{0.0};
    real_t dtime{0.0};

    VolumeArray<TCell> bulk; // bulk modulus (K)
    VolumeArray<TCell> rho; // density
};

} // namespace geo

#endif // GEOACOUSTIC_STATIC_CONFIG_HPP_
