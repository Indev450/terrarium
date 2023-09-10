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

#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/System/Vector2.hpp>

#include "block_def.hpp"

namespace Terrarium {

    struct Block {
        blockid id = 0;
        double timer_start = 0;
        double timer = 0;

        Block() = default;

        Block(blockid _id): id(_id), timer(0) {}

        inline void set(blockid newid) {
            id = newid;
            timer = 0;
        }

        inline operator bool() const {
            return id != 0;
        }
    };

    struct Tile {
        static const int SIZE = 16;

        Block fg;
        Block bg;

        sf::Vector2<uint8_t> multiblock_offset;
    };

}

#endif
