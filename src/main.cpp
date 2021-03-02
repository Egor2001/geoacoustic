#include <iostream>

#include "types.hpp"
#include "solver.hpp"
#include "stencil.hpp"

using namespace geo;

int main()
{
    Solver solver(/* dim3 = */ {2, 2, 2}, /* rank = */ 0, Stencil{});

    solver.domain().load(std::cin);

    int_t time_ticks = 10;
    solver.process(time_ticks);

    solver.domain().store(std::cout);

    return 0;
}
