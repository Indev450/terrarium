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

#ifndef VECTOR2I_HASH_HPP
#define VECTOR2I_HASH_HPP

#include <limits>

#include <SFML/System/Vector2.hpp>

template <>
struct std::hash<sf::Vector2i> {
    std::size_t operator()(const sf::Vector2i &vec) const noexcept {
        return vec.x + static_cast<size_t>(vec.y) * std::numeric_limits<int>::max();
    }
};

#endif
