#ifndef GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_
#define GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_

#include "types.hpp"

namespace geo {

struct alignas(8 * sizeof(real_t)) ZCube2Cell
{
    static constexpr int_t NRankX = 1, NRankY = 1, NRankZ = 1;

    real_t arr[2 * 2 * 2];
};

inline __attribute__((always_inline)) 
void zcube2_cell_proc(int3_t idx3, const Config<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    (AT_(ampl_next, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] = \
     (AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
      AT_(ampl_next, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
      AT_(ampl, 0, 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)] + \
      AT_(ampl, 0, 0, 0)[(X) + 2*(1 - (Y)) + 4*(Z)] + \
      AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(1 - (Z))] + \
      AT_(ampl, 2*(X) - 1, (Y), (Z))[(1 - (X)) + 2*(Y) + 4*(Z)] + \
      AT_(ampl, (X), 2*(Y) - 1, (Z))[(X) + 2*(1 - (Y)) + 4*(Z)] + \
      AT_(ampl, (X), (Y), 2*(Z) - 1)[(X) + 2*(Y) + 4*(1 - (Z))]) / 8.0)

    PROC_STENCIL_(1, 1, 1);
    PROC_STENCIL_(0, 1, 1);
    PROC_STENCIL_(1, 0, 1);
    PROC_STENCIL_(1, 1, 0);
    PROC_STENCIL_(1, 0, 0);
    PROC_STENCIL_(0, 1, 0);
    PROC_STENCIL_(0, 0, 1);
    PROC_STENCIL_(0, 0, 0);

#undef PROC_STENCIL_

#undef AT_
}

void zcube2_cell_load(int3_t dim3, VolumeSpan<ZCube2Cell> span, 
                      std::istream& stream)
{
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x / 2, y / 2, z / 2})->
            arr[x % 2 + 2 * (y % 2) + 4 * (z % 2)];
    }
}

void zcube2_cell_store(int3_t dim3, VolumeConstSpan<ZCube2Cell> span, 
                       std::ostream& stream)
{
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        stream << span.at(dim3, int3_t{x / 2, y / 2, z / 2})->
            arr[x % 2 + 2 * (y % 2) + 4 * (z % 2)] << ' ';
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_
