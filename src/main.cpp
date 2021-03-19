#include <iostream>
#include <fstream>

#include "static/types.hpp"
#include "static/config.hpp"
#include "static/solver.hpp"

using namespace geo;

int main()
{
    using TCell = LinearCell;
    static constexpr int_t NTileRank = 0;

    int3_t grid_size = {2, 2, 2};
    int_t steps_cnt = 10;

    Config<TCell> cfg(grid_size, steps_cnt);

    cfg.dspace = 0.1; 
    cfg.dtime = 0.1; 

    Solver<TCell, NTileRank> solver(cfg, std::cin, std::cin);

    solver.proc(std::cout);

    return 0;
}
