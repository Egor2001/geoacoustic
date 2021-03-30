#ifndef GEOACOUSTIC_STATIC_ZC4VEC_CELL_HPP_
#define GEOACOUSTIC_STATIC_ZC4VEC_CELL_HPP_

#include <iostream>
#include <functional>

#include "vector_cell.hpp"
#include "packed_stencil.hpp"
#include "types.hpp"

namespace geo {

#define PACK_(X, Y, Z) \
    ((((X) & 1) << 0) | (((X) & 2) << 2) | \
     (((Y) & 1) << 1) | (((Y) & 2) << 3) | \
     (((Z) & 1) << 2) | (((Z) & 2) << 4))

#define UNPACK_(IDX) \
    { ((IDX) & 0b000001 >> 0) | (((IDX) & 0b001000) >> 2), \
      ((IDX) & 0b000010 >> 1) | (((IDX) & 0b010000) >> 3), \
      ((IDX) & 0b000100 >> 2) | (((IDX) & 0b100000) >> 4) }

struct alignas(8 * sizeof(VectorCell)) ZC4VecCell
{
    static constexpr int_t 
        NRankX = 2, NRankY = 2, NRankZ = 2 + VectorCell::NRankZ;

    VectorCell arr[4 * 4 * 4]{};
};

void zc4vec_cell_proc(int3_t idx3, const Config<ZC4VecCell>& cfg,
        VolumeSpan<ZC4VecCell> ampl_next, VolumeSpan<ZC4VecCell> ampl);
void zc4vec_cell_test_proc(int3_t idx3, const Config<ZC4VecCell>& cfg,
        VolumeSpan<ZC4VecCell> ampl_next, VolumeSpan<ZC4VecCell> ampl);

void zc4vec_cell_load(int3_t dim3, VolumeSpan<ZC4VecCell> span, 
                      std::istream& stream);
void zc4vec_cell_store(int3_t dim3, VolumeConstSpan<ZC4VecCell> span, 
                       std::ostream& stream);

void zc4vec_cell_fill(int3_t dim3, VolumeSpan<ZC4VecCell> span, 
                      std::function<real_t(int3_t, int3_t)> func);
void zc4vec_cell_read(int3_t dim3, VolumeConstSpan<ZC4VecCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func);

inline __attribute__((always_inline)) 
void zc4vec_cell_proc(int3_t idx3, const Config<ZC4VecCell>& cfg,
        VolumeSpan<ZC4VecCell> ampl_next, VolumeSpan<ZC4VecCell> ampl)
{
    // zc4vec_cell_test_proc(idx3, cfg, ampl_next, ampl);
    // return;

#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(IDX, \
        XDEC, XDEC_IDX, XINC, XINC_IDX, \
        YDEC, YDEC_IDX, YINC, YINC_IDX, \
        ZDEC, ZDEC_IDX, ZINC, ZINC_IDX \
        ) \
    GEO_PACKED_STENCIL_USEDIV( \
                AT_(cfg.bulk.span(), 0, 0, 0)[IDX].vec, \
                AT_(cfg.rho.span(), 0, 0, 0)[IDX].vec, \
                cfg.dtime, cfg.dspace, \
                AT_(ampl_next, 0, 0, 0)[IDX].vec, \
                AT_(ampl, 0, 0, 0)     [IDX].vec, \
                AT_(ampl, (XDEC), 0, 0)[XDEC_IDX].vec, \
                AT_(ampl, (XINC), 0, 0)[XINC_IDX].vec, \
                AT_(ampl, 0, (YDEC), 0)[YDEC_IDX].vec, \
                AT_(ampl, 0, (YINC), 0)[YINC_IDX].vec, \
                AT_(ampl, 0, 0, (ZDEC))[ZDEC_IDX].vec, \
                AT_(ampl, 0, 0, (ZINC))[ZINC_IDX].vec \
            )

    // 64 PROC_STENCIL_() calls
    #include "gen/zcube4_proc_stencil.hpp"

#undef PROC_STENCIL_

#undef AT_
}

inline __attribute__((always_inline)) 
void zc4vec_cell_wide_proc(int3_t idx3, const Config<ZC4VecCell>& cfg,
        VolumeSpan<ZC4VecCell> ampl_next, VolumeSpan<ZC4VecCell> ampl)
{
    // zc4vec_cell_test_proc(idx3, cfg, ampl_next, ampl);
    // return;

#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(IDX, \
        XDEC, XDEC_IDX, XINC, XINC_IDX, XDEC2, XDEC2_IDX, XINC2, XINC2_IDX, \
        YDEC, YDEC_IDX, YINC, YINC_IDX, YDEC2, YDEC2_IDX, YINC2, YINC2_IDX, \
        ZDEC, ZDEC_IDX, ZINC, ZINC_IDX, ZDEC2, ZDEC2_IDX, ZINC2, ZINC2_IDX \
        ) \
    GEO_PACKED_STENCIL_WIDE_USEDIV( \
                AT_(cfg.bulk.span(), 0, 0, 0)[IDX].vec, \
                AT_(cfg.rho.span(), 0, 0, 0)[IDX].vec, \
                cfg.dtime, cfg.dspace, \
                AT_(ampl_next, 0, 0, 0) [IDX].vec, \
                AT_(ampl, 0, 0, 0)      [IDX].vec, \
                \
                AT_(ampl, (XDEC), 0, 0) [XDEC_IDX].vec, \
                AT_(ampl, (XINC), 0, 0) [XINC_IDX].vec, \
                AT_(ampl, (XDEC2), 0, 0)[XDEC2_IDX].vec, \
                AT_(ampl, (XINC2), 0, 0)[XINC2_IDX].vec, \
                \
                AT_(ampl, 0, (YDEC), 0) [YDEC_IDX].vec, \
                AT_(ampl, 0, (YINC), 0) [YINC_IDX].vec, \
                AT_(ampl, 0, (YDEC2), 0)[YDEC2_IDX].vec, \
                AT_(ampl, 0, (YINC2), 0)[YINC2_IDX].vec, \
                \
                AT_(ampl, 0, 0, (ZDEC)) [ZDEC_IDX].vec, \
                AT_(ampl, 0, 0, (ZINC)) [ZINC_IDX].vec, \
                AT_(ampl, 0, 0, (ZDEC2))[ZDEC2_IDX].vec, \
                AT_(ampl, 0, 0, (ZINC2))[ZINC2_IDX].vec \
            )

    // 64 PROC_STENCIL_() calls
    #include "gen/zcube4_proc_stencil_wide.hpp"

#undef PROC_STENCIL_

#undef AT_
}

inline __attribute__((always_inline)) 
void zc4vec_cell_test_proc(int3_t idx3, const Config<ZC4VecCell>& cfg,
        VolumeSpan<ZC4VecCell> ampl_next, VolumeSpan<ZC4VecCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(IDX, \
        XDEC, XDEC_IDX, XINC, XINC_IDX, \
        YDEC, YDEC_IDX, YINC, YINC_IDX, \
        ZDEC, ZDEC_IDX, ZINC, ZINC_IDX \
        ) \
    GEO_PACKED_STENCIL_TEST( \
                AT_(ampl_next, 0, 0, 0)[IDX].vec, \
                AT_(ampl, 0, 0, 0)     [IDX].vec, \
                AT_(ampl, (XDEC), 0, 0)[XDEC_IDX].vec, \
                AT_(ampl, (XINC), 0, 0)[XINC_IDX].vec, \
                AT_(ampl, 0, (YDEC), 0)[YDEC_IDX].vec, \
                AT_(ampl, 0, (YINC), 0)[YINC_IDX].vec, \
                AT_(ampl, 0, 0, (ZDEC))[ZDEC_IDX].vec, \
                AT_(ampl, 0, 0, (ZINC))[ZINC_IDX].vec \
            )

    // 64 PROC_STENCIL_() calls
    #include "gen/zcube4_proc_stencil.hpp"

#undef PROC_STENCIL_

#undef AT_
}

void zc4vec_cell_load(int3_t dim3, VolumeSpan<ZC4VecCell> span, 
                      std::istream& stream)
{
    static constexpr int_t NSizeX = (1 << VectorCell::NRankZ);
    int3_t grid_size = {4 * dim3.x, 4 * dim3.y, 4 * NSizeX * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x / 4, y / 4, z / (4 * NSizeX)})->
            arr[PACK_(x % 4, y % 4, (z / NSizeX) % 4)].arr[z % NSizeX];
    }
}

void zc4vec_cell_store(int3_t dim3, VolumeConstSpan<ZC4VecCell> span, 
                       std::ostream& stream)
{
    static constexpr int_t NSizeX = (1 << VectorCell::NRankZ);
    int3_t grid_size = {4 * dim3.x, 4 * dim3.y, 4 * NSizeX * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream << span.at(dim3, int3_t{x / 4, y / 4, z / (4 * NSizeX)})->
            arr[PACK_(x % 4, y % 4, (z / NSizeX) % 4)].arr[z % NSizeX] << ' ';
    }
}

void zc4vec_cell_fill(int3_t dim3, VolumeSpan<ZC4VecCell> span, 
                      std::function<real_t(int3_t, int3_t)> func)
{
    static constexpr int_t NSizeX = (1 << VectorCell::NRankZ);
    int3_t grid_size = {4 * dim3.x, 4 * dim3.y, 4 * NSizeX * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        span.at(dim3, int3_t{x / 4, y / 4, z / (4 * NSizeX)})->
            arr[PACK_(x % 4, y % 4, (z / NSizeX) % 4)].arr[z % NSizeX] = 
            func(grid_size, idx3);
    }
}

void zc4vec_cell_read(int3_t dim3, VolumeConstSpan<ZC4VecCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func)
{
    static constexpr int_t NSizeX = (1 << VectorCell::NRankZ);
    int3_t grid_size = {4 * dim3.x, 4 * dim3.y, 4 * NSizeX * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        func(grid_size, idx3, 
                span.at(dim3, int3_t{x / 4, y / 4, z / (4 * NSizeX)})->
                arr[PACK_(x % 4, y % 4, (z / NSizeX) % 4)].arr[z % NSizeX]);
    }
}

#undef PACK_
#undef UNPACK_

} // namespace geo

#endif // GEOACOUSTIC_STATIC_ZC4VEC_CELL_HPP_
