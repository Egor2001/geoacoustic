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

    const real_t fdc_1 = -5.0 / 2.0, fdc_2 = 4.0 / 3.0;

    const real_t bulk = AT_(cfg.bulk.span(), 0, 0, 0);
    const real_t factor = 
        bulk * (cfg.dtime * cfg.dtime) / (cfg.dspace * cfg.dspace);

    real_t u_dx = 
        fdc_1 * AT_(ampl_next, 0, 0, 0) + 
        fdc_2 * (AT_(ampl, +1, 0, 0) - AT_(ampl, -1, 0, 0));
    real_t u_dy = 
        fdc_1 * AT_(ampl_next, 0, 0, 0) + 
        fdc_2 * (AT_(ampl, 0, +1, 0) - AT_(ampl, 0, -1, 0));
    real_t u_dz = 
        fdc_1 * AT_(ampl_next, 0, 0, 0) + 
        fdc_2 * (AT_(ampl, 0, 0, +1) - AT_(ampl, 0, 0, -1));

    AT_(ampl_next, 0, 0, 0) = 
        2.0 * AT_(ampl, 0, 0, 0) - AT_(ampl_next, 0, 0, 0) + 
        factor * (u_dx + u_dy + u_dz);

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

inline __attribute__((always_inline)) 
void linear_cell_test_proc(int3_t idx3, const Config<LinearCell>& cfg,
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

} // namespace geo

#endif // GEOACOUSTIC_STATIC_LINEAR_CELL_HPP_
