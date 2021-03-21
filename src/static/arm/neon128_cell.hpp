#ifndef GEOACOUSTIC_STATIC_ARM_NEON128_CELL_HPP_
#define GEOACOUSTIC_STATIC_ARM_NEON128_CELL_HPP_

#include <iostream>
#include <functional>
#include <arm_neon.h>

#include "types.hpp"
#include "context.hpp"
#include "volume_array.hpp"

namespace geo {

struct alignas(alignof(float64x2_t)) VectorCell
{
    static constexpr int_t NRankX = 0, NRankY = 0, NRankZ = 1;

    union {
        float64x2_t vec;
        double arr[2]{};
    };
};

void vector_cell_proc(int3_t idx3, const Config<VectorCell>& cfg,
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
void vector_cell_proc(int3_t idx3, const Config<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
    vector_cell_test_proc(idx3, cfg, ampl_next, ampl);
    return;

#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    const float64x2_t fdc_1 = vdupq_n_f64(-5.0 / 2.0);
    const float64x2_t fdc_2 = vdupq_n_f64(4.0 / 3.0);

    const float64x2_t bulk = AT_(cfg.bulk.span(), 0, 0, 0);
    const float64x2_t rho = AT_(cfg.rho.span(), 0, 0, 0);
    const float64x2_t speed_sqr = vdivq_f64(bulk, rho);

    const float64x2_t factor = 
        vmulq_f64(
            speed_sqr, 
            vdupq_n_f64(
                (cfg.dtime * cfg.dtime) / (cfg.dspace * cfg.dspace)
                )
            );

    float64x2_t x_dec = AT_(ampl, -1, 0, 0);
    float64x2_t x_inc = AT_(ampl, +1, 0, 0);

    float64x2_t y_dec = AT_(ampl, 0, -1, 0);
    float64x2_t y_inc = AT_(ampl, 0, +1, 0);

    // AT_(ampl, 0, 0, -1) (x[x) (x]x) AT_(ampl, 0, 0, 0)
    float64x2_t z_dec = vextq_f64(AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, 0), 1);

    // AT_(ampl, 0, 0, 0) (x[x) (x]x) AT_(ampl, 0, 0, +1)
    float64x2_t z_dec = vextq_f64(AT_(ampl, 0, 0, 0), AT_(ampl, 0, 0, +1), 1);

    float64x2_t u_dx = 
        vaddq_f64(
            vmulq_f64(fdc_1, AT_(ampl, 0, 0, 0)),
            vmulq_f64(fdc_2, vsubq_f64(x_inc, x_dec))
            );

    float64x2_t u_dy = 
        vaddq_f64(
            vmulq_f64(fdc_1, AT_(ampl, 0, 0, 0)),
            vmulq_f64(fdc_2, vsubq_f64(y_inc, y_dec))
            );

    float64x2_t u_dz = 
        vaddq_f64(
            vmulq_f64(fdc_1, AT_(ampl, 0, 0, 0)),
            vmulq_f64(fdc_2, vsubq_f64(z_inc, z_dec))
            );

    AT_(ampl_next, 0, 0, 0) = 
        vaddq_f64(
            vsubq_f64(
                vmulq_f64(vdupq_n_f64(2.0), AT_(ampl, 0, 0, 0)),
                AT_(ampl_next, 0, 0, 0)
                ),
            vmulq_f64(
                factor,
                vaddq_f64(vaddq_f64(u_dx, u_dy), u_dz)
                )
            );

#undef AT_
}

inline __attribute__((always_inline)) 
void vector_cell_test_proc(int3_t idx3, const Config<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    // AT_(ampl, 0, 0, -1) (x[x) (x]x) AT_(ampl, 0, 0, 0)
    float64x2_t z_dec = vextq_f64(AT_(ampl, 0, 0, -1), AT_(ampl, 0, 0, 0), 1);

    // AT_(ampl, 0, 0, 0) (x[x) (x]x) AT_(ampl, 0, 0, +1)
    float64x2_t z_dec = vextq_f64(AT_(ampl, 0, 0, 0), AT_(ampl, 0, 0, +1), 1);

    AT_(ampl_next, 0, 0, 0) = 
        vmulq_f64(
            vaddq_f64(
                vaddq_f64(
                    vaddq_f64(AT_(ampl, 0, 0, 0), AT_(ampl_next, 0, 0, 0)),
                    vaddq_f64(z_dec, z_inc)
                    ),
                vaddq_f64(
                    vaddq_f64(AT_(ampl, +1, 0, 0), AT_(ampl, -1, 0, 0)),
                    vaddq_f64(AT_(ampl, 0, +1, 0), AT_(ampl, 0, -1, 0))
                    )
                ),
            vdupq_n_f64(1.0 / 8)
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
