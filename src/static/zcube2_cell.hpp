#ifndef GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_
#define GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_

#include "types.hpp"

namespace geo {

struct _Alignas(64) ZCube2Cell
{
    real_t arr[2 * 2 * 2];
};

inline __attribute__((always_inline)) 
void zorder_cell_proc(int3_t idx3, const Config<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    (AMPL).at(cfg.grid_size, idx3 + {(X), (Y), (Z)})->vec

#define PROC_(X, Y, Z) \
    (ampl_next->arr[(X) + 2*(Y) + 4*(Z)] = \
     (ampl->arr[(X) + 2*(Y) + 4*(Z)] + ampl->arr[(X) + 2*(Y) + 4*(Z)]) + \
     (ampl->at) + \
     / 8.0)

    PROC(1, 1, 1)
    PROC(0, 1, 1)
    PROC(1, 0, 1)
    PROC(1, 1, 0)
    PROC(1, 0, 0)
    PROC(0, 1, 0)
    PROC(0, 0, 1)
    PROC(0, 0, 0)

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

#endif // GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_
