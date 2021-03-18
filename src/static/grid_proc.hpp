#ifndef GEOACOUSTIC_STATIC_GRID_PROC_HPP_
#define GEOACOUSTIC_STATIC_GRID_PROC_HPP_

#include "types.hpp"
#include "context.hpp"

#include <cassert>

namespace geo {

template<int_t NTileRank>
void grid_proc(const Config& cfg)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    // config needs to be adjusted before processing
    assert(cfg.grid_size.x % NTileSize == 0 && 
           cfg.grid_size.y % NTileSize == 0 &&
           cfg.grid_size.z % NTileSize == 0);
    assert(cfg.steps_cnt % NTileSize == 0);

    // init context owning data for tiling routine
    Context ctx(cfg);

    // prepare data views for tiling routine
    VolumeSpan<real_t> ampl = ctx.ampl.span();
    VolumeSpan<real_t> next_ampl = ctx.next_ampl.span();
    VolumeSpan<real_t> 
        x1 = ctx.x1.span(), y1 = ctx.y1.span(), z1 = ctx.z1.span();

    for (int_t step = 0; step < cfg.steps_cnt; step += NTileSize)
    {
        // process grid depending on tiling
        tiling_proc<NTileRank>(cfg, ampl, next_ampl, x1, y1, z1);
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_GRID_PROC_HPP_
