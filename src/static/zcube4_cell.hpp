#ifndef GEOACOUSTIC_STATIC_ZCUBE4_CELL_HPP_
#define GEOACOUSTIC_STATIC_ZCUBE4_CELL_HPP_

#include <iostream>
#include <functional>

#include "types.hpp"
#include "single_stencil.hpp"

namespace geo {

#define PACK_(X, Y, Z) \
    ((((X) & 1) << 0) | (((X) & 2) << 2) | \
     (((Y) & 1) << 1) | (((Y) & 2) << 3) | \
     (((Z) & 1) << 2) | (((Z) & 2) << 4))

#define UNPACK_(IDX) \
    { ((IDX) & 0b000001 >> 0) | (((IDX) & 0b001000) >> 2), \
      ((IDX) & 0b000010 >> 1) | (((IDX) & 0b010000) >> 3), \
      ((IDX) & 0b000100 >> 2) | (((IDX) & 0b100000) >> 4) }

struct alignas(8 * sizeof(real_t)) ZCube4Cell
{
    static constexpr int_t NRankX = 2, NRankY = 2, NRankZ = 2;

    real_t arr[4 * 4 * 4]{};
};

void zcube4_cell_proc(int3_t idx3, const Config<ZCube4Cell>& cfg,
        VolumeSpan<ZCube4Cell> ampl_next, VolumeSpan<ZCube4Cell> ampl);
void zcube4_cell_wide_proc(int3_t idx3, const Config<ZCube4Cell>& cfg,
        VolumeSpan<ZCube4Cell> ampl_next, VolumeSpan<ZCube4Cell> ampl);
void zcube4_cell_test_proc(int3_t idx3, const Config<ZCube4Cell>& cfg,
        VolumeSpan<ZCube4Cell> ampl_next, VolumeSpan<ZCube4Cell> ampl);

void zcube4_cell_load(int3_t dim3, VolumeSpan<ZCube4Cell> span, 
                      std::istream& stream);
void zcube4_cell_store(int3_t dim3, VolumeConstSpan<ZCube4Cell> span, 
                       std::ostream& stream);

void zcube4_cell_fill(int3_t dim3, VolumeSpan<ZCube4Cell> span, 
                      std::function<real_t(int3_t, int3_t)> func);
void zcube4_cell_read(int3_t dim3, VolumeConstSpan<ZCube4Cell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func);

inline __attribute__((always_inline)) 
void zcube4_cell_proc(int3_t idx3, const Config<ZCube4Cell>& cfg,
        VolumeSpan<ZCube4Cell> ampl_next, VolumeSpan<ZCube4Cell> ampl)
{
    // zcube4_cell_test_proc(idx3, cfg, ampl_next, ampl);
    // return;

#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(IDX, \
        XDEC, XDEC_IDX, XINC, XINC_IDX, \
        YDEC, YDEC_IDX, YINC, YINC_IDX, \
        ZDEC, ZDEC_IDX, ZINC, ZINC_IDX \
        ) \
    GEO_SINGLE_STENCIL_USEDIV( \
                AT_(cfg.bulk.span(), 0, 0, 0)[IDX], \
                AT_(cfg.rho.span(), 0, 0, 0)[IDX], \
                cfg.dtime, cfg.dspace, \
                AT_(ampl_next, 0, 0, 0)[IDX], \
                AT_(ampl, 0, 0, 0)     [IDX], \
                AT_(ampl, (XDEC), 0, 0)[XDEC_IDX], \
                AT_(ampl, (XINC), 0, 0)[XINC_IDX], \
                AT_(ampl, 0, (YDEC), 0)[YDEC_IDX], \
                AT_(ampl, 0, (YINC), 0)[YINC_IDX], \
                AT_(ampl, 0, 0, (ZDEC))[ZDEC_IDX], \
                AT_(ampl, 0, 0, (ZINC))[ZINC_IDX] \
            )

    // 64 PROC_STENCIL_() calls
    #include "gen/zcube4_proc_stencil.hpp"

#undef PROC_STENCIL_

#undef AT_
}

inline __attribute__((always_inline)) 
void zcube4_cell_wide_proc(int3_t idx3, const Config<ZCube4Cell>& cfg,
        VolumeSpan<ZCube4Cell> ampl_next, VolumeSpan<ZCube4Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(IDX, \
        XDEC, XDEC_IDX, XINC, XINC_IDX, XDEC2, XDEC2_IDX, XINC2, XINC2_IDX, \
        YDEC, YDEC_IDX, YINC, YINC_IDX, YDEC2, YDEC2_IDX, YINC2, YINC2_IDX, \
        ZDEC, ZDEC_IDX, ZINC, ZINC_IDX, ZDEC2, ZDEC2_IDX, ZINC2, ZINC2_IDX \
        ) \
    GEO_SINGLE_STENCIL_WIDE_USEDIV( \
                AT_(cfg.bulk.span(), 0, 0, 0)[IDX], \
                AT_(cfg.rho.span(), 0, 0, 0)[IDX], \
                cfg.dtime, cfg.dspace, \
                AT_(ampl_next, 0, 0, 0) [IDX], \
                AT_(ampl, 0, 0, 0)      [IDX], \
                \
                AT_(ampl, (XDEC), 0, 0) [XDEC_IDX], \
                AT_(ampl, (XINC), 0, 0) [XINC_IDX], \
                AT_(ampl, (XDEC2), 0, 0)[XDEC2_IDX], \
                AT_(ampl, (XINC2), 0, 0)[XINC2_IDX], \
                \
                AT_(ampl, 0, (YDEC), 0) [YDEC_IDX], \
                AT_(ampl, 0, (YINC), 0) [YINC_IDX], \
                AT_(ampl, 0, (YDEC2), 0)[YDEC2_IDX], \
                AT_(ampl, 0, (YINC2), 0)[YINC2_IDX], \
                \
                AT_(ampl, 0, 0, (ZDEC)) [ZDEC_IDX], \
                AT_(ampl, 0, 0, (ZINC)) [ZINC_IDX], \
                AT_(ampl, 0, 0, (ZDEC2))[ZDEC2_IDX], \
                AT_(ampl, 0, 0, (ZINC2))[ZINC2_IDX] \
            )

    // 64 PROC_STENCIL_() calls
    #include "gen/zcube4_proc_stencil_wide.hpp"

#undef PROC_STENCIL_

#undef AT_
}

inline __attribute__((always_inline)) 
void zcube4_cell_test_proc(int3_t idx3, const Config<ZCube4Cell>& cfg,
        VolumeSpan<ZCube4Cell> ampl_next, VolumeSpan<ZCube4Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(IDX, \
        XDEC, XDEC_IDX, XINC, XINC_IDX, \
        YDEC, YDEC_IDX, YINC, YINC_IDX, \
        ZDEC, ZDEC_IDX, ZINC, ZINC_IDX \
        ) \
    GEO_SINGLE_STENCIL_TEST( \
                AT_(ampl_next, 0, 0, 0)[IDX], \
                AT_(ampl, 0, 0, 0)     [IDX], \
                AT_(ampl, (XDEC), 0, 0)[XDEC_IDX], \
                AT_(ampl, (XINC), 0, 0)[XINC_IDX], \
                AT_(ampl, 0, (YDEC), 0)[YDEC_IDX], \
                AT_(ampl, 0, (YINC), 0)[YINC_IDX], \
                AT_(ampl, 0, 0, (ZDEC))[ZDEC_IDX], \
                AT_(ampl, 0, 0, (ZINC))[ZINC_IDX] \
            )

    // 64 PROC_STENCIL_() calls
    #include "gen/zcube4_proc_stencil.hpp"

#undef PROC_STENCIL_

#undef AT_
}

void zcube4_cell_load(int3_t dim3, VolumeSpan<ZCube4Cell> span, 
                      std::istream& stream)
{
    int3_t grid_size = {4 * dim3.x, 4 * dim3.y, 4 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x / 4, y / 4, z / 4})->
            arr[PACK_(x % 4, y % 4, z % 4)];
    }
}

void zcube4_cell_store(int3_t dim3, VolumeConstSpan<ZCube4Cell> span, 
                       std::ostream& stream)
{
    int3_t grid_size = {4 * dim3.x, 4 * dim3.y, 4 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream << span.at(dim3, int3_t{x / 4, y / 4, z / 4})->
            arr[PACK_(x % 4, y % 4, z % 4)] << ' ';
    }
}

void zcube4_cell_fill(int3_t dim3, VolumeSpan<ZCube4Cell> span, 
                      std::function<real_t(int3_t, int3_t)> func)
{
    int3_t grid_size = {4 * dim3.x, 4 * dim3.y, 4 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        span.at(dim3, int3_t{x / 4, y / 4, z / 4})->
            arr[PACK_(x % 4, y % 4, z % 4)] = func(grid_size, idx3);
    }
}

void zcube4_cell_read(int3_t dim3, VolumeConstSpan<ZCube4Cell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func)
{
    int3_t grid_size = {4 * dim3.x, 4 * dim3.y, 4 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        func(grid_size, idx3, span.at(dim3, int3_t{x / 4, y / 4, z / 4})->
                arr[PACK_(x % 4, y % 4, z % 4)]);
    }
}

#undef PACK_
#undef UNPACK_

} // namespace geo

#endif // GEOACOUSTIC_STATIC_ZCUBE4_CELL_HPP_
