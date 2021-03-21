#include <iostream>
#include <fstream>

// #define GEO_DEBUG
#include "static/types.hpp"
#include "static/config.hpp"
#include "static/solver.hpp"

using namespace geo;

int main()
{
    using TCell = ZCube2Cell;
    static constexpr int_t NTileRank = 0;

    int3_t grid_size = {32, 32, 32};
    int_t steps_cnt = 8;

    Config<TCell> cfg(grid_size, steps_cnt);

    cfg.dspace = 3.0; 
    cfg.dtime = 0.6; 

    real_t bulk = 3000.0, rho = 1.0;

    int3_t init_idx3 = {grid_size.x / 2, grid_size.y / 2, grid_size.z / 2};
    real_t init_rad = 5.0, init_val = 10.0;

    Solver<TCell, NTileRank> solver(cfg);

    solver.fill_bulk([bulk](int3_t, int3_t) { return bulk; });
    solver.fill_rho([rho](int3_t, int3_t) { return rho; });
    solver.fill_init([init_idx3, init_rad, init_val]
        (int3_t, int3_t idx3) -> real_t
        {
            int3_t rv = idx3 - init_idx3;
            real_t r2 = static_cast<real_t>(rv.x*rv.x + rv.y*rv.y + rv.z*rv.z);

            return (r2 < init_rad ? (init_rad - r2) * init_val : 0.0);
        }
    );

    solver.proc();

    // solver.read_ctx([](int3_t grid_size, int3_t idx3, real_t ampl) {});
    // solver.store_result(std::cout);
    std::cout << std::endl;

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

    return 0;
}

