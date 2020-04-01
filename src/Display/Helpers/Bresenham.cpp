#include "Bresenham.hpp"

BEGIN_CS_NAMESPACE

size_t BresenhamLine::Pixel::printTo(Print &p) const {
    return p.print('(') + p.print(x) + p.print(", ") + p.print(y) +
           p.print(')');
}

END_CS_NAMESPACE