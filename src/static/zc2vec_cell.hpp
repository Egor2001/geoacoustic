#ifndef GEOACOUSTIC_STATIC_ZC2VEC_CELL_HPP_
#define GEOACOUSTIC_STATIC_ZC2VEC_CELL_HPP_

#include <iostream>
#include <functional>

#include "vector_cell.hpp"
#include "packed_stencil.hpp"
#include "types.hpp"

namespace geo {

struct alignas(8 * sizeof(VectorCell)) ZC2VecCell
{
    static constexpr int_t 
        NRankX = 1, NRankY = 1; NRankZ = 1 + VectorCell::NRankZ;

    VectorCell arr[2 * 2 * 2]{};
};

void zc2vec_cell_proc(int3_t idx3, const Config<ZC2VecCell>& cfg,
        VolumeSpan<ZC2VecCell> ampl_next, VolumeSpan<ZC2VecCell> ampl);
void zc2vec_cell_test_proc(int3_t idx3, const Config<ZC2VecCell>& cfg,
        VolumeSpan<ZC2VecCell> ampl_next, VolumeSpan<ZC2VecCell> ampl);

void zc2vec_cell_load(int3_t dim3, VolumeSpan<ZC2VecCell> span, 
                      std::istream& stream);
void zc2vec_cell_store(int3_t dim3, VolumeConstSpan<ZC2VecCell> span, 
                       std::ostream& stream);

void zc2vec_cell_fill(int3_t dim3, VolumeSpan<ZC2VecCell> span, 
                      std::function<real_t(int3_t, int3_t)> func);
void zc2vec_cell_read(int3_t dim3, VolumeConstSpan<ZC2VecCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func);

inline __attribute__((always_inline)) 
void zc2vec_cell_proc(int3_t idx3, const Config<ZC2VecCell>& cfg,
        VolumeSpan<ZC2VecCell> ampl_next, VolumeSpan<ZC2VecCell> ampl)
{
    // zc2vec_cell_test_proc(idx3, cfg, ampl_next, ampl);
    // return;

#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    do { \
        const real_t fdc_1 = -5. / 2., fdc_2 = 4. / 3.; \
        \
        const real_t bulk = \
            AT_(cfg.bulk.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)]; \
        const real_t rho = \
            AT_(cfg.rho.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)]; \
        const real_t speed_sqr = bulk / rho; \
        \
        const real_t factor = \
            speed_sqr * (cfg.dtime * cfg.dtime) / (cfg.dspace * cfg.dspace); \
        \
        real_t u_dx = \
            fdc_1 * AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
            fdc_2 * (AT_(ampl, (X), 0, 0)      [(1 - (X)) + 2*(Y) + 4*(Z)] + \
                     AT_(ampl, ((X) - 1), 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)]); \
        \
        real_t u_dy = \
            fdc_1 * AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
            fdc_2 * (AT_(ampl, 0, (Y), 0)      [(X) + 2*(1 - (Y)) + 4*(Z)] + \
                     AT_(ampl, 0, ((Y) - 1), 0)[(X) + 2*(1 - (Y)) + 4*(Z)]); \
        \
        real_t u_dz = \
            fdc_1 * AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
            fdc_2 * (AT_(ampl, 0, 0, (Z))      [(X) + 2*(Y) + 4*(1 - (Z))] + \
                     AT_(ampl, 0, 0, ((Z) - 1))[(X) + 2*(Y) + 4*(1 - (Z))]); \
        \
        AT_(ampl_next, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] = \
            2.0 * AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] - \
            AT_(ampl_next, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
            factor * (u_dx + u_dy + u_dz); \
    } while (0)

    PROC_STENCIL_(1, 1, 1);
    PROC_STENCIL_(0, 1, 1);
    PROC_STENCIL_(1, 0, 1);
    PROC_STENCIL_(1, 1, 0);
    PROC_STENCIL_(1, 0, 0);
    PROC_STENCIL_(0, 1, 0);
    PROC_STENCIL_(0, 0, 1);
    PROC_STENCIL_(0, 0, 0);

#undef PROC_STENCIL_

#undef AT_
}

inline __attribute__((always_inline)) 
void zc2vec_cell_wide_proc(int3_t idx3, const Config<ZC2VecCell>& cfg,
        VolumeSpan<ZC2VecCell> ampl_next, VolumeSpan<ZC2VecCell> ampl)
{
    // zc2vec_cell_test_proc(idx3, cfg, ampl_next, ampl);
    // return;

#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    do { \
        const real_t fdc_1 = -5. / 2., fdc_2 = 4. / 3., fdc_3 = -1. / 12.; \
        \
        const real_t bulk = \
            AT_(cfg.bulk.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)]; \
        const real_t rho = \
            AT_(cfg.rho.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)]; \
        const real_t speed_sqr = bulk / rho; \
        \
        const real_t factor = \
            speed_sqr * (cfg.dtime * cfg.dtime) / (cfg.dspace * cfg.dspace); \
        \
        real_t u_dx = \
            fdc_1 * AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
            fdc_2 * (AT_(ampl, (X), 0, 0)      [(1 - (X)) + 2*(Y) + 4*(Z)] + \
                     AT_(ampl, ((X) - 1), 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)]) + \
            fdc_3 * (AT_(ampl, +1, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
                     AT_(ampl, -1, 0, 0)[(X) + 2*(Y) + 4*(Z)]); \
        \
        real_t u_dy = \
            fdc_1 * AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
            fdc_2 * (AT_(ampl, 0, (Y), 0)      [(X) + 2*(1 - (Y)) + 4*(Z)] + \
                     AT_(ampl, 0, ((Y) - 1), 0)[(X) + 2*(1 - (Y)) + 4*(Z)]) + \
            fdc_3 * (AT_(ampl, 0, +1, 0)[(X) + 2*(Y) + 4*(Z)] + \
                     AT_(ampl, 0, -1, 0)[(X) + 2*(Y) + 4*(Z)]); \
        \
        real_t u_dz = \
            fdc_1 * AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
            fdc_2 * (AT_(ampl, 0, 0, (Z))      [(X) + 2*(Y) + 4*(1 - (Z))] + \
                     AT_(ampl, 0, 0, ((Z) - 1))[(X) + 2*(Y) + 4*(1 - (Z))]) + \
            fdc_3 * (AT_(ampl, 0, 0, +1)[(X) + 2*(Y) + 4*(Z)] + \
                     AT_(ampl, 0, 0, -1)[(X) + 2*(Y) + 4*(Z)]); \
        \
        AT_(ampl_next, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] = \
            2.0 * AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] - \
            AT_(ampl_next, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
            factor * (u_dx + u_dy + u_dz); \
    } while (0)

    PROC_STENCIL_(1, 1, 1);
    PROC_STENCIL_(0, 1, 1);
    PROC_STENCIL_(1, 0, 1);
    PROC_STENCIL_(1, 1, 0);
    PROC_STENCIL_(1, 0, 0);
    PROC_STENCIL_(0, 1, 0);
    PROC_STENCIL_(0, 0, 1);
    PROC_STENCIL_(0, 0, 0);

#undef PROC_STENCIL_

#undef AT_
}

inline __attribute__((always_inline)) 
void zc2vec_cell_test_proc(int3_t idx3, const Config<ZC2VecCell>& cfg,
        VolumeSpan<ZC2VecCell> ampl_next, VolumeSpan<ZC2VecCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    (AT_(ampl_next, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] = \
     (AT_(ampl, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
      AT_(ampl_next, 0, 0, 0)[(X) + 2*(Y) + 4*(Z)] + \
      AT_(ampl, (X), 0, 0)      [(1 - (X)) + 2*(Y) + 4*(Z)] + \
      AT_(ampl, ((X) - 1), 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)] + \
      AT_(ampl, 0, (Y), 0)      [(X) + 2*(1 - (Y)) + 4*(Z)] + \
      AT_(ampl, 0, ((Y) - 1), 0)[(X) + 2*(1 - (Y)) + 4*(Z)] + \
      AT_(ampl, 0, 0, (Z))      [(X) + 2*(Y) + 4*(1 - (Z))] + \
      AT_(ampl, 0, 0, ((Z) - 1))[(X) + 2*(Y) + 4*(1 - (Z))]) / 8.0)

    PROC_STENCIL_(1, 1, 1);
    PROC_STENCIL_(0, 1, 1);
    PROC_STENCIL_(1, 0, 1);
    PROC_STENCIL_(1, 1, 0);
    PROC_STENCIL_(1, 0, 0);
    PROC_STENCIL_(0, 1, 0);
    PROC_STENCIL_(0, 0, 1);
    PROC_STENCIL_(0, 0, 0);

#undef PROC_STENCIL_

#undef AT_
}

void zc2vec_cell_load(int3_t dim3, VolumeSpan<ZC2VecCell> span, 
                      std::istream& stream)
{
    int3_t grid_size = {2 * dim3.x, 2 * dim3.y, 2 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x / 2, y / 2, z / 2})->
            arr[x % 2 + 2 * (y % 2) + 4 * (z % 2)];
    }
}

void zc2vec_cell_store(int3_t dim3, VolumeConstSpan<ZC2VecCell> span, 
                       std::ostream& stream)
{
    int3_t grid_size = {2 * dim3.x, 2 * dim3.y, 2 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream << span.at(dim3, int3_t{x / 2, y / 2, z / 2})->
            arr[x % 2 + 2 * (y % 2) + 4 * (z % 2)] << ' ';
    }
}

void zc2vec_cell_fill(int3_t dim3, VolumeSpan<ZC2VecCell> span, 
                      std::function<real_t(int3_t, int3_t)> func)
{
    int3_t grid_size = {2 * dim3.x, 2 * dim3.y, 2 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        span.at(dim3, int3_t{x / 2, y / 2, z / 2})->
            arr[x % 2 + 2 * (y % 2) + 4 * (z % 2)] = func(grid_size, idx3);
    }
}

void zc2vec_cell_read(int3_t dim3, VolumeConstSpan<ZC2VecCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func)
{
    int3_t grid_size = {2 * dim3.x, 2 * dim3.y, 2 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        func(grid_size, idx3, span.at(dim3, int3_t{x / 2, y / 2, z / 2})->
                arr[x % 2 + 2 * (y % 2) + 4 * (z % 2)]);
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_ZC2VEC_CELL_HPP_
