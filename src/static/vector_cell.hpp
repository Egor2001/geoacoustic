#ifndef GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_
#define GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_

#include "types.hpp"
#include "context.hpp"
#include "volume_array.hpp"

#include <immintrin.h>

namespace geo {

struct _Alignas(alignof(_mm256_pd)) VectorCell
{
    union {
        __m256d vec;
        double arr[4];
    }
};

inline __attribute__((always_inline)) 
void vector_cell_proc(int3_t idx3, const Config<VectorCell>& cfg,
        VolumeSpan<VectorCell> ampl_next, VolumeSpan<VectorCell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + {(X), (Y), (Z)})->vec)

    ampl_next->vec = 
        _mm256_mul_pd(
            _mm256_sum_pd(
                _mm256_sum_pd(
                    _mm256_sum_pd(ampl->vec,           ampl_next->vec),
                    _mm256_sum_pd(AT_(ampl, +1, 0, 0), AT_(ampl, -1, 0, 0)),
                    ),
                _mm256_sum_pd(
                    _mm256_sum_pd(AT_(ampl, 0, +1, 0), AT_(ampl, 0, -1, 0)),
                    _mm256_sum_pd(AT_(ampl, 0, 0, +1), AT_(ampl, 0, 0, -1)),
                    ),
                ),
            _mm256_set1_pd(1.0 / 8)
            );
#undef AT_
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_VECTOR_CELL_HPP_
