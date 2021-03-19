#ifndef GEOACOUSTIC_STENCIL_HPP_
#define GEOACOUSTIC_STENCIL_HPP_

#include "cell.hpp"
#include "types.hpp"

namespace geo {

class Stencil
{
public:
    static constexpr int_t NOrder = 1;

    // TODO(@geome_try):
    // Stencil(/*const Config& config*/);

    void apply(Cell* next, const Cell* cur, 
            const Cell* x0, const Cell* y0, const Cell* z0,
            const Cell* x1, const Cell* y1, const Cell* z1)
    {
        next->data = next->data + cur->data + 
            x0->data + y0->data + z0->data + 
            x1->data + y1->data + z1->data;

        real_t scale = 1.0 / 8.0;
        next->data.x *= scale; next->data.y *= scale; next->data.z *= scale;
    }

    // TODO(@geome_try):
    // void apply_pml(...)

private:
    /* dx, dy, dx, dt, ... */
};

} // namespace geo

#endif // GEOACOUSTIC_STENCIL_HPP_
