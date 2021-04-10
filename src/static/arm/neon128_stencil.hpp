#ifndef GEOACOUSTIC_STATIC_ARM_PACKED_STENCIL_HPP_
#define GEOACOUSTIC_STATIC_ARM_PACKED_STENCIL_HPP_

#include <arm_neon.h>

#include "types.hpp"

namespace geo {

// finite difference coefficients, accuracy = 1
const float64x2_t kFDC1_1v = vdupq_n_f64(-2.0);
const float64x2_t kFDC1_2v = vdupq_n_f64(1.0);

// finite difference coefficients, accuracy = 2
const float64x2_t kFDC2_1v = vdupq_n_f64(-5.0 / 2.0);
const float64x2_t kFDC2_2v = vdupq_n_f64(4.0 / 3.0);
const float64x2_t kFDC2_3v = vdupq_n_f64(-1.0 / 12.0);

} // namespace geo

// vectorized, accuracy = 1, using pre-computed isotropic factor
#define GEO_PACKED_STENCIL_FACTOR(FACTOR, NEXT, CUR, \
                                  XDEC, XINC, YDEC, YINC, ZDEC, ZINC) \
    do { \
        const float64x2_t cur = (CUR); \
        /* ZDEC [x | x] cur */ \
        float64x2_t z_dec = vextq_f64((ZDEC), cur, 1); \
        /* cur [x | x] ZINC */ \
        float64x2_t z_inc = vextq_f64(cur, (ZINC), 1); \
        \
        (NEXT) = \
            vaddq_f64( \
                vsubq_f64( \
                    vmulq_f64(vdupq_n_f64(2.0), cur), \
                    (NEXT) \
                    ), \
                vmulq_f64( \
                    (FACTOR), \
                    vaddq_f64( \
                        vmulq_f64( \
                            vmulq_f64(vdupq_n_f64(3.0), kFDC1_1v), \
                            cur \
                            ), \
                        vmulq_f64( \
                            kFDC1_2v, \
                            vaddq_f64( \
                                vaddq_f64( \
                                    vaddq_f64((XDEC), (XINC)),  \
                                    vaddq_f64((YDEC), (YINC)) \
                                    ), \
                                vaddq_f64(z_dec, z_inc) \
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
        const float64x2_t cur = (CUR); \
        /* ZDEC [x | x] cur */ \
        float64x2_t z_dec = vextq_f64((ZDEC), cur, 1); \
        /* ZDEC [xx | ] cur */ \
        float64x2_t z_dec2 = (ZDEC); \
        /* cur [x | x] ZINC */ \
        float64x2_t z_inc = vextq_f64(cur, (ZINC), 1); \
        /* cur [ | xx] ZINC */ \
        float64x2_t z_inc2 = (ZINC); \
        \
        (NEXT) = \
            vaddq_f64( \
                vsubq_f64( \
                    vmulq_f64(vdupq_n_f64(2.0), cur), \
                    (NEXT) \
                    ), \
                vmulq_f64( \
                    (FACTOR), \
                    vaddq_f64( \
                        vmulq_f64( \
                            vmulq_f64(vdupq_n_f64(3.0), kFDC2_1v), \
                            cur \
                            ), \
                        vaddq_f64( \
                            vmulq_f64( \
                                kFDC2_2v, \
                                vaddq_f64( \
                                    vaddq_f64( \
                                        vaddq_f64((XDEC), (XINC)), \
                                        vaddq_f64((YDEC), (YINC)) \
                                        ), \
                                    vaddq_f64(z_dec, z_inc) \
                                    ) \
                                ), \
                            vmulq_f64( \
                                kFDC2_3v, \
                                vaddq_f64( \
                                    vaddq_f64( \
                                        vaddq_f64((XDEC2), (XINC2)), \
                                        vaddq_f64((YDEC2), (YINC2)) \
                                        ), \
                                    vaddq_f64(z_dec2, z_inc2) \
                                    ) \
                                ) \
                            ) \
                        ) \
                    ) \
                ); \
    } while (false)

// vectorized, accuracy = 1, with mul-by-inv
#define GEO_PACKED_STENCIL_USEINV( \
                BULK, INV_RHO, DTIME, INV_DSPACE, NEXT, CUR, \
                XDEC, XINC, YDEC, YINC, ZDEC, ZINC \
            ) \
    do { \
        const float64x2_t speed_sqr = vmulq_f64((BULK), (INV_RHO)); \
        const real_t ratio = (DTIME) * (INV_DSPACE); \
        const float64x2_t factor = \
            vmulq_f64(speed_sqr, vdupq_n_f64(ratio * ratio)); \
        \
        const float64x2_t cur = (CUR); \
        \
        float64x2_t x_dec = (XDEC); \
        float64x2_t x_inc = (XINC); \
        float64x2_t y_dec = (YDEC); \
        float64x2_t y_inc = (YINC); \
        /* ZDEC [x | x] cur */ \
        float64x2_t z_dec = vextq_f64((ZDEC), cur, 1); \
        /* cur [x | x] ZINC */ \
        float64x2_t z_inc = vextq_f64(cur, (ZINC), 1); \
        \
        float64x2_t u_dx = vaddq_f64(vmulq_f64(kFDC1_1v, cur), \
                vmulq_f64(kFDC1_2v, vaddq_f64(x_inc, x_dec))); \
        \
        float64x2_t u_dy = vaddq_f64(vmulq_f64(kFDC1_1v, cur), \
                vmulq_f64(kFDC1_2v, vaddq_f64(y_inc, y_dec))); \
        \
        float64x2_t u_dz = vaddq_f64(vmulq_f64(kFDC1_1v, cur), \
                vmulq_f64(kFDC1_2v, vaddq_f64(z_inc, z_dec))); \
        \
        (NEXT) = vaddq_f64( \
                vsubq_f64(vmulq_f64( \
                    vdupq_n_f64(2.0), cur), (NEXT) \
                    ), \
                vmulq_f64( \
                    factor, vaddq_f64(vaddq_f64(u_dx, u_dy), u_dz) \
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
        const float64x2_t speed_sqr = vmulq_f64((BULK), (INV_RHO)); \
        const real_t ratio = (DTIME) * (INV_DSPACE); \
        const float64x2_t factor = \
            vmulq_f64(speed_sqr, vdupq_n_f64(ratio * ratio)); \
        \
        const float64x2_t cur = (CUR); \
        \
        float64x2_t x_dec = (XDEC), x_dec2 = (XDEC2); \
        float64x2_t x_inc = (XINC), x_inc2 = (XINC2); \
        float64x2_t y_dec = (YDEC), y_dec2 = (YDEC2); \
        float64x2_t y_inc = (YINC), y_inc2 = (YINC2); \
        /* ZDEC [x | x] cur */ \
        float64x2_t z_dec = vextq_f64((ZDEC), cur, 1); \
        /* ZDEC [xx | ] cur */ \
        float64x2_t z_dec2 = (ZDEC); \
        /* cur [x | x] ZINC */ \
        float64x2_t z_inc = vextq_f64(cur, (ZINC), 1); \
        /* cur [ | xx] ZINC */ \
        float64x2_t z_inc2 = (ZINC); \
        \
        float64x2_t u_dx = vaddq_f64(vmulq_f64(kFDC2_1v, cur), \
                vaddq_f64(\
                    vmulq_f64(kFDC2_2v, vaddq_f64(x_inc, x_dec)), \
                    vmulq_f64(kFDC2_3v, vaddq_f64(x_inc2, x_dec2)) \
                    )); \
        float64x2_t u_dy = vaddq_f64(vmulq_f64(kFDC2_1v, cur), \
                vaddq_f64(\
                    vmulq_f64(kFDC2_2v, vaddq_f64(y_inc, y_dec)), \
                    vmulq_f64(kFDC2_3v, vaddq_f64(y_inc2, y_dec2)) \
                    )); \
        float64x2_t u_dz = vaddq_f64(vmulq_f64(kFDC2_1v, cur), \
                vaddq_f64(\
                    vmulq_f64(kFDC2_2v, vaddq_f64(z_inc, z_dec)), \
                    vmulq_f64(kFDC2_3v, vaddq_f64(z_inc2, z_dec2)) \
                    )); \
        \
        (NEXT) = vaddq_f64( \
                vsubq_f64(vmulq_f64( \
                    vdupq_n_f64(2.0), cur), (NEXT) \
                    ), \
                vmulq_f64( \
                    factor, vaddq_f64(vaddq_f64(u_dx, u_dy), u_dz) \
                    ) \
                ); \
    } while (false)

// TODO(@geome_try): to use copy-paste in production!
// vectorized, accuracy = 1, with divisions
#define GEO_PACKED_STENCIL_USEDIV( \
                BULK, RHO, DTIME, DSPACE, NEXT, CUR, \
                XDEC, XINC, YDEC, YINC, ZDEC, ZINC \
            ) \
    GEO_PACKED_STENCIL_USEINV( \
            BULK, vdivq_f64(vdupq_n_f64(1.0), (RHO)), \
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
            BULK, vdivq_f64(vdupq_n_f64(1.0), (RHO)), \
            DTIME, 1.0 / (DSPACE), NEXT, CUR, \
            XDEC, XINC, XDEC2, XINC2, \
            YDEC, YINC, YDEC2, YINC2, \
            ZDEC, ZINC, ZDEC2, ZINC2)

// TODO:
#define GEO_PACKED_STENCIL_TEST( \
        NEXT, CUR, XDEC, XINC, YDEC, YINC, ZDEC, ZINC) \
    do { \
        const float64x2_t cur = (CUR); \
        \
        float64x2_t x_dec = (XDEC); \
        float64x2_t x_inc = (XINC); \
        float64x2_t y_dec = (YDEC); \
        float64x2_t y_inc = (YINC); \
        /* ZDEC [x | x] cur */ \
        float64x2_t z_dec = vextq_f64((ZDEC), cur, 1); \
        /* cur [x | x] ZINC */ \
        float64x2_t z_inc = vextq_f64(cur, (ZINC), 1); \
        \
        (NEXT) = vmulq_f64( \
                vaddq_f64( \
                    vaddq_f64( \
                        vaddq_f64(cur, (NEXT)), \
                        vaddq_f64(z_dec, z_inc) \
                        ), \
                    vaddq_f64( \
                        vaddq_f64(x_dec, x_inc), \
                        vaddq_f64(y_dec, y_inc) \
                        ) \
                    ), \
                vdupq_n_f64(1.0 / 8.0) \
                ); \
    } while (false)

#endif // GEOACOUSTIC_STATIC_ARM_PACKED_STENCIL_HPP_
