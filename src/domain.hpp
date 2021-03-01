#ifndef GEOACOUSTIC_DOMAIN_HPP_
#define GEOACOUSTIC_DOMAIN_HPP_

// TODO(@geome_try): to add store / load from stream
// #include <istream>
#include <cassert>

#include "types.hpp"
#include "cube.hpp"

namespace geo {

class Domain
{
public:
    Domain(int3_t dim3, int_t cubes_rank):
        dims_{dim3 + int3_t{2, 2, 2}}, // add ghost cubes at {-1, dim}^3
        cubes_rank_{cubes_rank},
        cubes_(dim3_.x * dim3_.y * dim3_.z, Cube(cubes_rank))
    {
        assert(dim3_.x > 0 && dim3_.y > 0 && dim3_.z > 0);
    }

    int_t cubes_rank() const
    {
        return cubes_rank_;
    }

    int3_t dim3() const
    {
        return dim3_;
    }

    Cube& at(int3_t idx3) // const
    {
        idx3 += {1, 1, 1}; // shift because of ghost cubes
        assert(idx3_.x > 0 && idx3_.y > 0 && idx3_.z > 0 && 
               dim3_.x > idx3_.x && dim3_.y > idx3_.y && dim3_.z > idx3_.z);

        return cubes_[dim3_.x * dim3_.y * idx3.z + dim3_.x * idx3.y + idx3.x];
    }

    const Cube& at(int3_t idx3) const
    {
        return const_cast<Domain*>(this)->at(idx3);
    }

    // TODO(@geome_try):
    // static Domain load(std::istream& stream);
    // static Domain store(std::ostream& stream);

private:
    int3_t dim3_;
    int_t cubes_rank_;
    std::vector<Cube> cubes_;
};

} // namespace geo

#endif // GEOACOUSTIC_DOMAIN_HPP_
