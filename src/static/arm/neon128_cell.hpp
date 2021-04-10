#ifndef GEOACOUSTIC_STATIC_ARM_NEON128_CELL_HPP_
#define GEOACOUSTIC_STATIC_ARM_NEON128_CELL_HPP_

#include <iostream>
#include <functional>
#include <arm_neon.h>

#include "types.hpp"
#include "config.hpp"
#include "volume_array.hpp"
#include "neon128_stencil.hpp"

namespace geo {

struct alignas(alignof(float64x2_t)) VectorCell
{
    static constexpr int_t NRankX = 0, NRankY = 0, NRankZ = 1;

    union {
        float64x2_t vec;
        double arr[2]{};
    };
};

void vector_cell_simplified_proc(int3_t idx3, 
        const SimplifiedConfig<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl);
void vector_cell_simplified_wide_proc(int3_t idx3, 
        const SimplifiedConfig<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl);
void vector_cell_proc(int3_t idx3, 
        const GeneralizedConfig<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl);
void vector_cell_wide_proc(int3_t idx3, 
        const GeneralizedConfig<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl);
void vector_cell_test_proc(int3_t idx3, const Config<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl);

void vector_cell_load(int3_t dim3, VolumeSpan<VectorCell> span, 
                      std::istream& stream);
void vector_cell_store(int3_t dim3, VolumeConstSpan<VectorCell> span, 
                       std::ostream& stream);

void vector_cell_fill(int3_t dim3, VolumeSpan<VectorCell> span, 
                      std::function<real_t(int3_t, int3_t)> func);
void vector_cell_read(int3_t dim3, VolumeConstSpan<VectorCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func);

inline __attribute__((always_inline)) 
void vector_cell_simplified_proc(int3_t idx3, 
        const SimplifiedConfig<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    GEO_PACKED_STENCIL_FACTOR(
            AT_(cfg.factor.span(), 0, 0, 0), 
            AT_(ampl_next, 0, 0, 0), AT_(ampl, 0, 0, 0),
            AT_(ampl, -1, 0, 0), AT_(ampl, +1, 0, 0), // XDEC XINC
            AT_(ampl, 0, -1, 0), AT_(ampl, 0, +1, 0), // YDEC YINC
            AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, +1)  // ZDEC ZINC
            );

#undef AT_
}

inline __attribute__((always_inline)) 
void vector_cell_simplified_wide_proc(int3_t idx3, 
        const SimplifiedConfig<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    GEO_PACKED_STENCIL_WIDE_FACTOR(
            AT_(cfg.factor.span(), 0, 0, 0), 
            AT_(ampl_next, 0, 0, 0), AT_(ampl, 0, 0, 0),
            // XDEC XINC XDEC2 XINC2
            AT_(ampl, -1, 0, 0), AT_(ampl, +1, 0, 0),
            AT_(ampl, -2, 0, 0), AT_(ampl, +2, 0, 0),
            // YDEC YINC YDEC2 YINC2
            AT_(ampl, 0, -1, 0), AT_(ampl, 0, +1, 0),
            AT_(ampl, 0, -2, 0), AT_(ampl, 0, +2, 0),
            // ZDEC ZINC ZDEC2 ZINC2
            AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, +1),
            AT_(ampl, 0, 0, -2), AT_(ampl, 0, 0, +2)
            );

#undef AT_
}

inline __attribute__((always_inline)) 
void vector_cell_proc(int3_t idx3, 
        const GeneralizedConfig<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    GEO_PACKED_STENCIL_USEDIV(
            AT_(cfg.bulk.span(), 0, 0, 0), AT_(cfg.rho.span(), 0, 0, 0),
            cfg.dtime, cfg.dspace, AT_(ampl_next, 0, 0, 0), AT_(ampl, 0, 0, 0),
            AT_(ampl, -1, 0, 0), AT_(ampl, +1, 0, 0), // XDEC XINC
            AT_(ampl, 0, -1, 0), AT_(ampl, 0, +1, 0), // YDEC YINC
            AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, +1)  // ZDEC ZINC
            );

#undef AT_
}

inline __attribute__((always_inline)) 
void vector_cell_wide_proc(int3_t idx3, 
        const GeneralizedConfig<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    GEO_PACKED_STENCIL_WIDE_USEDIV(
            AT_(cfg.bulk.span(), 0, 0, 0), AT_(cfg.rho.span(), 0, 0, 0),
            cfg.dtime, cfg.dspace, AT_(ampl_next, 0, 0, 0), AT_(ampl, 0, 0, 0),
            // XDEC XINC XDEC2 XINC2
            AT_(ampl, -1, 0, 0), AT_(ampl, +1, 0, 0),
            AT_(ampl, -2, 0, 0), AT_(ampl, +2, 0, 0),
            // YDEC YINC YDEC2 YINC2
            AT_(ampl, 0, -1, 0), AT_(ampl, 0, +1, 0),
            AT_(ampl, 0, -2, 0), AT_(ampl, 0, +2, 0),
            // ZDEC ZINC ZDEC2 ZINC2
            AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, +1),
            AT_(ampl, 0, 0, -2), AT_(ampl, 0, 0, +2)
            );

#undef AT_
}

inline __attribute__((always_inline)) 
void vector_cell_test_proc(int3_t idx3, const Config<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    GEO_PACKED_STENCIL_TEST(
            AT_(ampl_next, 0, 0, 0), AT_(ampl, 0, 0, 0),
            AT_(ampl, -1, 0, 0), AT_(ampl, +1, 0, 0),
            AT_(ampl, 0, -1, 0), AT_(ampl, 0, +1, 0),
            AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, +1)
            );

#undef AT_
}

void vector_cell_load(int3_t dim3, VolumeSpan<VectorCell> span, 
                      std::istream& stream)
{
    int3_t grid_size = {dim3.x, dim3.y, 2 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x, y, z / 2})->arr[z % 2];
    }
}

void vector_cell_store(int3_t dim3, VolumeConstSpan<VectorCell> span, 
                       std::ostream& stream)
{
    int3_t grid_size = {dim3.x, dim3.y, 2 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream << span.at(dim3, int3_t{x, y, z / 2})->arr[z % 2] << ' ';
    }
}

void vector_cell_fill(int3_t dim3, VolumeSpan<VectorCell> span, 
                      std::function<real_t(int3_t, int3_t)> func)
{
    int3_t grid_size = {dim3.x, dim3.y, 2 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        span.at(dim3, int3_t{x, y, z / 2})->arr[z % 2] = func(grid_size, idx3);
    }
}

void vector_cell_read(int3_t dim3, VolumeConstSpan<VectorCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func)
{
    int3_t grid_size = {dim3.x, dim3.y, 2 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        func(grid_size, idx3, span.at(dim3, int3_t{x, y, z / 2})->arr[z % 2]);
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_ARM_NEON128_CELL_HPP_
