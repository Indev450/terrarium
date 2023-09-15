/*
 * Copyright 2022 Indev
 *
 * This file is part of TerrariumEngine
 *
 * TerrariumEngine is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

// Example: stepify(19, 16) = floor(19/16) * 16 = floor(1.whatever) * 16 = 1 * 16 = 16
inline float stepify(float x, float step) {
    return floor(x / step) * step;
}

template <class TL, class TR, class TT>
constexpr auto lerp(const TL a, const TR b, const TT t) {
    return a + (b - a) * t;
}

// Scales value to new min-max range
template <class T>
constexpr auto rescale(const T value, const T oldmin, const T oldmax, const T newmin, const T newmax) {
    return newmin + (value-oldmin)/oldmax*(newmax-newmin);
}

// Takes 2 values from -1 to 1, combines them and returns new value that is
// still from -1 to 1
constexpr double combine_noise(const double value1, const double value2) {
    return (value1 + value2) / 2;
}

#endif
