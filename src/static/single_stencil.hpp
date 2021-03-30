#ifndef GEOACOUSTIC_STATIC_SINGLE_STENCIL_HPP_
#define GEOACOUSTIC_STATIC_SINGLE_STENCIL_HPP_

#include "types.hpp"

namespace geo {

// finite difference coefficients, accuracy = 1
const real_t kFDC1_1s = -5.0 / 2.0;
const real_t kFDC1_2s = 4.0 / 3.0;

// finite difference coefficients, accuracy = 2
const real_t kFDC2_1s = -5.0 / 2.0;
const real_t kFDC2_2s = 4.0 / 3.0;
const real_t kFDC2_3s = -1.0 / 12.0;

} // namespace geo

// not vectorized, accuracy = 1, with mul-by-inv
#define GEO_SINGLE_STENCIL_USEINV(BULK, INV_RHO, DTIME, INV_DSPACE, NEXT, CUR, \
                                  XDEC, XINC, YDEC, YINC, ZDEC, ZINC) \
    do { \
        const real_t speed_sqr = (BULK) * (INV_RHO); \
        const real_t ratio = (DTIME) * (INV_DSPACE); \
        const real_t factor = speed_sqr * ratio * ratio; \
        \
        const real_t cur = (CUR); \
        real_t u_dx = kFDC1_1s * cur + kFDC1_2s * ((XDEC) + (XINC)); \
        real_t u_dy = kFDC1_1s * cur + kFDC1_2s * ((YDEC) + (YINC)); \
        real_t u_dz = kFDC1_1s * cur + kFDC1_2s * ((ZDEC) + (ZINC)); \
        \
        (NEXT) = 2.0 * cur - (NEXT) + factor * (u_dx + u_dy + u_dz); \
    } while (false)

// not vectorized, accuracy = 2, with mul-by-inv
#define GEO_SINGLE_STENCIL_WIDE_USEINV( \
                BULK, INV_RHO, DTIME, INV_DSPACE, NEXT, CUR, \
                XDEC, XINC, XDEC2, XINC2, \
                YDEC, YINC, YDEC2, YINC2, \
                ZDEC, ZINC, ZDEC2, ZINC2 \
            ) \
    do { \
        const real_t speed_sqr = (BULK) * (INV_RHO); \
        const real_t ratio = (DTIME) * (INV_DSPACE); \
        const real_t factor = speed_sqr * ratio * ratio; \
        \
        const real_t cur = (CUR); \
        real_t u_dx = kFDC2_1s * cur + \
            kFDC2_2s * ((XDEC) + (XINC)) + kFDC2_3s * ((XDEC2) + (XINC2)); \
        real_t u_dy = kFDC2_1s * cur + \
            kFDC2_2s * ((YDEC) + (YINC)) + kFDC2_3s * ((YDEC2) + (YINC2)); \
        real_t u_dz = kFDC2_1s * cur + \
            kFDC2_2s * ((ZDEC) + (ZINC)) + kFDC2_3s * ((ZDEC2) + (ZINC2)); \
        \
        (NEXT) = 2.0 * cur - (NEXT) + factor * (u_dx + u_dy + u_dz); \
    } while (false)

// TODO(@geome_try): to use copy-paste in production
// not vectorized, accuracy = 1, with divisions
#define GEO_SINGLE_STENCIL_USEDIV(BULK, RHO, DTIME, DSPACE, NEXT, CUR, \
                                  XDEC, XINC, YDEC, YINC, ZDEC, ZINC) \
    GEO_SINGLE_STENCIL_USEINV( \
            BULK, 1.0 / (RHO), DTIME, 1.0 / (DSPACE), NEXT, CUR, \
            XDEC, XINC, YDEC, YINC, ZDEC, ZINC)

// TODO(@geome_try): to use copy-paste in production
// not vectorized, accuracy = 2, with divisions
#define GEO_SINGLE_STENCIL_WIDE_USEDIV( \
                BULK, RHO, DTIME, DSPACE, NEXT, CUR, \
                XDEC, XINC, XDEC2, XINC2, \
                YDEC, YINC, YDEC2, YINC2, \
                ZDEC, ZINC, ZDEC2, ZINC2 \
            ) \
    GEO_SINGLE_STENCIL_WIDE_USEINV( \
            BULK, 1.0 / (RHO), DTIME, 1.0 / (DSPACE), NEXT, CUR, \
            XDEC, XINC, XDEC2, XINC2, \
            YDEC, YINC, YDEC2, YINC2, \
            ZDEC, ZINC, ZDEC2, ZINC2)

#endif // GEOACOUSTIC_STATIC_SINGLE_STENCIL_HPP_
