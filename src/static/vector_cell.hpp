#ifndef GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_
#define GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_

#include <iostream>
#include <functional>
#include <immintrin.h>

#include "types.hpp"
#include "context.hpp"
#include "volume_array.hpp"

namespace geo {

struct alignas(alignof(__m256d)) VectorCell
{
    static constexpr int_t NRankX = 0, NRankY = 0, NRankZ = 2;

    union {
        __m256d vec;
        double arr[4];
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
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    const __m256d fdc_1 = _mm256_set1_pd(-5.0 / 2.0);
    const __m256d fdc_2 = _mm256_set1_pd(4.0 / 3.0);

    const __m256d bulk = AT_(cfg.bulk.span(), 0, 0, 0);
    const __m256d rho = AT_(cfg.rho.span(), 0, 0, 0);
    const __m256d speed_sqr = _mm256_div_pd(bulk, rho);

    const __m256d factor = 
        _mm256_mul_pd(
            speed_sqr, 
            _mm256_set1_pd(
                (cfg.dtime * cfg.dtime) / (cfg.dspace * cfg.dspace)
                )
            );

    __m256d x_dec = AT_(ampl, -1, 0, 0);
    __m256d x_inc = AT_(ampl, +1, 0, 0);

    __m256d y_dec = AT_(ampl, 0, -1, 0);
    __m256d y_inc = AT_(ampl, 0, +1, 0);

    // AT_(ampl, 0, 0, -1) [x | xxx] ampl->vec
    __m256d z_dec = _mm256_shuffle_pd(
            _mm256_permute2f128_pd(
                AT_(ampl, 0, 0, -1), 
                AT_(ampl, 0, 0, 0), 
                0b00'10'00'01
                ), 
            AT_(ampl, 0, 0, 0), 
            0b0101
            );

    // ampl->vec [xxx | x] AT_(ampl, 0, 0, +1)
    __m256d z_inc = _mm256_shuffle_pd(
            AT_(ampl, 0, 0, 0), 
            _mm256_permute2f128_pd(
                AT_(ampl, 0, 0, 0), 
                AT_(ampl, 0, 0, +1), 
                0b00'10'00'01
                ), 
            0b0101
            );

    __m256d u_dx = 
        _mm256_add_pd(
            _mm256_mul_pd(fdc_1, AT_(ampl, 0, 0, 0)),
            _mm256_mul_pd(fdc_2, _mm256_sub_pd(x_inc, x_dec))
            );

    __m256d u_dy = 
        _mm256_add_pd(
            _mm256_mul_pd(fdc_1, AT_(ampl, 0, 0, 0)),
            _mm256_mul_pd(fdc_2, _mm256_sub_pd(y_inc, y_dec))
            );

    __m256d u_dz = 
        _mm256_add_pd(
            _mm256_mul_pd(fdc_1, AT_(ampl, 0, 0, 0)),
            _mm256_mul_pd(fdc_2, _mm256_sub_pd(z_inc, z_dec))
            );

    AT_(ampl_next, 0, 0, 0) = 
        _mm256_add_pd(
            _mm256_sub_pd(
                _mm256_mul_pd(_mm256_set1_pd(2.0), AT_(ampl, 0, 0, 0)),
                AT_(ampl_next, 0, 0, 0)
                ),
            _mm256_mul_pd(
                factor,
                _mm256_add_pd(_mm256_add_pd(u_dx, u_dy), u_dz)
                )
            );

#undef AT_
}

void vector_cell_test_proc(int3_t idx3, const Config<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->vec)

    // AT_(ampl, 0, 0, -1) [x | xxx] ampl->vec
    __m256d z_dec = _mm256_shuffle_pd(
            _mm256_permute2f128_pd(
                AT_(ampl, 0, 0, -1), 
                AT_(ampl, 0, 0, 0), 
                0b00'10'00'01
                ), 
            AT_(ampl, 0, 0, 0), 
            0b0101
            );

    // ampl->vec [xxx | x] AT_(ampl, 0, 0, +1)
    __m256d z_inc = _mm256_shuffle_pd(
            AT_(ampl, 0, 0, 0), 
            _mm256_permute2f128_pd(
                AT_(ampl, 0, 0, 0), 
                AT_(ampl, 0, 0, +1), 
                0b00'10'00'01
                ), 
            0b0101
            );

    ampl_next->vec = 
        _mm256_mul_pd(
            _mm256_add_pd(
                _mm256_add_pd(
                    _mm256_add_pd(AT_(ampl, 0, 0, 0), AT_(ampl_next, 0, 0, 0)),
                    _mm256_add_pd(z_dec, z_inc)
                    ),
                _mm256_add_pd(
                    _mm256_add_pd(AT_(ampl, +1, 0, 0), AT_(ampl, -1, 0, 0)),
                    _mm256_add_pd(AT_(ampl, 0, +1, 0), AT_(ampl, 0, -1, 0))
                    )
                ),
            _mm256_set1_pd(1.0 / 8)
            );
#undef AT_
}

void vector_cell_load(int3_t dim3, VolumeSpan<VectorCell> span, 
                      std::istream& stream)
{
    int3_t grid_size = {dim3.x, dim3.y, 4 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x, y, z / 4})->arr[z % 4];
    }
}

void vector_cell_store(int3_t dim3, VolumeConstSpan<VectorCell> span, 
                       std::ostream& stream)
{
    int3_t grid_size = {dim3.x, dim3.y, 4 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        stream << span.at(dim3, int3_t{x, y, z / 4})->arr[z % 4] << ' ';
    }
}

void vector_cell_fill(int3_t dim3, VolumeSpan<VectorCell> span, 
                      std::function<real_t(int3_t, int3_t)> func)
{
    int3_t grid_size = {dim3.x, dim3.y, 4 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        span.at(dim3, int3_t{x, y, z / 4})->arr[z % 4] = func(grid_size, idx3);
    }
}

void vector_cell_read(int3_t dim3, VolumeConstSpan<VectorCell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func)
{
    int3_t grid_size = {dim3.x, dim3.y, 4 * dim3.z};
    for (int_t z = 0; z < grid_size.z; ++z)
    for (int_t y = 0; y < grid_size.y; ++y)
    for (int_t x = 0; x < grid_size.x; ++x)
    {
        int3_t idx3 = int3_t{x, y, z};
        func(grid_size, idx3, span.at(dim3, int3_t{x, y, z / 4})->arr[z % 4]);
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_
