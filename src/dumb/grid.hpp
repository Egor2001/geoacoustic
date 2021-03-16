#ifndef GEOACOUSTIC_GRID_HPP_
#define GEOACOUSTIC_GRID_HPP_

#include <cassert>

#include "types.hpp"
#include "domain.hpp"
#include "stencil.hpp"

namespace geo {

template<typename TTiling>
class Grid
{
public:
    using Tile = typename TTiling::Tile;

    explicit Grid(std::vector<Tile>&& tiles):
        tiles_(std::move(tiles))
    {}

    void process(Domain& next, Domain& cur, Stencil& stencil, int_t time_ticks)
    {
        for (int_t tick = 0; tick < time_ticks; tick += 2)
        {
            for (auto& tile : tiles_)
                tile.process(next, cur, stencil);

            for (auto& tile : tiles_)
                tile.process(cur, next, stencil);
        }

        if (time_ticks % 2 == 1)
        {
            for (auto& tile : tiles_)
                tile.process(next, cur, stencil);
        }
    }

private:
    std::vector<Tile> tiles_;
};

} // namespace geo

#endif // GEOACOUSTIC_GRID_HPP_
