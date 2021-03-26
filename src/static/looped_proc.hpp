#ifndef GEOACOUSTIC_STATIC_LOOPED_PROC_HPP_
#define GEOACOUSTIC_STATIC_LOOPED_PROC_HPP_

#include <algorithm>
#include <cstdio>

#include "macro.hpp"
#include "types.hpp"
#include "config.hpp"
#include "context.hpp"
#include "cell_layout.hpp"
#include "volume_array.hpp"

namespace geo {

template<typename TCell>
void looped_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    int3_t dim3 = cfg.grid_size;
    for (int_t iz = dim3.z - 1; iz >= 0; iz -= 1)
    for (int_t iy = dim3.y - 1; iy >= 0; iy -= 1)
    for (int_t ix = dim3.x - 1; ix >= 0; ix -= 1)
    {
        CellLayout<TCell>::
            cell_proc(int3_t{ix, iy, iz}, cfg, ampl_next, ampl);
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_LOOPED_PROC_HPP_
