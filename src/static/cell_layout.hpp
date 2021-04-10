#ifndef GEOACOUSTIC_STATIC_CELL_LAYOUT_HPP_
#define GEOACOUSTIC_STATIC_CELL_LAYOUT_HPP_

#include <iostream>

#include "types.hpp"
#include "volume_array.hpp"
#include "config.hpp"
#include "linear_cell.hpp"
#include "zcube2_cell.hpp"
#include "zcube4_cell.hpp"
#include "vector_cell.hpp"
#include "zc2vec_cell.hpp"
#include "zc4vec_cell.hpp"

namespace geo {

template<typename TC>
struct CellLayout;

template<>
struct CellLayout<LinearCell>
{
    using TCell = LinearCell;
    static constexpr int_t NRankX = TCell::NRankX;
    static constexpr int_t NRankY = TCell::NRankY;
    static constexpr int_t NRankZ = TCell::NRankZ;

    inline __attribute__((always_inline))
    static void cell_proc(int3_t idx3, const Config<TCell>& cfg,
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
#if defined(GEO_SIMPLIFIED_CELL)
        linear_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
#else
        linear_cell_proc(idx3, cfg, ampl_next, ampl);
#endif
    }
/*
    inline __attribute__((always_inline))
    static void cell_simplified_proc(int3_t idx3, 
            const SimplifiedConfig<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        linear_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
    }
*/
    static void load(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::istream& stream)
    {
        linear_cell_load(grid_size, span, stream);
    }

    static void store(int3_t grid_size, VolumeConstSpan<TCell> span, 
                      std::ostream& stream)
    {
        linear_cell_store(grid_size, span, stream);
    }

    static void fill(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::function<real_t(int3_t, int3_t)> func)
    {
        linear_cell_fill(grid_size, span, std::move(func));
    }

    static void read(int3_t grid_size, VolumeConstSpan<TCell> span, 
                     std::function<void(int3_t, int3_t, real_t)> func)
    {
        linear_cell_read(grid_size, span, std::move(func));
    }
};

template<>
struct CellLayout<ZCube2Cell>
{
    using TCell = ZCube2Cell;
    static constexpr int_t NRankX = TCell::NRankX;
    static constexpr int_t NRankY = TCell::NRankY;
    static constexpr int_t NRankZ = TCell::NRankZ;

    inline __attribute__((always_inline))
    static void cell_proc(int3_t idx3, const Config<TCell>& cfg,
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
#if defined(GEO_SIMPLIFIED_CELL)
        zcube2_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
#else
        zcube2_cell_proc(idx3, cfg, ampl_next, ampl);
#endif
    }
/*
    inline __attribute__((always_inline))
    static void cell_simplified_proc(int3_t idx3, 
            const SimplifiedConfig<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        zcube2_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
    }
*/
    static void load(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::istream& stream)
    {
        zcube2_cell_load(grid_size, span, stream);
    }

    static void store(int3_t grid_size, VolumeConstSpan<TCell> span, 
                      std::ostream& stream)
    {
        zcube2_cell_store(grid_size, span, stream);
    }

    static void fill(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::function<real_t(int3_t, int3_t)> func)
    {
        zcube2_cell_fill(grid_size, span, std::move(func));
    }

    static void read(int3_t grid_size, VolumeConstSpan<TCell> span, 
                     std::function<void(int3_t, int3_t, real_t)> func)
    {
        zcube2_cell_read(grid_size, span, std::move(func));
    }
};

template<>
struct CellLayout<ZCube4Cell>
{
    using TCell = ZCube4Cell;
    static constexpr int_t NRankX = TCell::NRankX;
    static constexpr int_t NRankY = TCell::NRankY;
    static constexpr int_t NRankZ = TCell::NRankZ;

    inline __attribute__((always_inline))
    static void cell_proc(int3_t idx3, const Config<TCell>& cfg,
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
#if defined(GEO_SIMPLIFIED_CELL)
        zcube4_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
#else
        zcube4_cell_proc(idx3, cfg, ampl_next, ampl);
#endif
    }
/*
    inline __attribute__((always_inline))
    static void cell_simplified_proc(int3_t idx3, 
            const SimplifiedConfig<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        zcube4_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
    }
*/
    static void load(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::istream& stream)
    {
        zcube4_cell_load(grid_size, span, stream);
    }

    static void store(int3_t grid_size, VolumeConstSpan<TCell> span, 
                      std::ostream& stream)
    {
        zcube4_cell_store(grid_size, span, stream);
    }

    static void fill(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::function<real_t(int3_t, int3_t)> func)
    {
        zcube4_cell_fill(grid_size, span, std::move(func));
    }

    static void read(int3_t grid_size, VolumeConstSpan<TCell> span, 
                     std::function<void(int3_t, int3_t, real_t)> func)
    {
        zcube4_cell_read(grid_size, span, std::move(func));
    }
};

template<>
struct CellLayout<VectorCell>
{
    using TCell = VectorCell;
    static constexpr int_t NRankX = TCell::NRankX;
    static constexpr int_t NRankY = TCell::NRankY;
    static constexpr int_t NRankZ = TCell::NRankZ;

    inline __attribute__((always_inline))
    static void cell_proc(int3_t idx3, const Config<TCell>& cfg,
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
#if defined(GEO_SIMPLIFIED_CELL)
        vector_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
#else
        vector_cell_proc(idx3, cfg, ampl_next, ampl);
#endif
    }
/*
    inline __attribute__((always_inline))
    static void cell_simplified_proc(int3_t idx3, 
            const SimplifiedConfig<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        vector_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
    }
*/
    static void load(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::istream& stream)
    {
        vector_cell_load(grid_size, span, stream);
    }

    static void store(int3_t grid_size, VolumeConstSpan<TCell> span, 
                      std::ostream& stream)
    {
        vector_cell_store(grid_size, span, stream);
    }

    static void fill(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::function<real_t(int3_t, int3_t)> func)
    {
        vector_cell_fill(grid_size, span, std::move(func));
    }

    static void read(int3_t grid_size, VolumeConstSpan<TCell> span, 
                     std::function<void(int3_t, int3_t, real_t)> func)
    {
        vector_cell_read(grid_size, span, std::move(func));
    }
};

template<>
struct CellLayout<ZC2VecCell>
{
    using TCell = ZC2VecCell;
    static constexpr int_t NRankX = TCell::NRankX;
    static constexpr int_t NRankY = TCell::NRankY;
    static constexpr int_t NRankZ = TCell::NRankZ;

    inline __attribute__((always_inline))
    static void cell_proc(int3_t idx3, const Config<TCell>& cfg,
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
#if defined(GEO_SIMPLIFIED_CELL)
        zc2vec_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
#else
        zc2vec_cell_proc(idx3, cfg, ampl_next, ampl);
#endif
    }
/*
    inline __attribute__((always_inline))
    static void cell_simplified_proc(int3_t idx3, 
            const SimplifiedConfig<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        zc2vec_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
    }
*/
    static void load(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::istream& stream)
    {
        zc2vec_cell_load(grid_size, span, stream);
    }

    static void store(int3_t grid_size, VolumeConstSpan<TCell> span, 
                      std::ostream& stream)
    {
        zc2vec_cell_store(grid_size, span, stream);
    }

    static void fill(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::function<real_t(int3_t, int3_t)> func)
    {
        zc2vec_cell_fill(grid_size, span, std::move(func));
    }

    static void read(int3_t grid_size, VolumeConstSpan<TCell> span, 
                     std::function<void(int3_t, int3_t, real_t)> func)
    {
        zc2vec_cell_read(grid_size, span, std::move(func));
    }
};

template<>
struct CellLayout<ZC4VecCell>
{
    using TCell = ZC4VecCell;
    static constexpr int_t NRankX = TCell::NRankX;
    static constexpr int_t NRankY = TCell::NRankY;
    static constexpr int_t NRankZ = TCell::NRankZ;

    inline __attribute__((always_inline))
    static void cell_proc(int3_t idx3, const Config<TCell>& cfg,
        VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
#if defined(GEO_SIMPLIFIED_CELL)
        zc4vec_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
#else
        zc4vec_cell_proc(idx3, cfg, ampl_next, ampl);
#endif
    }
/*
    inline __attribute__((always_inline))
    static void cell_simplified_proc(int3_t idx3, 
            const SimplifiedConfig<TCell>& cfg, 
            VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
    {
        zc4vec_cell_simplified_proc(idx3, cfg, ampl_next, ampl);
    }
*/
    static void load(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::istream& stream)
    {
        zc4vec_cell_load(grid_size, span, stream);
    }

    static void store(int3_t grid_size, VolumeConstSpan<TCell> span, 
                      std::ostream& stream)
    {
        zc4vec_cell_store(grid_size, span, stream);
    }

    static void fill(int3_t grid_size, VolumeSpan<TCell> span, 
                     std::function<real_t(int3_t, int3_t)> func)
    {
        zc4vec_cell_fill(grid_size, span, std::move(func));
    }

    static void read(int3_t grid_size, VolumeConstSpan<TCell> span, 
                     std::function<void(int3_t, int3_t, real_t)> func)
    {
        zc4vec_cell_read(grid_size, span, std::move(func));
    }
};

} // namespace geo

#endif // GEOACOUSTIC_STATIC_CELL_LAYOUT_HPP_
