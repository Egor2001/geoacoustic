#ifndef GEOACOUSTIC_TYPES_HPP_
#define GEOACOUSTIC_TYPES_HPP_

#include <cstdint>

#include "macro.hpp"

namespace geo {

using int_t = int64_t;
using real_t = double;

struct int3_t { int_t x, y, z; };
struct real3_t { real_t x, y, z; };

int3_t& operator += (int3_t&, const int3_t&);
int3_t& operator -= (int3_t&, const int3_t&);
int3_t operator + (const int3_t&, const int3_t&);
int3_t operator - (const int3_t&, const int3_t&);

real3_t& operator += (real3_t&, const real3_t&);
real3_t& operator -= (real3_t&, const real3_t&);
real3_t operator + (const real3_t&, const real3_t&);
real3_t operator - (const real3_t&, const real3_t&);

enum ESide
{
    SIDE_X0, SIDE_X1, SIDE_Y0, SIDE_Y1, SIDE_Z0, SIDE_Z1,
    SIDE_CNT
};

enum EEdge
{
    EDGE_X0_Y0, EDGE_X1_Y0, EDGE_X0_Y1, EDGE_X1_Y1,
    EDGE_X0_Z0, EDGE_X1_Z0, EDGE_X0_Z1, EDGE_X1_Z1,
    EDGE_Y0_Z0, EDGE_Y1_Z0, EDGE_Y0_Z1, EDGE_Y1_Z1,
    EDGE_CNT
};

enum ECorner
{
    CORNER_000, CORNER_001, CORNER_010, CORNER_011,
    CORNER_100, CORNER_101, CORNER_110, CORNER_111,
    CORNER_CNT
};

int3_t& operator += (int3_t& lhs, const int3_t& rhs)
{
    lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z;
    return lhs;
}

int3_t& operator -= (int3_t& lhs, const int3_t& rhs)
{
    lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z;
    return lhs;
}

int3_t operator + (const int3_t& lhs, const int3_t& rhs)
{
    auto res = lhs;
    return (res += rhs);
}

int3_t operator - (const int3_t& lhs, const int3_t& rhs)
{
    auto res = lhs;
    return (res -= rhs);
}

real3_t& operator += (real3_t& lhs, const real3_t& rhs)
{
    lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z;
    return lhs;
}

real3_t& operator -= (real3_t& lhs, const real3_t& rhs)
{
    lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z;
    return lhs;
}

real3_t operator + (const real3_t& lhs, const real3_t& rhs)
{
    auto res = lhs;
    return (res += rhs);
}

real3_t operator - (const real3_t& lhs, const real3_t& rhs)
{
    auto res = lhs;
    return (res -= rhs);
}

} // namespace geo

#endif // GEOACOUSTIC_TYPES_HPP_
