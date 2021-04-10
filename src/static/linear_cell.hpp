#ifndef GEOACOUSTIC_STATIC_LINEAR_CELL_HPP_
#define GEOACOUSTIC_STATIC_LINEAR_CELL_HPP_

#include <iostream>
#include <functional>

#include "types.hpp"
#include "context.hpp"
#include "volume_array.hpp"
#include "single_stencil.hpp"

namespace geo {

struct LinearCell
{
    static constexpr int_t NRankX = 0, NRankY = 0, NRankZ = 0;

    real_t data{};
};

void linear_cell_proc(int3_t idx3, const Config<LinearCell>& cfg,
        VolumeSpan<LinearCell> ampl_next, VolumeSpan<LinearCell> ampl);
void linear_cell_wide_proc(int3_t idx3, const Config<LinearCell>& cfg,
        VolumeSpan<LinearCell> ampl_next, VolumeSpan<LinearCell> ampl);
void linear_cell_test_proc(int3_t idx3, const Config<LinearCell>& cfg,
        VolumeSpan<LinearCell> ampl_next, VolumeSpan<LinearCell> ampl);

void linear_cell_load(int3_t dim3, VolumeSpan<LinearCell> span, 
                      std::istream& stream);
void linear_cell_store(int3_t dim3, VolumeConstSpan<LinearCell> span, 
                       std::ostream& stream);

void linear_cell_fill(int3_t dim3, VolumeSpan<LinearCell> span, 
                      std::function<real_t(int3_t, int3_t)> func);
void linear_cell_read(int3_t dim3, VolumeConstSpan<LinearCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func);

inline __attribute__((always_inline)) 
void linear_cell_proc(int3_t idx3, const Config<LinearCell>& cfg,
        VolumeSpan<LinearCell> ampl_next, VolumeSpan<LinearCell> ampl)
{
    // linear_cell_test_proc(idx3, cfg, ampl_next, ampl);
    // return;

#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->data)

    GEO_SINGLE_STENCIL_USEDIV(
            AT_(cfg.bulk.span(), 0, 0, 0), AT_(cfg.rho.span(), 0, 0, 0), 
            cfg.dtime, cfg.dspace, AT_(ampl_next, 0, 0, 0), AT_(ampl, 0, 0, 0), 
            AT_(ampl, -1, 0, 0), AT_(ampl, +1, 0, 0), // XDEC XINC
            AT_(ampl, 0, -1, 0), AT_(ampl, 0, +1, 0), // YDEC YINC
            AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, +1)  // ZDEC ZINC
            );

#undef AT_
}

inline __attribute__((always_inline)) 
void linear_cell_wide_proc(int3_t idx3, const Config<LinearCell>& cfg,
        VolumeSpan<LinearCell> ampl_next, VolumeSpan<LinearCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->data)

    GEO_SINGLE_STENCIL_WIDE_USEDIV(
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
void linear_cell_test_proc(int3_t idx3, const Config<LinearCell>& cfg,
        VolumeSpan<LinearCell> ampl_next, VolumeSpan<LinearCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->data)
/*
    AT_(ampl_next, 0, 0, 0) = 
        (AT_(ampl, 0, 0, 0) + 
         AT_(ampl_next, 0, 0, 0) + 
         AT_(ampl, +1, 0, 0) + 
         AT_(ampl, 0, +1, 0) + 
         AT_(ampl, 0, 0, +1) + 
         AT_(ampl, -1, 0, 0) + 
         AT_(ampl, 0, -1, 0) + 
         AT_(ampl, 0, 0, -1)) / 8.0;
*/

    GEO_SINGLE_STENCIL_TEST(AT_(ampl_next, 0, 0, 0), AT_(ampl, 0, 0, 0), 
         AT_(ampl, -1, 0, 0), AT_(ampl, +1, 0, 0), // XDEC XINC
         AT_(ampl, 0, -1, 0), AT_(ampl, 0, +1, 0), // YDEC YINC
         AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, +1)  // ZDEC ZINC
         );

#undef AT_
}

void linear_cell_load(int3_t dim3, VolumeSpan<LinearCell> span, 
                      std::istream& stream)
{
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x, y, z})->data;
    }
}

void linear_cell_store(int3_t dim3, VolumeConstSpan<LinearCell> span, 
                       std::ostream& stream)
{
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        stream << span.at(dim3, int3_t{x, y, z})->data << ' ';
    }
}

void linear_cell_fill(int3_t dim3, VolumeSpan<LinearCell> span, 
                      std::function<real_t(int3_t, int3_t)> func)
{
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        span.at(dim3, idx3)->data = func(dim3, idx3);
    }
}

void linear_cell_read(int3_t dim3, VolumeConstSpan<LinearCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func)
{
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        func(dim3, idx3, span.at(dim3, idx3)->data);
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_LINEAR_CELL_HPP_
