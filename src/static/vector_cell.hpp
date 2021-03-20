#ifndef GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_
#define GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_

#include <iostream>
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

inline __attribute__((always_inline)) 
void vector_cell_proc(int3_t idx3, const Config<VectorCell>& cfg,
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
    for (int_t z = 0; z < (4 * dim3.z); ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        stream >> span.at(dim3, int3_t{x, y, z / 4})->arr[z % 4];
    }
}

void vector_cell_store(int3_t dim3, VolumeConstSpan<VectorCell> span, 
                       std::ostream& stream)
{
    for (int_t z = 0; z < (4 * dim3.z); ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        stream << span.at(dim3, int3_t{x, y, z / 4})->arr[z % 4] << ' ';
    }
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_
