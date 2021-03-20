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

    void proc()
    {
        grid_proc<TCell, NTileRank>(cfg_, ctx_);
    }

    const Config<TCell>& get_cfg() const
    {
        return cfg_;
    }

    const Context<TCell>& get_ctx() const
    {
        return ctx_;
    }

    void load_ctx(std::istream& from)
    {
        CellLayout<TCell>::load(cfg_.grid_size, ctx_.ampl.span(), from);
    }

private:
    Config<TCell> cfg_;
    Context<TCell> ctx_;
};

} // namespace geo

#endif // GEOACOUSTIC_STATIC_SOLVER_HPP_
