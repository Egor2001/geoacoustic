#ifndef GEOACOUSTIC_STATIC_TILING_PROC_HPP_
#define GEOACOUSTIC_STATIC_TILING_PROC_HPP_

#include <type_traits>
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

// General version (TEnable is for specializations)
template<typename TCell, int_t NTileRank, 
         int_t NTypeX, int_t NTypeY, int_t NTypeZ, typename TEnable = void>
struct RecursiveTile
{
    static void tile_proc(int3_t, const Config<TCell>&, 
                          VolumeSpan<TCell>, VolumeSpan<TCell>)
    {}
};

#if defined(GEO_UNROLL_TILE3)
    #include "gen/spec_tile_rank1.hpp"
    #include "gen/spec_tile_rank2.hpp"
    #include "gen/spec_tile_rank3.hpp"
#elif defined(GEO_UNROLL_TILE2)
    #include "gen/spec_tile_rank1.hpp"
    #include "gen/spec_tile_rank2.hpp"
#elif defined(GEO_UNROLL_TILE1)
    #include "gen/spec_tile_rank1.hpp"
#endif

// Specialization for Rank = 0 && Tile is inside Volume
template<typename TCell, int_t NTypeX, int_t NTypeY, int_t NTypeZ>
struct RecursiveTile<TCell, 0, NTypeX, NTypeY, NTypeZ, 
    std::enable_if_t<NTypeX != TILE_I && NTypeY != TILE_I && NTypeZ != TILE_I &&
                     NTypeX != TILE_A && NTypeY != TILE_A && NTypeZ != TILE_A>>
{
    static void tile_proc(int3_t idx3, const Config<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        GEO_ON_DEBUG(fprintf(stderr, "CELL PROC[%d %d %d][%p]!\n", 
                     static_cast<int>(idx3.x), 
                     static_cast<int>(idx3.y),
                     static_cast<int>(idx3.z),
                     static_cast<void*>(ampl_next.at(int3_t{-1, -1, -1}, 
                             cfg.grid_size))));
        CellLayout<TCell>::cell_proc(idx3, cfg, ampl_next, ampl);
    }
};

// Specialization for Rank >= 1 && Tile is not outside Volume
template<typename TCell, int_t NTileRank, 
         int_t NTypeX, int_t NTypeY, int_t NTypeZ>
struct RecursiveTile<TCell, NTileRank, NTypeX, NTypeY, NTypeZ, 
    std::enable_if_t<NTileRank >= 1 &&
        NTypeX != TILE_I && NTypeY != TILE_I && NTypeZ != TILE_I>>
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
inline __attribute__((always_inline)) 
void tile_proc(int3_t idx3, const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    GEO_ON_DEBUG(fprintf(stderr, "TILE PROC [%d %d %d]!\n", 
                 static_cast<int>(idx3.x), 
                 static_cast<int>(idx3.y), 
                 static_cast<int>(idx3.z)));

#define MASK_3(X, Y, Z) ((X) + (Y)*3 + (Z)*9)
    int8_t type_mask = 0;
    if (idx3.x == -NTileSize) type_mask += 0 * 1;
    else if (idx3.x == (cfg.grid_size.x - NTileSize)) type_mask += 2 * 1;
    else type_mask += 1 * 1;

    if (idx3.y == -NTileSize) type_mask += 0 * 3;
    else if (idx3.y == (cfg.grid_size.y - NTileSize)) type_mask += 2 * 3;
    else type_mask += 1 * 3;

    if (idx3.z == -NTileSize) type_mask += 0 * 9;
    else if (idx3.z == (cfg.grid_size.z - NTileSize)) type_mask += 2 * 9;
    else type_mask += 1 * 9;

#define CALL_TILE_PROC(TX, TY, TZ) \
    RecursiveTile<TCell, NTileRank, (TX), (TY), (TZ)>::tile_proc \
    (idx3, cfg, ampl_next, ampl)

    // TODO: to divide loop into corners, edges, faces and inner part
    switch (type_mask)
    {
#define TA TILE_A
#define TB TILE_B
#define TC TILE_C
        case MASK_3(0,0,0): CALL_TILE_PROC(TA, TA, TA); break;
        case MASK_3(0,0,1): CALL_TILE_PROC(TA, TA, TB); break;
        case MASK_3(0,0,2): CALL_TILE_PROC(TA, TA, TC); break;
        case MASK_3(0,1,0): CALL_TILE_PROC(TA, TB, TA); break;
        case MASK_3(0,1,1): CALL_TILE_PROC(TA, TB, TB); break;
        case MASK_3(0,1,2): CALL_TILE_PROC(TA, TB, TC); break;
        case MASK_3(0,2,0): CALL_TILE_PROC(TA, TC, TA); break;
        case MASK_3(0,2,1): CALL_TILE_PROC(TA, TC, TB); break;
        case MASK_3(0,2,2): CALL_TILE_PROC(TA, TC, TC); break;

        case MASK_3(1,0,0): CALL_TILE_PROC(TB, TA, TA); break;
        case MASK_3(1,0,1): CALL_TILE_PROC(TB, TA, TB); break;
        case MASK_3(1,0,2): CALL_TILE_PROC(TB, TA, TC); break;
        case MASK_3(1,1,0): CALL_TILE_PROC(TB, TB, TA); break;
        case MASK_3(1,1,1): CALL_TILE_PROC(TB, TB, TB); break;
        case MASK_3(1,1,2): CALL_TILE_PROC(TB, TB, TC); break;
        case MASK_3(1,2,0): CALL_TILE_PROC(TB, TC, TA); break;
        case MASK_3(1,2,1): CALL_TILE_PROC(TB, TC, TB); break;
        case MASK_3(1,2,2): CALL_TILE_PROC(TB, TC, TC); break;

        case MASK_3(2,0,0): CALL_TILE_PROC(TC, TA, TA); break;
        case MASK_3(2,0,1): CALL_TILE_PROC(TC, TA, TB); break;
        case MASK_3(2,0,2): CALL_TILE_PROC(TC, TA, TC); break;
        case MASK_3(2,1,0): CALL_TILE_PROC(TC, TB, TA); break;
        case MASK_3(2,1,1): CALL_TILE_PROC(TC, TB, TB); break;
        case MASK_3(2,1,2): CALL_TILE_PROC(TC, TB, TC); break;
        case MASK_3(2,2,0): CALL_TILE_PROC(TC, TC, TA); break;
        case MASK_3(2,2,1): CALL_TILE_PROC(TC, TC, TB); break;
        case MASK_3(2,2,2): CALL_TILE_PROC(TC, TC, TC); break;

        default: assert(!"impossible");
#undef TA
#undef TB
#undef TC
    }

#undef CALL_TILE_PROC

#undef MASK_3
}

template<typename TCell, int_t NTileRank>
inline __attribute__((always_inline)) 
void diag_proc(int trd_idx, int trd_cnt, 
        int_t cur_diag, int& cell_counter, const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    GEO_ON_DEBUG(fprintf(stderr, "DIAG PROC [%d]!\n", 
                 static_cast<int>(cur_diag)));

    int3_t dim3 = cfg.grid_size;
    int_t min_z = std::max(static_cast<int_t>(-NTileSize),
            cur_diag - (dim3.x + dim3.y - 2 * NTileSize));
    int_t max_z = std::min(dim3.z - NTileSize, cur_diag + 2 * NTileSize);
    for (int_t iz = min_z; iz <= max_z; iz += NTileSize)
    {
        int_t min_y = std::max(static_cast<int_t>(-NTileSize),
                cur_diag - (iz + dim3.x - NTileSize));
        int_t max_y = std::min(dim3.y - NTileSize, cur_diag + NTileSize - iz);
        for (int_t iy = min_y; iy <= max_y; iy += NTileSize)
        {
            if (cell_counter % trd_cnt == trd_idx)
            {
                int_t ix = cur_diag - (iy + iz);
                tile_proc<TCell, NTileRank>
                    (int3_t{ix, iy, iz}, cfg, ampl_next, ampl);
            }

            ++cell_counter;
        }
    }
}

template<typename TCell, int_t NTileRank>
void rectangular_tiling_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    int3_t dim3 = cfg.grid_size;
    for (int_t iz = dim3.z - NTileSize; iz >= -NTileSize; iz -= NTileSize)
    for (int_t iy = dim3.y - NTileSize; iy >= -NTileSize; iy -= NTileSize)
    for (int_t ix = dim3.x - NTileSize; ix >= -NTileSize; ix -= NTileSize)
    {
        tile_proc<TCell, NTileRank>(int3_t{ix, iy, iz}, cfg, ampl_next, ampl);
    }
}

template<typename TCell, int_t NTileRank>
void diagonal_tiling_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    int3_t dim3 = cfg.grid_size;
    int_t diag_max = dim3.x + dim3.y + dim3.z - 3 * NTileSize;
    int_t diag_min = -3 * NTileSize;

    // Starting thread team
    #pragma omp parallel
    {
        int trd_cnt = omp_get_num_threads();
        int trd_idx = omp_get_thread_num();
        int cur_cell_idx = 0;
        for (int_t cur_diag = diag_max; cur_diag >= diag_min; 
             cur_diag -= NTileSize)
        {
            diag_proc<TCell, NTileRank>
                (trd_idx, trd_cnt, cur_diag, cur_cell_idx, 
                 cfg, ampl_next, ampl);

            // Synchronizing thread team before starting next diagonal
            #pragma omp barrier
        }
    }
}

template<typename TCell, int_t NTileRank>
void hyperplane_tiling_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl, 
        int_t layers_cnt)
{
    static constexpr int_t NTileSize = 1 << NTileRank;
    static constexpr int_t NDiagInterval = 5;

    int3_t dim3 = cfg.grid_size;
    int_t diag_max = dim3.x + dim3.y + dim3.z - 3 * NTileSize;
    int_t diag_min = diag_max - NDiagInterval * NTileSize;

    // Starting thread team
    #pragma omp parallel
    {
        int trd_cnt = omp_get_num_threads();
        int trd_idx = omp_get_thread_num();
        int cur_cell_idx = 0;
        for (int_t layer = 0; layer < layers_cnt; ++layer)
        {
            for (int_t diag = diag_max; diag > diag_min; diag -= NTileSize)
            {
                int_t cur_min_diag = std::max(
                        static_cast<int_t>(-3 * NTileSize), 
                        diag - layer * NDiagInterval * NTileSize);
                for (int_t cur_diag = diag; cur_diag >= cur_min_diag; 
                     cur_diag -= NDiagInterval * NTileSize)
                {
                    diag_proc<TCell, NTileRank>
                        (trd_idx, trd_cnt, cur_diag, cur_cell_idx, 
                         cfg, ampl_next, ampl);
                }

                #pragma omp barrier
            }
        }

        for (int_t diag = diag_min; diag >= -3 * NTileSize; diag -= NTileSize)
        {
            int_t cur_min_diag = std::max(
                    static_cast<int_t>(-3 * NTileSize), 
                    diag - (layers_cnt - 1) * NDiagInterval * NTileSize);
            for (int_t cur_diag = diag; cur_diag >= cur_min_diag; 
                 cur_diag -= NDiagInterval * NTileSize)
            {
                diag_proc<TCell, NTileRank>
                    (trd_idx, trd_cnt, cur_diag, cur_cell_idx, 
                     cfg, ampl_next, ampl);
            }

            #pragma omp barrier
        }
    }

#undef PROC_DIAG_
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_TILING_PROC_HPP_
