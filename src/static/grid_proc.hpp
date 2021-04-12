#ifndef GEOACOUSTIC_STATIC_GRID_PROC_HPP_
#define GEOACOUSTIC_STATIC_GRID_PROC_HPP_

#include <algorithm>
#include <cassert>
#include <cstdio>

#include "macro.hpp"
#include "types.hpp"
#include "config.hpp"
#include "context.hpp"
#include "tiling_proc.hpp"
#include "looped_proc.hpp"

namespace geo {

template<typename TCell, int_t NTileRank>
struct VolumeProcessor;

template<typename TCell>
struct VolumeProcessor<TCell, 0>
{
    static void volume_proc(const Config<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next,
            VolumeSpan<TCell> ampl)
    {
        for (int_t step = 0; step < cfg.steps_cnt; ++step)
        {
#if defined(GEO_GRID_LOOPED_DIAGONAL)
            diagonal_looped_proc<TCell>(cfg, ampl_next, ampl);
#else
            rectangular_looped_proc<TCell>(cfg, ampl_next, ampl);
#endif
            GEO_ON_DEBUG(fprintf(stderr, "NEXT STEP!\n"));

            // swap volumes
            std::swap(ampl_next, ampl);
        }
    }
};

template<typename TCell, int_t NTileRank>
struct VolumeProcessor
{
    static void volume_proc(const Config<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next,
            VolumeSpan<TCell> ampl)
    {
        static constexpr int_t NTileSize = 1 << NTileRank;

#if defined(GEO_GRID_SINGLE_LAYER)
        for (int_t step = 0; step < cfg.steps_cnt; step += NTileSize)
        {
#if defined(GEO_GRID_TILING_RECTANGULAR)
            rectangular_tiling_proc<TCell, NTileRank>(cfg, ampl_next, ampl);
#else
            diagonal_tiling_proc<TCell, NTileRank>(cfg, ampl_next, ampl);
#endif
            GEO_ON_DEBUG(fprintf(stderr, "NEXT STEP!\n"));
            // no need in swapping volumes because of tiling
        }
#else
        hyperplane_tiling_proc<TCell, NTileRank>
            (cfg, ampl_next, ampl, cfg.steps_cnt / NTileSize);
#endif
    }
};

template<typename TCell, int_t NTileRank>
void grid_proc(const Config<TCell>& cfg, Context<TCell>& ctx)
{
    // config needs to be adjusted before processing
    GEO_ON_DEBUG(
        assert(cfg.grid_size.x % NTileSize == 0 && 
               cfg.grid_size.y % NTileSize == 0 &&
               cfg.grid_size.z % NTileSize == 0)
        );
    GEO_ON_DEBUG(assert(cfg.steps_cnt % NTileSize == 0));

    // prepare data views for tiling routine
    VolumeSpan<TCell> ampl = ctx.ampl.span();
    VolumeSpan<TCell> ampl_next = ctx.ampl_next.span();

    VolumeProcessor<TCell, NTileRank>::volume_proc(cfg, ampl_next, ampl);
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_GRID_PROC_HPP_
