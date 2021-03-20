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

    int3_t grid_size = {2, 2, 2};
    int_t steps_cnt = 100;

    Config<TCell> cfg(grid_size, steps_cnt);
    CellLayout<TCell>::load(cfg.grid_size, cfg.bulk.span(), std::cin);
    CellLayout<TCell>::load(cfg.grid_size, cfg.rho.span(), std::cin);

    cfg.dspace = 3.0; 
    cfg.dtime = 0.001; 

    Solver<TCell, NTileRank> solver(cfg);
    solver.load_ctx(std::cin);

    solver.proc();

    CellLayout<TCell>::store(solver.get_cfg().grid_size, 
            solver.get_ctx().ampl_next.span(), std::cout);
    std::cout << std::endl;

    return 0;
}
