#ifndef GEOACOUSTIC_STATIC_CONTEXT_HPP_
#define GEOACOUSTIC_STATIC_CONTEXT_HPP_

#include "types.hpp"
#include "config.hpp"
#include "volume_array.hpp"

namespace geo {

// TODO: to add pml context

template<typename TC>
struct Context
{
    using TCell = TC;

    VolumeArray<TCell> ampl, ampl_next; // current speed values
};

template<typename TCell>
Context<TCell> create_context(const Config<TCell>& cfg)
{
    return Context<TCell> {
        /* .ampl = */ 
        VolumeArray<TCell>(cfg.grid_size),
        /* .ampl_next = */
        VolumeArray<TCell>(cfg.grid_size),
    };
}

} // namespace geo

#endif // GEOACOUSTIC_STATIC_CONTEXT_HPP_
