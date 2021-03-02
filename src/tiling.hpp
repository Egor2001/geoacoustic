#ifndef GEOACOUSTIC_TILING_HPP_
#define GEOACOUSTIC_TILING_HPP_

#include <vector>
#include <algorithm>

#include "types.hpp"
#include "stencil.hpp"
#include "cube.hpp"
#include "stencil.hpp"
#include "domain.hpp"
#include "grid.hpp"

namespace geo {

class Tiling
{
public:
    struct Tile
    {
        int3_t idx3;

        void process(Domain& next, Domain& cur, Stencil& stencil)
        {
            int_t rank = next.cubes_rank();
            static_cast<CubeView>(next.at(idx3))
                .apply(rank, stencil, cur.at(idx3),
                        cur.at(idx3 + int3_t{-1, 0, 0}), // x0
                        cur.at(idx3 + int3_t{0, -1, 0}), // y0
                        cur.at(idx3 + int3_t{0, 0, -1}), // z0
                        cur.at(idx3 + int3_t{+1, 0, 0}), // x1
                        cur.at(idx3 + int3_t{0, +1, 0}), // y1
                        cur.at(idx3 + int3_t{0, 0, +1})  // z1
                    );
        }
    };

    explicit Tiling(int3_t dim3):
        dim3_{dim3}
    {}

    Grid<Tiling> grid() const
    {
        std::vector<Tile> tiles;
        for (int_t z = 0; z < dim3_.z; ++z)
        for (int_t y = 0; y < dim3_.y; ++y)
        for (int_t x = 0; x < dim3_.x; ++x)
            tiles.push_back(Tile{/* cube_idx3_ = */ {x, y, z}});

        auto tiles_comp = [](const Tile& lhs, const Tile& rhs) -> bool
        {
            return std::greater{}(
                    std::make_tuple(lhs.idx3.x + lhs.idx3.y + lhs.idx3.z, 
                        -lhs.idx3.z, -lhs.idx3.y, -lhs.idx3.x),
                    std::make_tuple(rhs.idx3.x + rhs.idx3.y + rhs.idx3.z, 
                        -rhs.idx3.z, -rhs.idx3.y, -rhs.idx3.x)
                );
        };

        std::stable_sort(std::begin(tiles), std::end(tiles), tiles_comp);

        return Grid<Tiling>(std::move(tiles));
    }

private:
    int3_t dim3_;
};

} // namespace geo

#endif // GEOACOUSTIC_TILING_HPP_
