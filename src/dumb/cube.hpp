#ifndef GEOACOUSTIC_CUBE_HPP_
#define GEOACOUSTIC_CUBE_HPP_

#include <vector>
#include <iostream>

#include <cassert>

#include "types.hpp"
#include "cell.hpp"
#include "stencil.hpp"

namespace geo {

struct CubeView
{
    int_t rank;
    Cell* data;

    CubeView operator [] (int_t idx)
    {
        assert(idx >= 0 && idx < CORNER_CNT);

        return CubeView {
            /* rank = */ this->rank - 1, 
            /* data = */ this->data + idx * (1 << (3 * (this->rank - 1)))
        };
    }

    // TODO(@geome_try): 
    //  - to implement versions with less parameters count
    //  - to change CubeView's interface to avoid passing rank with it
    void apply(int_t cur_rank, Stencil& stencil, CubeView cur, 
            CubeView x0, CubeView y0, CubeView z0, 
            CubeView x1, CubeView y1, CubeView z1)
    {
        if (cur_rank > 0)
        {
#if defined(__GNUG__)
            #pragma GCC unroll(8)
#elif defined(__clang__) or defined(__INTEL_COMPILER)
            #pragma unroll
#endif
            for (int_t idx = 0; idx < CORNER_CNT/* = 8 */; ++idx)
            {
                (*this)[idx].apply(cur_rank - 1, stencil, cur[idx],
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
            stencil.apply(this->data[0], cur.data[0], 
                    x0.data[0], y0.data[0], z0.data[0], 
                    x1.data[0], y1.data[0], z1.data[0]);
        }
    }
};

class Cube
{
public:
    enum CubeType { PML, ELASTIC };

    explicit Cube(int_t rank):
        rank_{rank},
        cells_{1u << (3 * rank_)}
    {
        assert(rank_ >= 0);
    }

    Cube(Cube&&) noexcept = default;
    Cube& operator = (Cube&&) noexcept = default;

    operator CubeView ()
    {
        return CubeView { /* .rank = */ rank_, /* .data = */ cells_.data() };
    }

    CubeType type() const
    {
        return type_;
    }

    bool load(std::istream& stream)
    {
        for (auto& cell : cells_)
            stream >> cell;

        return true;
    }

    void store(std::ostream& stream) const
    {
        for (const auto& cell : cells_)
            stream << cell;
    }

protected:
    // deleted to avoid CubeView's invalidations
    Cube(const Cube&) = delete;
    Cube& operator = (const Cube&) = delete;

private:
    CubeType type_;
    int_t rank_;
    std::vector<Cell> cells_;
};

} // namespace geo

#endif // GEOACOUSTIC_CUBE_HPP_
