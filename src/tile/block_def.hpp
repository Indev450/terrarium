/*
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

#ifndef BLOCK_DEF_HPP
#define BLOCK_DEF_HPP

#include <cstdint>

#include <SFML/Graphics.hpp>

namespace Terrarium {

    typedef uint16_t blockid;

    struct BlockDef {
        sf::Sprite sprite;

        bool is_solid = true;
        float slippery = 0;
    };

} // namespace Terrarium

#endif
