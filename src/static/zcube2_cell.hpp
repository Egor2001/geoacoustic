#ifndef GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_
#define GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_

#include <iostream>
#include <functional>

#include "types.hpp"
#include "config.hpp"
#include "volume_array.hpp"
#include "single_stencil.hpp"

namespace geo {

struct alignas(8 * sizeof(real_t)) ZCube2Cell
{
    static constexpr int_t NRankX = 1, NRankY = 1, NRankZ = 1;

    real_t arr[2 * 2 * 2]{};
};

void zcube2_cell_simplified_proc(int3_t idx3, 
        const SimplifiedConfig<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl);
void zcube2_cell_simplified_wide_proc(int3_t idx3, 
        const SimplifiedConfig<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl);
void zcube2_cell_proc(int3_t idx3, 
        const GeneralizedConfig<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl);
void zcube2_cell_wide_proc(int3_t idx3, 
        const GeneralizedConfig<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl);
void zcube2_cell_test_proc(int3_t idx3, const Config<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl);

void zcube2_cell_load(int3_t dim3, VolumeSpan<ZCube2Cell> span, 
                      std::istream& stream);
void zcube2_cell_store(int3_t dim3, VolumeConstSpan<ZCube2Cell> span, 
                       std::ostream& stream);

void zcube2_cell_fill(int3_t dim3, VolumeSpan<ZCube2Cell> span, 
                      std::function<real_t(int3_t, int3_t)> func);
void zcube2_cell_read(int3_t dim3, VolumeConstSpan<ZCube2Cell> span, 
                      std::function<void(int3_t, int3_t, real_t)> func);

inline __attribute__((always_inline)) 
void zcube2_cell_simplified_proc(int3_t idx3, 
        const SimplifiedConfig<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    GEO_SINGLE_STENCIL_FACTOR( \
                AT_(cfg.factor.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl_next, 0, 0, 0)   [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, 0)        [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, ((X) - 1), 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, (X), 0, 0)      [(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, ((Y) - 1), 0)[(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, (Y), 0)      [(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, 0, ((Z) - 1))[(X) + 2*(Y) + 4*(1 - (Z))], \
                AT_(ampl, 0, 0, (Z))      [(X) + 2*(Y) + 4*(1 - (Z))] \
            )

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
void zcube2_cell_simplified_wide_proc(int3_t idx3, 
        const SimplifiedConfig<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    GEO_SINGLE_STENCIL_WIDE_FACTOR( \
                AT_(cfg.factor.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl_next, 0, 0, 0)   [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, 0)        [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, ((X) - 1), 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, (X), 0, 0)      [(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, -1, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, +1, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, ((Y) - 1), 0)[(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, (Y), 0)      [(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, -1, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, +1, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, ((Z) - 1))[(X) + 2*(Y) + 4*(1 - (Z))], \
                AT_(ampl, 0, 0, (Z))      [(X) + 2*(Y) + 4*(1 - (Z))], \
                AT_(ampl, 0, 0, -1)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, +1)[(X) + 2*(Y) + 4*(Z)] \
            )

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
void zcube2_cell_proc(int3_t idx3, 
        const GeneralizedConfig<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    GEO_SINGLE_STENCIL_USEDIV( \
                AT_(cfg.bulk.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(cfg.rho.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                cfg.dtime, cfg.dspace, \
                AT_(ampl_next, 0, 0, 0)   [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, 0)        [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, ((X) - 1), 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, (X), 0, 0)      [(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, ((Y) - 1), 0)[(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, (Y), 0)      [(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, 0, ((Z) - 1))[(X) + 2*(Y) + 4*(1 - (Z))], \
                AT_(ampl, 0, 0, (Z))      [(X) + 2*(Y) + 4*(1 - (Z))] \
            )

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
void zcube2_cell_wide_proc(int3_t idx3, 
        const GeneralizedConfig<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    GEO_SINGLE_STENCIL_WIDE_USEDIV( \
                AT_(cfg.bulk.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(cfg.rho.span(), 0, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                cfg.dtime, cfg.dspace, \
                AT_(ampl_next, 0, 0, 0)   [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, 0)        [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, ((X) - 1), 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, (X), 0, 0)      [(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, -1, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, +1, 0, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, ((Y) - 1), 0)[(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, (Y), 0)      [(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, -1, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, +1, 0)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, ((Z) - 1))[(X) + 2*(Y) + 4*(1 - (Z))], \
                AT_(ampl, 0, 0, (Z))      [(X) + 2*(Y) + 4*(1 - (Z))], \
                AT_(ampl, 0, 0, -1)[(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, +1)[(X) + 2*(Y) + 4*(Z)] \
            )

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
void zcube2_cell_test_proc(int3_t idx3, const Config<ZCube2Cell>& cfg,
        VolumeSpan<ZCube2Cell> ampl_next, VolumeSpan<ZCube2Cell> ampl)
{
#define AT_(AMPL, X, Y, Z) \
    ((AMPL).at(cfg.grid_size, idx3 + int3_t{(X), (Y), (Z)})->arr)

#define PROC_STENCIL_(X, Y, Z) \
    GEO_SINGLE_STENCIL_TEST( \
                AT_(ampl_next, 0, 0, 0)   [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, 0, 0)        [(X) + 2*(Y) + 4*(Z)], \
                AT_(ampl, ((X) - 1), 0, 0)[(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, (X), 0, 0)      [(1 - (X)) + 2*(Y) + 4*(Z)], \
                AT_(ampl, 0, ((Y) - 1), 0)[(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, (Y), 0)      [(X) + 2*(1 - (Y)) + 4*(Z)], \
                AT_(ampl, 0, 0, ((Z) - 1))[(X) + 2*(Y) + 4*(1 - (Z))], \
                AT_(ampl, 0, 0, (Z))      [(X) + 2*(Y) + 4*(1 - (Z))] \
            )

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

void zcube2_cell_load(int3_t dim3, VolumeSpan<ZCube2Cell> span, 
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

void zcube2_cell_store(int3_t dim3, VolumeConstSpan<ZCube2Cell> span, 
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

void zcube2_cell_fill(int3_t dim3, VolumeSpan<ZCube2Cell> span, 
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

void zcube2_cell_read(int3_t dim3, VolumeConstSpan<ZCube2Cell> span, 
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

#endif // GEOACOUSTIC_STATIC_ZCUBE2_CELL_HPP_
