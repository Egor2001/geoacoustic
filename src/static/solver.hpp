#ifndef GEOACOUSTIC_SOLVER_HPP_
#define GEOACOUSTIC_SOLVER_HPP_

#include <cassert>

#include "types.hpp"
#include "tiling.hpp"
#include "stencil.hpp"
#include "general.hpp"

namespace geo {

template<int_t NCubeRank>
class Solver
{
public:
    static_assert(NCubeRank >= 0);
    static constexpr int_t NCubeSide = 1 << NCubeRank;

    // TODO(@geome_try): to add context
    Solver(Config config, Stencil stencil):
        stencil_{std::move(stencil)},
        config_{std::move(config)},
        domains_{{ 
            std::vector<Cell>(config_.dim3.x + config_.dim3.y + config_.dim3.z),
            std::vector<Cell>(config_.dim3.x + config_.dim3.y + config_.dim3.z) 
        }},
        tiles_{create_grid<NCubeRank>(config_.dim3)}
    {
        assert(config_.dim3.x > 0 && config_.dim3.x % NCubeSide == 0 && 
               config_.dim3.y > 0 && config_.dim3.y % NCubeSide == 0 && 
               config_.dim3.z > 0 && config_.dim3.z % NCubeSide == 0);

        config_.tiles = tiles_.data();
        config_.tiles_cnt = tiles_.size();

        gh_x0_.resize(config_.dim3.y * config_.dim3.z);
        gh_x1_.resize(config_.dim3.y * config_.dim3.z);

        gh_y0_.resize(config_.dim3.x * config_.dim3.z);
        gh_y1_.resize(config_.dim3.x * config_.dim3.z);

        gh_z0_.resize(config_.dim3.x * config_.dim3.y);
        gh_z1_.resize(config_.dim3.x * config_.dim3.y);

        config_.gh_x0_data = gh_x0_.data(); config_.gh_x1_data = gh_x1_.data();
        config_.gh_y0_data = gh_y0_.data(); config_.gh_y1_data = gh_y1_.data();
        config_.gh_z0_data = gh_z0_.data(); config_.gh_z1_data = gh_z1_.data();
    }

    // TODO(@geome_try): to add const version
    std::vector<Cell>& domain()
    {
        return domains_[0];
    }

    void process(int_t ticks_cnt)
    {
        config_.ticks_cnt = ticks_cnt;
        domain_proc<NCubeRank>(stencil_, config_, 
                domains_[0].data(), domains_[1].data());

        if (ticks_cnt %2 == 1)
            std::swap(domains_[0], domains_[1]);
    }

private:
    Stencil stencil_;
    Config config_;
    std::array<std::vector<Cell>, 2> domains_;
    std::vector<int3_t> tiles_;
    std::vector<Cell> gh_x0_, gh_y0_, gh_z0_;
    std::vector<Cell> gh_x1_, gh_y1_, gh_z1_;
};

} // namespace geo

#endif // GEOACOUSTIC_SOLVER_HPP_
