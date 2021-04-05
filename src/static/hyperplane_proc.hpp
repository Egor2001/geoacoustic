#ifndef GEOACOUSTIC_STATIC_HYPERPLANE_PROC_HPP_
#define GEOACOUSTIC_STATIC_HYPERPLANE_PROC_HPP_

#include <algorithm>
#include <cstdio>

#include <pthread.h>
#include <omp.h>

#include "tiling_proc.hpp"

namespace geo {

template<typename TCell, int_t NTileRank>
void hyperplane_tiling_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl, 
        int_t layers_cnt)
{
    static constexpr int_t NTileSize = 1 << NTileRank;
    static constexpr int_t NDiagInterval = 4;

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
                #pragma omp barrier
            }
        }

        for (int_t diag = diag_min; diag >= 0; diag -= NTileSize)
        {
            #pragma omp barrier
        }
    }
}

template<typename TCell, int_t NTileRank>
void diagonal_tiling_proc(const Config<TCell>& cfg, 
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
{
    static constexpr int_t NTileSize = 1 << NTileRank;

    int3_t dim3 = cfg.grid_size;
    int_t diag_max = dim3.x + dim3.y + dim3.z - 3 * NTileSize;

    // Starting thread team
    #pragma omp parallel
    {
        int trd_cnt = omp_get_num_threads();
        int trd_idx = omp_get_thread_num();
        int cur_cell_idx = 0;
        for (int_t diag = diag_max; diag >= 0; diag -= NTileSize)
        {
            int_t min_z = std::max(static_cast<int_t>(0), 
                                   diag - (dim3.x + dim3.y - 2 * NTileSize));
            int_t max_z = std::min(dim3.z - NTileSize, diag);
            for (int_t iz = min_z; iz <= max_z; iz += NTileSize)
            {
                int_t min_y = std::max(static_cast<int_t>(0), 
                                       diag - (iz + dim3.x - NTileSize));
                int_t max_y = std::min(dim3.y - NTileSize, diag - iz);
                for (int_t iy = min_y; iy <= max_y; iy += NTileSize)
                {
                    if (cur_cell_idx % trd_cnt == trd_idx)
                    {
                        int_t ix = diag - (iy + iz);
                        tile_proc<TCell, NTileRank>
                            (int3_t{ix, iy, iz}, cfg, ampl_next, ampl);
                    }

                    ++cur_cell_idx;
                }
            }

            // Synchronizing thread team before starting next diagonal
            #pragma omp barrier
        }
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_HYPERPLANE_PROC_HPP_
