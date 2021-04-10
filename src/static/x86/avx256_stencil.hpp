#ifndef GEOACOUSTIC_STATIC_X86_PACKED_STENCIL_HPP_
#define GEOACOUSTIC_STATIC_X86_PACKED_STENCIL_HPP_

#include <immintrin.h>

#include "types.hpp"

namespace geo {

// finite difference coefficients, accuracy = 1
const __m256d kFDC1_1v = _mm256_set1_pd(-2.0);
const __m256d kFDC1_2v = _mm256_set1_pd(1.0);

// finite difference coefficients, accuracy = 2
const __m256d kFDC2_1v = _mm256_set1_pd(-5.0 / 2.0);
const __m256d kFDC2_2v = _mm256_set1_pd(4.0 / 3.0);
const __m256d kFDC2_3v = _mm256_set1_pd(-1.0 / 12.0);

} // namespace geo

// vectorized, accuracy = 1, using pre-computed isotropic factor
#define GEO_PACKED_STENCIL_FACTOR(FACTOR, NEXT, CUR, \
                                  XDEC, XINC, YDEC, YINC, ZDEC, ZINC) \
    do { \
        const __m256d cur = (CUR); \
        /* ZDEC [x | xxx] cur */ \
        __m256d z_dec = _mm256_shuffle_pd(_mm256_permute2f128_pd( \
                    (ZDEC), cur, 0b00'10'00'01), cur, 0b0101); \
        /* cur [xxx | x] ZINC */ \
        __m256d z_inc = _mm256_shuffle_pd(cur, \
                _mm256_permute2f128_pd(cur, (ZINC), 0b00'10'00'01), 0b0101); \
        \
        (NEXT) = \
            _mm256_add_pd( \
                _mm256_sub_pd( \
                    _mm256_mul_pd(_mm256_set1_pd(2.0), cur), \
                    (NEXT) \
                    ), \
                _mm256_mul_pd( \
                    (FACTOR), \
                    _mm256_add_pd( \
                        _mm256_mul_pd( \
                            _mm256_mul_pd(_mm256_set1_pd(3.0), kFDC1_1v), \
                            cur \
                            ), \
                        _mm256_mul_pd( \
                            kFDC1_2v, \
                            _mm256_add_pd( \
                                _mm256_add_pd( \
                                    _mm256_add_pd((XDEC), (XINC)),  \
                                    _mm256_add_pd((YDEC), (YINC)) \
                                    ), \
                                _mm256_add_pd(z_dec, z_inc) \
                                ) \
                            ) \
                        ) \
                    ) \
                ); \
    } while (false)

// vectorized, accuracy = 2, using pre-computed isotropic factor
#define GEO_PACKED_STENCIL_WIDE_FACTOR( \
                FACTOR, NEXT, CUR, \
                XDEC, XINC, XDEC2, XINC2, \
                YDEC, YINC, YDEC2, YINC2, \
                ZDEC, ZINC, ZDEC2, ZINC2 \
            ) \
    do { \
        const __m256d cur = (CUR); \
        /* ZDEC [x | xxx] cur */ \
        __m256d z_dec = _mm256_shuffle_pd(_mm256_permute2f128_pd( \
                    (ZDEC), cur, 0b00'10'00'01), cur, 0b0101); \
        /* ZDEC [xx | xx] cur */ \
        __m256d z_dec2 = _mm256_permute2f128_pd((ZDEC), cur, 0b00'10'00'01); \
        /* cur [xxx | x] ZINC */ \
        __m256d z_inc = _mm256_shuffle_pd(cur, \
                _mm256_permute2f128_pd(cur, (ZINC), 0b00'10'00'01), 0b0101); \
        /* cur [xx | xx] ZINC */ \
        __m256d z_inc2 = _mm256_permute2f128_pd(cur, (ZINC), 0b00'10'00'01); \
        \
        (NEXT) = \
            _mm256_add_pd( \
                _mm256_sub_pd( \
                    _mm256_mul_pd(_mm256_set1_pd(2.0), cur), \
                    (NEXT) \
                    ), \
                _mm256_mul_pd( \
                    (FACTOR), \
                    _mm256_add_pd( \
                        _mm256_mul_pd( \
                            _mm256_mul_pd(_mm256_set1_pd(3.0), kFDC2_1v), \
                            cur \
                            ), \
                        _mm256_add_pd( \
                            _mm256_mul_pd( \
                                kFDC2_2v, \
                                _mm256_add_pd( \
                                    _mm256_add_pd( \
                                        _mm256_add_pd((XDEC), (XINC)), \
                                        _mm256_add_pd((YDEC), (YINC)) \
                                        ), \
                                    _mm256_add_pd(z_dec, z_inc) \
                                    ) \
                                ), \
                            _mm256_mul_pd( \
                                kFDC2_3v, \
                                _mm256_add_pd( \
                                    _mm256_add_pd( \
                                        _mm256_add_pd((XDEC2), (XINC2)), \
                                        _mm256_add_pd((YDEC2), (YINC2)) \
                                        ), \
                                    _mm256_add_pd(z_dec2, z_inc2) \
                                    ) \
                                ) \
                            ) \
                        ) \
                    ) \
                ); \
    } while (false)

// vectorized, accuracy = 1, with mul-by-inv
#define GEO_PACKED_STENCIL_USEINV(BULK, INV_RHO, DTIME, INV_DSPACE, NEXT, CUR, \
                                  XDEC, XINC, YDEC, YINC, ZDEC, ZINC) \
    do { \
        const __m256d speed_sqr = _mm256_mul_pd((BULK), (INV_RHO)); \
        const real_t ratio = (DTIME) * (INV_DSPACE); \
        const __m256d factor = \
            _mm256_mul_pd(speed_sqr, _mm256_set1_pd(ratio * ratio)); \
        \
        const __m256d cur = (CUR); \
        \
        __m256d x_dec = (XDEC); \
        __m256d x_inc = (XINC); \
        __m256d y_dec = (YDEC); \
        __m256d y_inc = (YINC); \
        /* ZDEC [x | xxx] cur */ \
        __m256d z_dec = _mm256_shuffle_pd(_mm256_permute2f128_pd( \
                    (ZDEC), cur, 0b00'10'00'01), cur, 0b0101); \
        /* cur [xxx | x] ZINC */ \
        __m256d z_inc = _mm256_shuffle_pd(cur, \
                _mm256_permute2f128_pd(cur, (ZINC), 0b00'10'00'01), 0b0101); \
        \
        __m256d u_dx = _mm256_add_pd(_mm256_mul_pd(kFDC1_1v, cur), \
                _mm256_mul_pd(kFDC1_2v, _mm256_add_pd(x_inc, x_dec))); \
        \
        __m256d u_dy = _mm256_add_pd(_mm256_mul_pd(kFDC1_1v, cur), \
                _mm256_mul_pd(kFDC1_2v, _mm256_add_pd(y_inc, y_dec))); \
        \
        __m256d u_dz = _mm256_add_pd(_mm256_mul_pd(kFDC1_1v, cur), \
                _mm256_mul_pd(kFDC1_2v, _mm256_add_pd(z_inc, z_dec))); \
        \
        (NEXT) = _mm256_add_pd( \
                _mm256_sub_pd(_mm256_mul_pd( \
                    _mm256_set1_pd(2.0), cur), (NEXT) \
                    ), \
                _mm256_mul_pd( \
                    factor, _mm256_add_pd(_mm256_add_pd(u_dx, u_dy), u_dz) \
                    ) \
                ); \
    } while (false)

// vectorized, accuracy = 2, with mul-by-inv
#define GEO_PACKED_STENCIL_WIDE_USEINV( \
                BULK, INV_RHO, DTIME, INV_DSPACE, NEXT, CUR, \
                XDEC, XINC, XDEC2, XINC2, \
                YDEC, YINC, YDEC2, YINC2, \
                ZDEC, ZINC, ZDEC2, ZINC2 \
            ) \
    do { \
        const __m256d speed_sqr = _mm256_mul_pd((BULK), (INV_RHO)); \
        const real_t ratio = (DTIME) * (INV_DSPACE); \
        const __m256d factor = \
            _mm256_mul_pd(speed_sqr, _mm256_set1_pd(ratio * ratio)); \
        \
        const __m256d cur = (CUR); \
        \
        __m256d x_dec = (XDEC), x_dec2 = (XDEC2); \
        __m256d x_inc = (XINC), x_inc2 = (XINC2); \
        __m256d y_dec = (YDEC), y_dec2 = (YDEC2); \
        __m256d y_inc = (YINC), y_inc2 = (YINC2); \
        /* ZDEC [x | xxx] cur */ \
        __m256d z_dec = _mm256_shuffle_pd(_mm256_permute2f128_pd( \
                    (ZDEC), cur, 0b00'10'00'01), cur, 0b0101); \
        /* ZDEC [xx | xx] cur */ \
        __m256d z_dec2 = _mm256_permute2f128_pd((ZDEC), cur, 0b00'10'00'01); \
        /* cur [xxx | x] ZINC */ \
        __m256d z_inc = _mm256_shuffle_pd(cur, \
                _mm256_permute2f128_pd(cur, (ZINC), 0b00'10'00'01), 0b0101); \
        /* cur [xx | xx] ZINC */ \
        __m256d z_inc2 = _mm256_permute2f128_pd(cur, (ZINC), 0b00'10'00'01); \
        \
        __m256d u_dx = _mm256_add_pd(_mm256_mul_pd(kFDC2_1v, cur), \
                _mm256_add_pd(\
                    _mm256_mul_pd(kFDC2_2v, _mm256_add_pd(x_inc, x_dec)), \
                    _mm256_mul_pd(kFDC2_3v, _mm256_add_pd(x_inc2, x_dec2)) \
                    ) \
                ); \
        __m256d u_dy = _mm256_add_pd(_mm256_mul_pd(kFDC2_1v, cur), \
                _mm256_add_pd(\
                    _mm256_mul_pd(kFDC2_2v, _mm256_add_pd(y_inc, y_dec)), \
                    _mm256_mul_pd(kFDC2_3v, _mm256_add_pd(y_inc2, y_dec2)) \
                    ) \
                ); \
        __m256d u_dz = _mm256_add_pd(_mm256_mul_pd(kFDC2_1v, cur), \
                _mm256_add_pd(\
                    _mm256_mul_pd(kFDC2_2v, _mm256_add_pd(z_inc, z_dec)), \
                    _mm256_mul_pd(kFDC2_3v, _mm256_add_pd(z_inc2, z_dec2)) \
                    ) \
                ); \
        \
        (NEXT) = _mm256_add_pd( \
                _mm256_sub_pd(_mm256_mul_pd( \
                    _mm256_set1_pd(2.0), cur), (NEXT) \
                    ), \
                _mm256_mul_pd( \
                    factor, _mm256_add_pd(_mm256_add_pd(u_dx, u_dy), u_dz) \
                    ) \
                ); \
    } while (false)

// TODO(@geome_try): to use copy-paste in production!
// vectorized, accuracy = 1, with divisions
#define GEO_PACKED_STENCIL_USEDIV(BULK, RHO, DTIME, DSPACE, NEXT, CUR, \
                                  XDEC, XINC, YDEC, YINC, ZDEC, ZINC) \
    GEO_PACKED_STENCIL_USEINV( \
            BULK, _mm256_div_pd(_mm256_set1_pd(1.0), (RHO)), \
            DTIME, 1.0 / (DSPACE), NEXT, CUR, \
            XDEC, XINC, YDEC, YINC, ZDEC, ZINC)

// TODO(@geome_try): to use copy-paste in production!
// vectorized, accuracy = 2, with divisions
#define GEO_PACKED_STENCIL_WIDE_USEDIV( \
                BULK, RHO, DTIME, DSPACE, NEXT, CUR, \
                XDEC, XINC, XDEC2, XINC2, \
                YDEC, YINC, YDEC2, YINC2, \
                ZDEC, ZINC, ZDEC2, ZINC2 \
            ) \
    GEO_PACKED_STENCIL_WIDE_USEINV( \
            BULK, _mm256_div_pd(_mm256_set1_pd(1.0), (RHO)), \
            DTIME, 1.0 / (DSPACE), NEXT, CUR, \
            XDEC, XINC, XDEC2, XINC2, \
            YDEC, YINC, YDEC2, YINC2, \
            ZDEC, ZINC, ZDEC2, ZINC2)
// TODO:
#define GEO_PACKED_STENCIL_TEST( \
        NEXT, CUR, XDEC, XINC, YDEC, YINC, ZDEC, ZINC) \
    do { \
        const __m256d cur = (CUR); \
        \
        __m256d x_dec = (XDEC); \
        __m256d x_inc = (XINC); \
        __m256d y_dec = (YDEC); \
        __m256d y_inc = (YINC); \
        /* ZDEC [x | xxx] cur */ \
        __m256d z_dec = _mm256_shuffle_pd(_mm256_permute2f128_pd( \
                    (ZDEC), cur, 0b00'10'00'01), cur, 0b0101); \
        /* cur [xxx | x] ZINC */ \
        __m256d z_inc = _mm256_shuffle_pd(cur, \
                _mm256_permute2f128_pd(cur, (ZINC), 0b00'10'00'01), 0b0101); \
        \
        (NEXT) = _mm256_mul_pd( \
                _mm256_add_pd( \
                    _mm256_add_pd( \
                        _mm256_add_pd(cur, (NEXT)), \
                        _mm256_add_pd(z_dec, z_inc) \
                        ), \
                    _mm256_add_pd( \
                        _mm256_add_pd(x_dec, x_inc), \
                        _mm256_add_pd(y_dec, y_inc) \
                        ) \
                    ), \
                _mm256_set1_pd(1.0 / 8.0) \
                ); \
    } while (false)

#endif // GEOACOUSTIC_STATIC_X86_PACKED_STENCIL_HPP_
