#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

// Example: stepify(19, 16) = floor(19/16) * 16 = floor(1.whatever) * 16 = 1 * 16 = 16
inline float stepify(float x, float step) {
    return floor(x / step) * step;
}

#endif
