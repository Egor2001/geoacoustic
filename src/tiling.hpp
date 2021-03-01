#ifndef GEOACOUSTIC_TILING_HPP_
#define GEOACOUSTIC_TILING_HPP_

#include "types.hpp"
#include "stencil.hpp"
#include "cube_view.hpp"
#include "stencil.hpp"

namespace geo {

class Tiling
{
public:
    explicit Tiling(Stencil stencil):
        stencil_{std::move(stencil)}
    {}

    // TODO(@geome_try): 
    //  - to implement versions with less parameters count
    //  - to change CubeView's interface to avoid passing rank with it
    void apply(int_t rank, CubeView next, CubeView cur, 
            CubeView x0, CubeView y0, CubeView z0, 
            CubeView x1, CubeView y1, CubeView z1)
    {
        if (rank > 0)
        {
            #pragma unroll
            for (int_t idx = 0; idx < CORNER_CNT/* = 8 */; ++idx)
            {
                apply(rank - 1, next[idx], cur[idx],
                        // this corner's x0, y0, z0
                        ((idx & 1) == 0 ? x0[idx ^ 1] : cur[idx ^ 1]),
                        ((idx & 2) == 0 ? y0[idx ^ 2] : cur[idx ^ 2]),
                        ((idx & 4) == 0 ? z0[idx ^ 4] : cur[idx ^ 4]),
                        // this corner's x1, y1, z1
                        ((idx & 1) == 0 ? cur[idx ^ 1] : x1[idx ^ 1]),
                        ((idx & 2) == 0 ? cur[idx ^ 2] : y1[idx ^ 2]),
                        ((idx & 4) == 0 ? cur[idx ^ 4] : z1[idx ^ 4])
                    ); // process()
            }
        }
        else
        {
            stencil.apply(next.data[0], cur.data[0], 
                    x0.data[0], y0.data[0], z0.data[0], 
                    x1.data[0], y1.data[0], z1.data[0]);
        }
    }

private:
    Stencil stencil_;
};

} // namespace geo

#endif // GEOACOUSTIC_TILING_HPP_
