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
        domains_{},
        tiling_(std::move(stencil))
    {
        assert(dim3.x > 0 && dim3.y > 0 && dim3.z > 0);
        assert(cubes_rank >= 0);

        domains_[0] = Domain(dim3, cubes_rank);
        domains_[1] = Domain(dim3, cubes_rank);
    }

    // TODO(@geome_try): to add const version
    Domain& domain()
    {
        return domains_[0];
    }

    void apply(int_t time_ticks)
    {
        for (int_t step = 0; step < time_ticks; ++step)
        {
            tiling.apply(&domains_[0], &domains_[1]);
            std::swap(domains_[0], domains_[1]);
        }
    }

private:
    std::array<Domain, 2> domains_;
    Tiling tiling_;
};

} // namespace geo

#endif // GEOACOUSTIC_SOLVER_HPP_
