#ifndef GEOACOUSTIC_STATIC_LINEAR_CELL_HPP_
#define GEOACOUSTIC_STATIC_LINEAR_CELL_HPP_

#include <iostream>
#include <cstdio>

#include "types.hpp"
#include "context.hpp"
#include "volume_array.hpp"

namespace geo {

struct LinearCell
{
    static constexpr int_t NRankX = 0, NRankY = 0, NRankZ = 0;

    real_t data;
};

inline __attribute__((always_inline)) 
void linear_cell_proc(int3_t idx3, const Config<LinearCell>& cfg,
        VolumeSpan<LinearCell> ampl_next, VolumeSpan<LinearCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->data)

    AT_(ampl_next, 0, 0, 0) = 
        (AT_(ampl, 0, 0, 0) + 
         AT_(ampl_next, 0, 0, 0) + 
         AT_(ampl, +1, 0, 0) + 
         AT_(ampl, 0, +1, 0) + 
         AT_(ampl, 0, 0, +1) + 
         AT_(ampl, -1, 0, 0) + 
         AT_(ampl, 0, -1, 0) + 
         AT_(ampl, 0, 0, -1)) / 8.0;

#undef AT_
}

void linear_cell_load(int3_t dim3, VolumeSpan<LinearCell> span, 
                      std::istream& stream)
{
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x, y, z})->data;
    }
}

void linear_cell_store(int3_t dim3, VolumeConstSpan<LinearCell> span, 
                       std::ostream& stream)
{
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        stream << span.at(dim3, int3_t{x, y, z})->data << ' ';
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_LINEAR_CELL_HPP_
