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
    static constexpr int_t NTileRank = 4;

    int3_t grid_size = {128, 128, 128};
    int_t steps_cnt = 128;

    Config<TCell> cfg(grid_size, steps_cnt);

    cfg.dspace = 3.0; 
    cfg.dtime = 0.001; 

    real_t bulk = 1000.0, rho = 1.0;

    int3_t init_idx3 = {grid_size.x / 2, grid_size.y / 2, grid_size.z / 2};
    real_t init_rad = 5.0, init_val = 1.0;

    Solver<TCell, NTileRank> solver(cfg);

    solver.fill_bulk([bulk](int3_t, int3_t) { return bulk; });
    solver.fill_rho([rho](int3_t, int3_t) { return rho; });
    solver.fill_init([init_idx3, init_rad, init_val]
        (int3_t/* grid_size */, int3_t idx3) -> real_t
        {
            int3_t rv = idx3 - init_idx3;
            real_t r2 = static_cast<real_t>(rv.x*rv.x + rv.y*rv.y + rv.z*rv.z);

            return (r2 < init_rad ? init_val : 0.0);
        }
    );

    solver.proc();

    // solver.read_ctx([](int3_t grid_size, int3_t idx3, real_t ampl) {});
    solver.store_result(std::cout);
    std::cout << std::endl;

    return 0;
}

