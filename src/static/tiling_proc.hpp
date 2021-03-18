#ifndef GEOACOUSTIC_STATIC_TILING_PROC_HPP_
#define GEOACOUSTIC_STATIC_TILING_PROC_HPP_

#include "types.hpp"
#include "context.hpp"
#include "volume_array.hpp"

namespace geo {

enum TileSideType
{
    TILE_INSIDE = 0, TILE_BORDER = 1, TILE_OUTSIDE = 2,
};

static constexpr TileSideType ATileSideSubtypeMtx[3][3] = {
    /* [TILE_INSIDE = 0][_] = */ { 
        /* [SHIFT = 0] = */ TILE_INSIDE,
        /* [SHIFT = 1] = */ TILE_INSIDE,
        /* [SHIFT = 2] = */ TILE_INSIDE,
    },
    /* [TILE_BORDER = 1][_] = */ {
        /* [SHIFT = 0] = */ TILE_INSIDE,
        /* [SHIFT = 1] = */ TILE_BORDER,
        /* [SHIFT = 2] = */ TILE_OUTSIDE
    },
    /* [TILE_OUTSIDE = 2][_] = */ {
        /* [SHIFT = 0] = */ TILE_OUTSIDE,
        /* [SHIFT = 1] = */ TILE_OUTSIDE,
        /* [SHIFT = 2] = */ TILE_OUTSIDE
    }
};

template<int_t NTileRank>
void tile_proc(int3_t idx3, const Config& cfg,
        VolumeSpan<real_t> ampl_next, VolumeSpan<real_t> ampl);

template<int_t NTileRank>
void tiling_proc(const Config& cfg, 
        VolumeSpan<real_t> ampl_next, VolumeSpan<real_t> ampl)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    for (int_t iz = cfg.grid_size.z - NTileSize; iz >= 0; iz -= NTileSize)
    for (int_t iy = cfg.grid_size.y - NTileSize; iy >= 0; iy -= NTileSize)
    for (int_t ix = cfg.grid_size.x - NTileSize; ix >= 0; ix -= NTileSize)
    {
        int8_t type_mask = 
            (ix == (cfg.grid_size.z - NTileSize) ? 1 : 0) | 
            (iy == (cfg.grid_size.y - NTileSize) ? 2 : 0) | 
            (iz == (cfg.grid_size.z - NTileSize) ? 4 : 0);

#define CALL_TILE_PROC(X, Y, Z, TX, TY, TZ) \
    tile_proc<NTileRank, (TX), (TY), (TZ)> \
    ({(X), (Y), (Z)}, cfg, ampl_next, ampl)

        // TODO: to divide loop into corners, edges, faces and inner part
        switch (type_mask)
        {
#define T_I TILE_INSIDE
#define T_B TILE_BORDER
            case 0b000: CALL_TILE_PROC(ix, iy, iz, T_I, T_I, T_I); break;
            case 0b001: CALL_TILE_PROC(ix, iy, iz, T_I, T_I, T_B); break;
            case 0b010: CALL_TILE_PROC(ix, iy, iz, T_I, T_B, T_I); break;
            case 0b011: CALL_TILE_PROC(ix, iy, iz, T_I, T_B, T_B); break;
            case 0b100: CALL_TILE_PROC(ix, iy, iz, T_B, T_I, T_I); break;
            case 0b101: CALL_TILE_PROC(ix, iy, iz, T_B, T_I, T_B); break;
            case 0b110: CALL_TILE_PROC(ix, iy, iz, T_B, T_B, T_I); break;
            case 0b111: CALL_TILE_PROC(ix, iy, iz, T_B, T_B, T_B); break;
            default: assert(!"impossible");
#undef T_I
#undef T_B
        }

#undef CALL_TILE_PROC
    }
}

template<int_t NTileRank, int_t NTypeX, int_t NTypeY, int_t NTypeZ>
void tile_proc(int3_t idx3, const Config& cfg,
        VolumeSpan<real_t> ampl_next, VolumeSpan<real_t> ampl)
{
    static_assert(NTileRank >= 0, "tile rank must be non-negative");
    static constexpr int_t NTileSize = 1 << NTileRank;

    // return if out of range
    if constexpr (NTypeX == -1 || NTypeY == -1 || NTypeZ == -1)
    {
        return;
    }

    if constexpr (NTileRank == 0)
    {
        cell_proc(idx3, cfg, ampl_next, ampl, x1, y1, z1);
    }
    else
    {
#define CALL_TILE_PROC(X, Y, Z) \
    tile_proc<NTileRank - 1, \
        ATileSideSubtypeMtx[NTypeX][X], \
        ATileSideSubtypeMtx[NTypeY][Y], \
        ATileSideSubtypeMtx[NTypeZ][Z]> \
        (idx3 + {(X) * (NTileSize / 2), \
                 (Y) * (NTileSize / 2), \
                 (Z) * (NTileSize / 2)}, ampl_next, ampl)

        // time level #0
        CALL_TILE_PROC(1, 1, 1);
        CALL_TILE_PROC(0, 1, 1);
        CALL_TILE_PROC(1, 0, 1);
        CALL_TILE_PROC(1, 1, 0);
        CALL_TILE_PROC(1, 0, 0);
        CALL_TILE_PROC(0, 1, 0);
        CALL_TILE_PROC(0, 0, 1);
        CALL_TILE_PROC(0, 0, 0);

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
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_TILING_PROC_HPP_
