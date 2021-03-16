#ifndef GEOACOUSTIC_SOLVER_HPP_
#define GEOACOUSTIC_SOLVER_HPP_

#include <cassert>

#include "types.hpp"
#include "domain.hpp"
#include "tiling.hpp"
#include "stencil.hpp"

namespace geo {

class Solver
{
public:
    // TODO(@geome_try): to add context
    Solver(int3_t dim3, int_t cubes_rank, Stencil stencil):
        /* context */
        domains_{{ Domain(dim3, cubes_rank), Domain(dim3, cubes_rank) }},
        tiling_(dim3),
        grid_{tiling_.grid()},
        stencil_{std::move(stencil)}
    {
        assert(dim3.x > 0 && dim3.y > 0 && dim3.z > 0);
        assert(cubes_rank >= 0);
    }

    // TODO(@geome_try): to add const version
    Domain& domain()
    {
        return domains_[0];
    }

    void process(int_t time_ticks)
    {
        grid_.process(domains_[0], domains_[1], stencil_, time_ticks);

        if (time_ticks %2 == 1)
            std::swap(domains_[0], domains_[1]);
    }

private:
    std::array<Domain, 2> domains_;
    Tiling tiling_;
    Grid<Tiling> grid_;
    Stencil stencil_;
};

} // namespace geo

#endif // GEOACOUSTIC_SOLVER_HPP_
