#ifndef GEOACOUSTIC_STATIC_TILING_PROC_HPP_
#define GEOACOUSTIC_STATIC_TILING_PROC_HPP_

#include <algorithm>
#include <cstdio>

#include "macro.hpp"
#include "types.hpp"
#include "config.hpp"
#include "context.hpp"
#include "cell_layout.hpp"
#include "volume_array.hpp"

namespace geo {

//  II ... A|A ... BB ... C|C ... II
// II ... AA| ... BB ... CC| ... II
//
//       AB       BB       CI
// A -> IA  B -> BB  C -> BC
enum TileSideType
{
    TILE_A = 0/* = 0 */, TILE_B/* = 1*/, TILE_C/* = 2*/, TILE_I/* = 3*/,
};

static constexpr TileSideType ATileSideSubtypeMtx[4][3] = {
    /* [TILE_A = 0][_] = */ { 
        /* [SHIFT = 0] = */ TILE_I,
        /* [SHIFT = 1] = */ TILE_A,
        /* [SHIFT = 2] = */ TILE_B,
    },
    /* [TILE_B = 1][_] = */ { 
        /* [SHIFT = 0] = */ TILE_B,
        /* [SHIFT = 1] = */ TILE_B,
        /* [SHIFT = 2] = */ TILE_B,
    },
    /* [TILE_C = 2][_] = */ {
        /* [SHIFT = 0] = */ TILE_B,
        /* [SHIFT = 1] = */ TILE_C,
        /* [SHIFT = 2] = */ TILE_I
    },
    /* [TILE_I = 3][_] = */ {
        /* [SHIFT = 0] = */ TILE_I,
        /* [SHIFT = 1] = */ TILE_I,
        /* [SHIFT = 2] = */ TILE_I
    }
};

template<bool NIsSwap>
struct ConditionalSwapper;

template<>
struct ConditionalSwapper<false>
{
    template<typename T>
    static void swap(T&, T&)
    {}
};

template<>
struct ConditionalSwapper<true>
{
    template<typename T>
    static void swap(T& lhs, T& rhs)
    {
        std::swap(lhs, rhs);
    }
};

template<typename TCell, bool NIsCell>
struct ConditionalCell;

template<typename TCell>
struct ConditionalCell<TCell, false>
{
    static void cell_proc(int3_t, const Config<TCell>&, 
            VolumeSpan<TCell>, VolumeSpan<TCell>)
    {}
};

template<typename TCell>
struct ConditionalCell<TCell, true>
{
    static void cell_proc(int3_t idx3, const Config<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        GEO_ON_DEBUG(fprintf(stderr, "CELL PROC!\n"));
        CellLayout<TCell>::cell_proc(idx3, cfg, ampl_next, ampl);
    }
};

template<typename TCell, int_t NTileRank, 
         int_t NTypeX, int_t NTypeY, int_t NTypeZ>
struct RecursiveTile;

template<typename TCell, int_t NTypeX, int_t NTypeY, int_t NTypeZ>
struct RecursiveTile<TCell, 0, NTypeX, NTypeY, NTypeZ>
{
    static void tile_proc(int3_t idx3, const Config<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        // return if out of range
        static constexpr bool NIsCell = 
            ((NTypeX == TILE_I || NTypeX == TILE_A || 
              NTypeY == TILE_I || NTypeY == TILE_A || 
              NTypeZ == TILE_I || NTypeZ == TILE_A) == false);

        ConditionalCell<TCell, NIsCell>::cell_proc(idx3, cfg, ampl_next, ampl);
    }
};

template<typename TCell, int_t NTileRank, 
         int_t NTypeX, int_t NTypeY, int_t NTypeZ>
struct RecursiveTile
{
    static_assert(NTileRank >= 0, "tile rank must be non-negative");
    static_assert(NTileRank != 0, "tile rank is 0 in general template");
    static constexpr int_t NTileSize = 1 << NTileRank;

    static void tile_proc(int3_t idx3, const Config<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
#define CALL_TILE_PROC(X, Y, Z) \
    RecursiveTile<TCell, NTileRank - 1, \
        ATileSideSubtypeMtx[NTypeX][X], \
        ATileSideSubtypeMtx[NTypeY][Y], \
        ATileSideSubtypeMtx[NTypeZ][Z]>::tile_proc \
        (idx3 + int3_t{(X) * (NTileSize / 2), \
                       (Y) * (NTileSize / 2), \
                       (Z) * (NTileSize / 2)}, cfg, ampl_next, ampl)

        // time level #0
        CALL_TILE_PROC(1, 1, 1);
        CALL_TILE_PROC(0, 1, 1);
        CALL_TILE_PROC(1, 0, 1);
        CALL_TILE_PROC(1, 1, 0);
        CALL_TILE_PROC(1, 0, 0);
        CALL_TILE_PROC(0, 1, 0);
        CALL_TILE_PROC(0, 0, 1);
        CALL_TILE_PROC(0, 0, 0);

        ConditionalSwapper<NTileRank == 1>::swap(ampl, ampl_next);

        // time level #1
        CALL_TILE_PROC(2, 2, 2);
        CALL_TILE_PROC(1, 2, 2);
        CALL_TILE_PROC(2, 1, 2);
        CALL_TILE_PROC(2, 2, 1);
        CALL_TILE_PROC(2, 1, 1);
        CALL_TILE_PROC(1, 2, 1);
        CALL_TILE_PROC(1, 1, 2);
        CALL_TILE_PROC(1, 1, 1);

#undef CALL_TILE_PROC
    }
};

template<typename TCell, int_t NTileRank>
void tiling_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    int3_t dim3 = cfg.grid_size;
    for (int_t iz = dim3.z - NTileSize; iz >= -NTileSize; iz -= NTileSize)
    for (int_t iy = dim3.y - NTileSize; iy >= -NTileSize; iy -= NTileSize)
    for (int_t ix = dim3.x - NTileSize; ix >= -NTileSize; ix -= NTileSize)
    {
#define MASK_3(X, Y, Z) ((X) + (Y)*3 + (Z)*9)

        int8_t type_mask = 0;
        if (ix == -NTileSize) type_mask += 0 * 1;
        else if (ix == (cfg.grid_size.x - NTileSize)) type_mask += 2 * 1;
        else type_mask += 1 * 1;

        if (iy == -NTileSize) type_mask += 0 * 3;
        else if (iy == (cfg.grid_size.y - NTileSize)) type_mask += 2 * 3;
        else type_mask += 1 * 3;

        if (iz == -NTileSize) type_mask += 0 * 9;
        else if (iz == (cfg.grid_size.z - NTileSize)) type_mask += 2 * 9;
        else type_mask += 1 * 9;

#define CALL_TILE_PROC(X, Y, Z, TX, TY, TZ) \
    RecursiveTile<TCell, NTileRank, (TX), (TY), (TZ)>::tile_proc \
    (int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl)

        // TODO: to divide loop into corners, edges, faces and inner part
        switch (type_mask)
        {
#define TA TILE_A
#define TB TILE_B
#define TC TILE_C
            case MASK_3(0,0,0): CALL_TILE_PROC(ix, iy, iz, TA, TA, TA); break;
            case MASK_3(0,0,1): CALL_TILE_PROC(ix, iy, iz, TA, TA, TB); break;
            case MASK_3(0,0,2): CALL_TILE_PROC(ix, iy, iz, TA, TA, TC); break;
            case MASK_3(0,1,0): CALL_TILE_PROC(ix, iy, iz, TA, TB, TA); break;
            case MASK_3(0,1,1): CALL_TILE_PROC(ix, iy, iz, TA, TB, TB); break;
            case MASK_3(0,1,2): CALL_TILE_PROC(ix, iy, iz, TA, TB, TC); break;
            case MASK_3(0,2,0): CALL_TILE_PROC(ix, iy, iz, TA, TC, TA); break;
            case MASK_3(0,2,1): CALL_TILE_PROC(ix, iy, iz, TA, TC, TB); break;
            case MASK_3(0,2,2): CALL_TILE_PROC(ix, iy, iz, TA, TC, TC); break;

            case MASK_3(1,0,0): CALL_TILE_PROC(ix, iy, iz, TB, TA, TA); break;
            case MASK_3(1,0,1): CALL_TILE_PROC(ix, iy, iz, TB, TA, TB); break;
            case MASK_3(1,0,2): CALL_TILE_PROC(ix, iy, iz, TB, TA, TC); break;
            case MASK_3(1,1,0): CALL_TILE_PROC(ix, iy, iz, TB, TB, TA); break;
            case MASK_3(1,1,1): CALL_TILE_PROC(ix, iy, iz, TB, TB, TB); break;
            case MASK_3(1,1,2): CALL_TILE_PROC(ix, iy, iz, TB, TB, TC); break;
            case MASK_3(1,2,0): CALL_TILE_PROC(ix, iy, iz, TB, TC, TA); break;
            case MASK_3(1,2,1): CALL_TILE_PROC(ix, iy, iz, TB, TC, TB); break;
            case MASK_3(1,2,2): CALL_TILE_PROC(ix, iy, iz, TB, TC, TC); break;

            case MASK_3(2,0,0): CALL_TILE_PROC(ix, iy, iz, TC, TA, TA); break;
            case MASK_3(2,0,1): CALL_TILE_PROC(ix, iy, iz, TC, TA, TB); break;
            case MASK_3(2,0,2): CALL_TILE_PROC(ix, iy, iz, TC, TA, TC); break;
            case MASK_3(2,1,0): CALL_TILE_PROC(ix, iy, iz, TC, TB, TA); break;
            case MASK_3(2,1,1): CALL_TILE_PROC(ix, iy, iz, TC, TB, TB); break;
            case MASK_3(2,1,2): CALL_TILE_PROC(ix, iy, iz, TC, TB, TC); break;
            case MASK_3(2,2,0): CALL_TILE_PROC(ix, iy, iz, TC, TC, TA); break;
            case MASK_3(2,2,1): CALL_TILE_PROC(ix, iy, iz, TC, TC, TB); break;
            case MASK_3(2,2,2): CALL_TILE_PROC(ix, iy, iz, TC, TC, TC); break;

            default: assert(!"impossible");
#undef TA
#undef TB
#undef TC
        }

#undef CALL_TILE_PROC

#undef MASK_3
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_TILING_PROC_HPP_
