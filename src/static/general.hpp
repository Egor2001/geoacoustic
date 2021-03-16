#ifndef GEOACOUSTIC_MACRO_GEN_GENERAL_HPP
#define GEOACOUSTIC_MACRO_GEN_GENERAL_HPP

#include "stencil.hpp"
#include "types.hpp"
#include "cell.hpp"

namespace geo {

struct Config
{
    int3_t dim3; int3_t pml3; int_t ticks_cnt;

    Cell* gh_x0_data; Cell* gh_y0_data; Cell* gh_z0_data; 
    Cell* gh_x1_data; Cell* gh_y1_data; Cell* gh_z1_data;

    int3_t* tiles; int_t tiles_cnt;
};

template<int_t NCubeRank>
void domain_proc(Stencil&, const Config&, 
        Cell* next_data, Cell* cur_data);

template<int_t NRank>
void tile_proc(Stencil&, const Config&, 
        Cell* next_data, Cell* cur_data, int3_t idx3);

template<int_t NRank>
void pml_proc(Stencil&, const Config&, 
        Cell* next, Cell* cur, int3_t idx3);

template<int_t NRank>
void cube_proc(Stencil&, Cell* next, Cell* cur, 
        Cell* x0, Cell* y0, Cell* z0, 
        Cell* x1, Cell* y1, Cell* z1);

// TODO(@geome_try): 
//  - to implement versions with less parameters count
template<int_t NRank>
void cube_proc(
        Stencil& stencil, Cell* next, Cell* cur, 
        Cell* x0, Cell* y0, Cell* z0, 
        Cell* x1, Cell* y1, Cell* z1
        )
{
    if constexpr (NRank == 0)
    {
        stencil.apply(next, cur, x0, y0, z0, x1, y1, z1);
    }
    else
    {
#if defined(__GNUG__)
        #pragma GCC unroll(8)
#elif defined(__clang__) or defined(__INTEL_COMPILER)
        #pragma unroll
#endif
        for (int_t idx = 0; idx < CORNER_CNT/* = 8 */; ++idx)
        {
#define AT(CELL, IDX) ((CELL) + (IDX) * (1 << (3 * (NRank - 1))))
            cube_proc<NRank - 1>(stencil, AT(next, idx), AT(cur, idx),
                    // this corner's x0, y0, z0
                    ((idx & 1) == 0 ? AT(x0, idx ^ 1) : AT(cur, idx ^ 1)),
                    ((idx & 2) == 0 ? AT(y0, idx ^ 2) : AT(cur, idx ^ 2)),
                    ((idx & 4) == 0 ? AT(z0, idx ^ 4) : AT(cur, idx ^ 4)),
                    // this corner's x1, y1, z1
                    ((idx & 1) == 0 ? AT(cur, idx ^ 1) : AT(x1, idx ^ 1)),
                    ((idx & 2) == 0 ? AT(cur, idx ^ 2) : AT(y1, idx ^ 2)),
                    ((idx & 4) == 0 ? AT(cur, idx ^ 4) : AT(z1, idx ^ 4))
                ); // process()
#undef AT
        }
    }
}

// TODO: to implement recursively
template<int_t NRank>
void pml_proc(
        Stencil& stencil, const Config& config, 
        Cell* next, Cell* cur, int3_t idx3
        )
{
    (void) stencil;
    (void) config;
    (void) next;
    (void) cur;
    (void) idx3;
    /*
    if (idx3.x < pml3.x)
        stencil.pml_x(next, cur, idx3, pml3.x - idx3.x);
    else if (idx3.x > dim3.x - 1 - pml3.x)
        stencil.pml_x(next, cur, idx3, idx3.x - (dim3.x - 1 - pml3.x));

    if (idx3.y < pml3.y)
        stencil.pml_y(next, cur, idx3, pml3.y - idx3.y);
    else if (idx3.y > dim3.y - 1 - pml3.y)
        stencil.pml_y(next, cur, idx3, idx3.y - (dim3.y - 1 - pml3.y));

    if (idx3.z < pml3.z)
        stencil.pml_z(next, cur, idx3, pml3.z - idx3.z);
    else if (idx3.z > dim3.z - 1 - pml3.z)
        stencil.pml_z(next, cur, idx3, idx3.z - (dim3.z - 1 - pml3.z));
    */
}

// TODO: to move params to the config
template<int_t NCubeRank>
void tile_proc(
        Stencil& stencil, const Config& config, 
        Cell* next_data, Cell* cur_data, int3_t idx3
        )
{
    static constexpr int_t NCubeVolume = (1 << (3 * NCubeRank));

    int_t offset = NCubeVolume * 
        ((idx3.x) + (idx3.y * config.dim3.x) + 
         (idx3.z * config.dim3.y * config.dim3.x));

    Cell* next = next_data + offset;
    Cell* cur = cur_data + offset;

#define AT(CELL, DX, DY, DZ) \
    ((CELL) + NCubeVolume * ((DX) + ((DY) * config.dim3.x) + \
        ((DZ) * config.dim3.y * config.dim3.x)))

    Cell* x0 = ((idx3.x > 0) ? AT(cur, -1, 0, 0) : (config.gh_x0_data + 
                NCubeVolume * (idx3.y + idx3.z * config.dim3.y)));
    Cell* y0 = ((idx3.y > 0) ? AT(cur, 0, -1, 0) : (config.gh_y0_data + 
                NCubeVolume * (idx3.x + idx3.z * config.dim3.x)));
    Cell* z0 = ((idx3.z > 0) ? AT(cur, 0, 0, -1) : (config.gh_z0_data + 
                NCubeVolume * (idx3.x + idx3.y * config.dim3.x)));

    Cell* x1 = ((idx3.x < config.dim3.x - 1) ? AT(cur, +1, 0, 0) : 
            (config.gh_x1_data + NCubeVolume * 
             (idx3.y + idx3.z * config.dim3.y)));
    Cell* y1 = ((idx3.y < config.dim3.y - 1) ? AT(cur, 0, +1, 0) : 
            (config.gh_y1_data + NCubeVolume * 
             (idx3.x + idx3.z * config.dim3.x)));
    Cell* z1 = ((idx3.z < config.dim3.z - 1) ? AT(cur, 0, 0, +1) : 
            (config.gh_z1_data + NCubeVolume * 
             (idx3.x + idx3.y * config.dim3.x)));

#undef AT

    cube_proc<NCubeRank>(
            stencil, next, cur, 
            x0, y0, z0, x1, y1, z1
        );

    pml_proc<NCubeRank>(
            stencil, config, next, cur, idx3
        );
}

template<int_t NCubeRank>
void domain_proc(
        Stencil& stencil, const Config& config, 
        Cell* next_data, Cell* cur_data
        )
{
    static constexpr int_t NCubeSide = (1 << NCubeRank);

    assert(next_data && cur_data);
    assert(config.dim3.x > 0 && config.dim3.x % NCubeSide == 0 && 
           config.dim3.y > 0 && config.dim3.y % NCubeSide == 0 && 
           config.dim3.z > 0 && config.dim3.z % NCubeSide == 0 && 
           config.ticks_cnt > 0);
    assert(config.gh_x0_data && config.gh_y0_data && config.gh_z0_data && 
           config.gh_x1_data && config.gh_y1_data && config.gh_z1_data);
    assert(config.tiles && config.tiles_cnt > 0);

    for (int_t tick = 0; tick < config.ticks_cnt; tick += 2)
    {
        for (int_t tile_idx = 0; tile_idx < config.tiles_cnt; ++tile_idx)
        {
            tile_proc<NCubeRank>(
                    stencil, config, 
                    next_data, cur_data, config.tiles[tile_idx]
                );
        }

        for (int_t tile_idx = 0; tile_idx < config.tiles_cnt; ++tile_idx)
        {
            tile_proc<NCubeRank>(
                    stencil, config, 
                    next_data, cur_data, config.tiles[tile_idx]
                );
        }
    }
}

} // namespace geo

#endif // GEOACOUSTIC_MACRO_GEN_GENERAL_HPP
