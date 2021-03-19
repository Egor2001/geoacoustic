#ifndef GEOACOUSTIC_STATIC_GRID_PROC_HPP_
#define GEOACOUSTIC_STATIC_GRID_PROC_HPP_

#include <algorithm>
#include <cassert>
#include <cstdio>

#include "types.hpp"
#include "config.hpp"
#include "context.hpp"
#include "tiling_proc.hpp"

namespace geo {

template<typename TCell, int_t NTileRank>
void grid_proc(const Config<TCell>& cfg, Context<TCell>& ctx)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    // config needs to be adjusted before processing
    assert(cfg.grid_size.x % NTileSize == 0 && 
           cfg.grid_size.y % NTileSize == 0 &&
           cfg.grid_size.z % NTileSize == 0);
    assert(cfg.steps_cnt % NTileSize == 0);

    // prepare data views for tiling routine
    VolumeSpan<TCell> ampl = ctx.ampl.span();
    VolumeSpan<TCell> ampl_next = ctx.ampl_next.span();

    for (int_t step = 0; step < cfg.steps_cnt; step += NTileSize)
    {
        // process grid depending on tiling
        tiling_proc<TCell, NTileRank>(cfg, ampl_next, ampl);
        fprintf(stderr, "NEXT STEP!\n");

        if constexpr (NTileRank == 0)
        {
            std::swap(ampl_next, ampl);
        }
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_GRID_PROC_HPP_
