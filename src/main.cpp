#include <iostream>

/*
#include "dumb/types.hpp"
#include "dumb/solver.hpp"
#include "dumb/stencil.hpp"
*/
#include "static/types.hpp"
#include "static/solver.hpp"
#include "static/stencil.hpp"

using namespace geo;
/*
void test_dumb()
{
    Solver solver({2, 2, 2}, 0, Stencil{});

    solver.domain().load(std::cin);

    int_t time_ticks = 10;
    solver.process(time_ticks);

    solver.domain().store(std::cout);
}
*/

void test_static()
{
    Config cfg{};
    cfg.dim3 = {2, 2, 2};
    cfg.pml3 = {0, 0, 0};

    Stencil stencil{};

    Solver</* rank = */ 0> solver(cfg, stencil);

    for (auto& cell : solver.domain())
        std::cin >> cell;

    int_t ticks_cnt = 10;
    solver.process(ticks_cnt);

    for (auto& cell : solver.domain())
        std::cout << cell;
}

int main()
{
    test_static();

    return 0;
}
