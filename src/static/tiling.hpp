#ifndef GEOACOUSTIC_TILING_HPP_
#define GEOACOUSTIC_TILING_HPP_

#include <vector>
#include <algorithm>

#include "types.hpp"
#include "stencil.hpp"

namespace geo {

template<int_t NCubeRank>
std::vector<int3_t> create_grid(int3_t dim3)
{
    static constexpr int_t NCubeVolume = 1 << (3 * NCubeRank);

    std::vector<int3_t> tiles;
    for (int_t z = 0; z < dim3.z; ++z)
    for (int_t y = 0; y < dim3.y; ++y)
    for (int_t x = 0; x < dim3.x; ++x)
    {
        tiles.push_back(int3_t{
                NCubeVolume * x, 
                NCubeVolume * y, 
                NCubeVolume * z
            });
    }

    auto tiles_comp = [](const int3_t& lhs, const int3_t& rhs) -> bool
    {
        return 
            std::make_tuple(lhs.x + lhs.y + lhs.z, -lhs.z, -lhs.y, -lhs.x) >
            std::make_tuple(rhs.x + rhs.y + rhs.z, -rhs.z, -rhs.y, -rhs.x);
    };

    std::stable_sort(std::begin(tiles), std::end(tiles), tiles_comp);

    return tiles;
}

} // namespace geo

#endif // GEOACOUSTIC_TILING_HPP_
