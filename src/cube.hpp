#ifndef GEOACOUSTIC_CUBE_HPP_
#define GEOACOUSTIC_CUBE_HPP_

#include <vector>

#include <cassert>

#include "types.hpp"
#include "cell.hpp"

namespace geo {

struct CubeView
{
    CubeView operator [] (int_t idx)
    {
        assert(idx >= 0 && idx < CORNER_CNT);
        assert(rank > 0);
        return CubeView {
            /* rank = */ rank - 1, 
            /* data = */ data + idx * (1 << (3 * (rank - 1)))
        };
    }

    int_t rank;
    Cell* data;
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

    Cube(Cube&&) = default;
    Cube& operator = (Cube&&) = default;

    operator CubeView ()
    {
        return CubeView { /* .rank = */ rank_, /* .data = */ cells_.data() };
    }

    CubeType type() const
    {
        return type_;
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
