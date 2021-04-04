#ifndef GEOACOUSTIC_STATIC_LOOPED_PROC_HPP_
#define GEOACOUSTIC_STATIC_LOOPED_PROC_HPP_

#include <algorithm>
#include <cstdio>

#include <pthread.h>
#include <omp.h>

#include "macro.hpp"
#include "types.hpp"
#include "config.hpp"
#include "context.hpp"
#include "cell_layout.hpp"
#include "volume_array.hpp"

namespace geo {

template<typename TCell>
void rectangular_looped_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    int3_t dim3 = cfg.grid_size;
    #pragma omp parallel for collapse(2)
    for (int_t iz = dim3.z - 1; iz >= 0; iz -= 1)
    for (int_t iy = dim3.y - 1; iy >= 0; iy -= 1)
    for (int_t ix = dim3.x - 1; ix >= 0; ix -= 1)
    {
        CellLayout<TCell>::
            cell_proc(int3_t{ix, iy, iz}, cfg, ampl_next, ampl);
    }
}

template<typename TCell>
void diagonal_looped_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    int3_t dim3 = cfg.grid_size;
    int_t diag_max = dim3.x + dim3.y + dim3.z - 3;
    for (int_t diag = diag_max; diag >= 0; --diag)
    {
        int_t min_z = std::max(static_cast<int_t>(0), 
                               diag - (dim3.x + dim3.y - 2));
        int_t max_z = std::min(dim3.z - 1, diag);

        #pragma omp parallel for
        for (int_t iz = min_z; iz <= max_z; ++iz)
        {
            int_t min_y = std::max(static_cast<int_t>(0), 
                                   diag - (iz + dim3.x - 1));
            int_t max_y = std::min(dim3.y - 1, diag - iz);
            for (int_t iy = min_y; iy <= max_y; ++iy)
            {
                int_t ix = diag - (iy + iz);
                CellLayout<TCell>::
                    cell_proc(int3_t{ix, iy, iz}, cfg, ampl_next, ampl);
            }
        }
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_LOOPED_PROC_HPP_
