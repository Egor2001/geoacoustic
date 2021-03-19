#ifndef GEOACOUSTIC_STATIC_SOLVER_HPP_
#define GEOACOUSTIC_STATIC_SOLVER_HPP_

#include <iostream>

#include "types.hpp"
#include "config.hpp"
#include "cell_layout.hpp"
#include "grid_proc.hpp"

namespace geo {

template<typename TCell, int_t NTileRank>
class Solver
{
public:
    explicit Solver(Config<TCell> cfg) :
        cfg_{std::move(cfg)},
        ctx_{create_context(cfg_)}
    {}

    Solver(Config<TCell> cfg, std::istream& cfg_in, std::istream& ctx_in) :
        Solver(std::move(cfg))
    {
        CellLayout<TCell>::load(cfg_.grid_size, cfg_.bulk.span(), cfg_in);
        CellLayout<TCell>::load(cfg_.grid_size, ctx_.ampl.span(), ctx_in);
    }

    void proc(std::ostream& out)
    {
        grid_proc<TCell, NTileRank>(cfg_, ctx_);
        CellLayout<TCell>::store(cfg_.grid_size, ctx_.ampl_next.span(), out);
    }

private:
    Config<TCell> cfg_;
    Context<TCell> ctx_;
};

} // namespace geo

#endif // GEOACOUSTIC_STATIC_SOLVER_HPP_
