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

    void load_bulk(std::istream& stream)
    {
        CellLayout<TCell>::load(cfg_.grid_size, cfg_.bulk.span(), 
                                stream);
    }

    void load_rho(std::istream& stream)
    {
        CellLayout<TCell>::load(cfg_.grid_size, cfg_.rho.span(), 
                                stream);
    }

    void fill_bulk(std::function<real_t(int3_t, int3_t)> func)
    {
        CellLayout<TCell>::fill(cfg_.grid_size, cfg_.bulk.span(), 
                                std::move(func));
    }

    void fill_rho(std::function<real_t(int3_t, int3_t)> func)
    {
        CellLayout<TCell>::fill(cfg_.grid_size, cfg_.rho.span(), 
                                std::move(func));
    }

    void load_init(std::istream& stream)
    {
        CellLayout<TCell>::load(cfg_.grid_size, ctx_.ampl.span(), 
                                stream);
    }

    void fill_init(std::function<real_t(int3_t, int3_t)> func)
    {
        CellLayout<TCell>::fill(cfg_.grid_size, ctx_.ampl.span(), 
                                std::move(func));
    }

    void store_result(std::ostream& stream) const
    {
        CellLayout<TCell>::store(cfg_.grid_size, ctx_.ampl_next.span(), 
                                 stream);
    }

    void read_result(std::function<void(int3_t, int3_t, real_t)> func) const
    {
        CellLayout<TCell>::read(cfg_.grid_size, ctx_.ampl_next.span(), 
                                std::move(func));
    }

private:
    Config<TCell> cfg_;
    Context<TCell> ctx_;
};

} // namespace geo

#endif // GEOACOUSTIC_STATIC_SOLVER_HPP_
