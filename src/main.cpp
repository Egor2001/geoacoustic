#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

// #define GEO_DEBUG
// #define GEO_BENCH

// #define GEO_GRID_LOOPED_DIAGONAL
// #define GEO_GRID_TILING_RECTANGULAR
// #define GEO_GRID_SINGLE_LAYER

#define GEO_SIMPLIFIED_CELL

#include "static/types.hpp"
#include "static/config.hpp"
#include "static/solver.hpp"

using namespace geo;

#if defined(GEO_BENCH)
// is used as fake computation result not to be optimized out
static volatile real_t g_result = 0.0;
#endif // defined(GEO_BENCH)

struct HaarWavelet
{
    int3_t mid;
    real_t rad;
    real_t val;

    real_t operator()(int3_t, int3_t idx3)
    {
        int3_t rv = idx3 - mid;
        real_t r2 = static_cast<real_t>(rv.x*rv.x + rv.y*rv.y + rv.z*rv.z);

        return ((r2 < rad * rad) ? val : 0.0);
    }
};

struct MHatWavelet
{
    int3_t mid;
    real_t rad;
    real_t val;

    real_t operator()(int3_t, int3_t idx3)
    {
        int3_t rv = idx3 - mid;
        real_t r2 = static_cast<real_t>(rv.x*rv.x + rv.y*rv.y + rv.z*rv.z);

        return val * (rad * rad - r2) * std::exp(-0.5 * r2);
    }
};

int main(int argc, char* argv[])
{
    using TCell = ZC4VecCell;
    static constexpr int_t NTileRank = 3;

    if (argc != 3)
    {
        std::cout << "USAGE: " << argv[0] << " CELLS_CNT STEPS_CNT\n";
        return 1;
    }

    auto usr_cells = strtoul(argv[1], nullptr, 0);
    auto usr_steps = strtoul(argv[2], nullptr, 0);

    static constexpr unsigned long NTileMask = 
        ((1 << (NTileRank + TCell::NRankZ)) - 1);

    int3_t grid_size = {
        static_cast<int_t>(usr_cells & ~NTileMask), 
        static_cast<int_t>(usr_cells & ~NTileMask), 
        static_cast<int_t>(usr_cells & ~NTileMask)
    };
    int_t steps_cnt = static_cast<int_t>(usr_steps & ~NTileMask);

    Config<TCell> cfg(grid_size, steps_cnt);

    cfg.dspace = 3.0; 
    cfg.dtime = 0.001; 

    real_t bulk = 1000.0, rho = 0.001;

    int3_t mid = {grid_size.x / 2, grid_size.y / 2, grid_size.z / 2};

    Solver<TCell, NTileRank> solver(cfg);
    
    solver.fill_config(
            [bulk](int3_t, int3_t) { return bulk; },
            [rho](int3_t, int3_t) { return rho; }
            );
    // solver.fill_bulk([bulk](int3_t, int3_t) { return bulk; });
    // solver.fill_rho([rho](int3_t, int3_t) { return rho; });
    solver.fill_init(HaarWavelet{ mid, /* rad = */ 3.0, /* val = */ 1.0 });
    // solver.fill_init(MHatWavelet{ mid, /* rad = */ 5.0, /* val = */ 1.0 });

    solver.proc();

#if defined(GEO_BENCH)
    solver.read_result([](int3_t, int3_t, real_t val) { g_result += val; });
#else
    int_t zcoord = grid_size.z / 2;
    std::vector<real_t> zlayer(grid_size.x * grid_size.y);

    solver.read_result([grid_size, zcoord, &zlayer]
        (int3_t, int3_t idx3, real_t val)
        {
            if (idx3.z == zcoord)
                zlayer[idx3.x + idx3.y * grid_size.x] = val;
        }
    );

    for (int_t y = 0; y < grid_size.y; ++y)
    {
        for (int_t x = 0; x < grid_size.x; ++x)
            std::cout << zlayer[x + y * grid_size.y] << " ";

        std::cout << '\n';
    }
#endif // defined(GEO_BENCH)
    return 0;
}

