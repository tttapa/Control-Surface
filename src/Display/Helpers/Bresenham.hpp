#pragma once

#include <Arduino.h>
#include <Printable.h>
#include <Settings/SettingsWrapper.hpp>
#include <limits.h>

BEGIN_CS_NAMESPACE

template <typename T>
static constexpr inline int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/// Line rasterization algorithm for drawing lines to the display.
class BresenhamLine {
  public:
    using pint = int16_t;
    struct Pixel : public Printable {
        pint x = 0, y = 0;
        Pixel(pint x, pint y) : x(x), y(y) {}
        Pixel() = default;
        size_t printTo(Print &p) const override {
            return p.print('(') + p.print(x) + p.print(", ") + p.print(y) +
                   p.print(')');
        };
        unsigned int distanceSquared(Pixel other) const {
            int dx = this->x - other.x;
            int dy = this->y - other.y;
            return dx * dx + dy * dy;
        }
    };

    BresenhamLine(Pixel start, int cos, int sin)
        : px(start),        // Starting point
          dx(cos), dy(sin), // cosine and sine of the slope of the line (scaled)
          adx(abs(dx)), ady(abs(dy)),   // absolute values of cos and sin
          xinc(sgn(dx)), yinc(sgn(dy)), // increment steps for x and y
          steep(ady > adx) // whether to increment x or y on each iteration
    {
        if (steep)
            error = adx - ady + adx;
        else
            error = ady - adx + ady;
    }

    BresenhamLine(Pixel start, float angle)
        : BresenhamLine{
              start,
              BresenhamLine::cos(angle),
              BresenhamLine::sin(angle),
          } {}

    pint getCurrentLength() const { return length; }

    Pixel next() {
        Pixel result = px;
        if (steep) {
            if (error >= 0) {
                px.x += xinc;
                error -= 2 * ady;
            }
            px.y += yinc;
            error += 2 * adx;
        } else {
            if (error >= 0) {
                px.y += yinc;
                error -= 2 * adx;
            }
            px.x += xinc;
            error += 2 * ady;
        }
        ++length;
        return result;
    }

    static int cos(double angle) {
        return round(errorScalingFactor * ::cos(angle));
    }

    static int sin(double angle) {
        return round(errorScalingFactor * ::sin(angle));
    }

  private:
    const static int errorScalingFactor = INT_MAX / 2;
    Pixel px;
    int dx, dy;
    int adx, ady;
    int xinc, yinc;
    bool steep;
    int error;
    pint length = 0;
};

END_CS_NAMESPACE