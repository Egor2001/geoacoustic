#ifndef GEOACOUSTIC_CELL_HPP_
#define GEOACOUSTIC_CELL_HPP_

#include <iostream>

#include "types.hpp"

namespace geo {

struct Cell
{
    real3_t data;
};

std::istream& operator >> (std::istream& stream, Cell& cell);
std::ostream& operator << (std::ostream& stream, const Cell& cell);

std::istream& operator >> (std::istream& stream, Cell& cell)
{
    return stream >> cell.data.x >> cell.data.y >> cell.data.z;
}

std::ostream& operator << (std::ostream& stream, const Cell& cell)
{
    return stream << cell.data.x << ' ' << 
                     cell.data.y << ' ' << 
                     cell.data.z << ' ';
}

} // namespace geo

#endif // GEOACOUSTIC_CELL_HPP_
