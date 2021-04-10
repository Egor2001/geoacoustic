#ifndef GEOACOUSTIC_STATIC_CONFIG_HPP_
#define GEOACOUSTIC_STATIC_CONFIG_HPP_

#include <iostream>
#include <functional>

#include "types.hpp"
#include "volume_array.hpp"

namespace geo {

template<typename TC, int_t NGhost>
struct GeneralizedConfig;

template<typename TC, int_t NGhost>
struct SimplifiedConfig;

#if defined(GEO_SIMPLIFIED_CELL)
template<typename TCell, int_t NGhost = 1u>
using Config = SimplifiedConfig<TCell, NGhost>;
#else
template<typename TCell, int_t NGhost = 1u>
using Config = GeneralizedConfig<TCell, NGhost>;
#endif

// TODO: to add PML params
template<typename TC, int_t NGhost = 1u>
struct GeneralizedConfig
{
public:
    using TCell = TC;

    GeneralizedConfig(int3_t real_grid_size, int_t real_steps_cnt):
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

    VolumeArray<TCell, NGhost> bulk; // bulk modulus (K)
    VolumeArray<TCell, NGhost> rho; // density
};

template<typename TC, int_t NGhost = 1u>
struct SimplifiedConfig
{
public:
    using TCell = TC;

    SimplifiedConfig(int3_t real_grid_size, int_t real_steps_cnt):
        grid_size{ 
            real_grid_size.x >> TCell::NRankX, 
            real_grid_size.y >> TCell::NRankY, 
            real_grid_size.z >> TCell::NRankZ
        },
        steps_cnt{real_steps_cnt},
        factor(grid_size)
    {}

public:
    int3_t grid_size{};
    int_t steps_cnt{};

    real_t dspace{0.0};
    real_t dtime{0.0};

    VolumeArray<TCell, NGhost> factor; // (bulk / rho) * pow(dtime / dspace, 2)
};

} // namespace geo

#endif // GEOACOUSTIC_STATIC_CONFIG_HPP_
