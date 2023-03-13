/*
 * Copyright 2022 Inner Devil
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

#ifndef BLOCK_DEF_HPP
#define BLOCK_DEF_HPP

#include <cstdint>

#include <SFML/Graphics.hpp>

namespace Terrarium {

    typedef uint16_t blockid;

    // When drawing an "autotile", game calculates which rect index to use by
    // creating bitmask:
    //
    // 0b1111
    //
    // Each bit represents presence of neighbour block
    // 0b0001 - left
    // 0b0010 - right
    // 0b0100 - up
    // 0b1000 - down
    static const sf::IntRect block_autotile_rects[16] = {
        { 0,   0,  16, 16 }, // 0b0000 - no neighbours
        { 16,  0,  16, 16 }, // 0b0001 - left
        { 32,  0,  16, 16 }, // 0b0010 - right
        { 48,  0,  16, 16 }, // 0b0011 - left and right
        { 64,  0,  16, 16 }, // 0b0100 - up
        { 80,  0,  16, 16 }, // 0b0101 - up and left
        { 96,  0,  16, 16 }, // 0b0110 - up and right
        { 112, 0,  16, 16 }, // 0b0111 - up and left and right
        { 0,   16, 16, 16 }, // 0b1000 - down
        { 16,  16, 16, 16 }, // 0b1001 - down and left
        { 32,  16, 16, 16 }, // 0b1010 - down and right
        { 48,  16, 16, 16 }, // 0b1011 - down and left and right
        { 64,  16, 16, 16 }, // 0b1100 - down and up
        { 80,  16, 16, 16 }, // 0b1101 - down and up and left
        { 96,  16, 16, 16 }, // 0b1110 - down and up and right
        { 112, 16, 16, 16 }, // 0b1111 - all neighbours
    };

    struct BlockDef {
        std::string name;

        sf::Sprite sprite;

        enum DrawType {
            None,
            Image,
            Autotile,
            Multiblock,
        } draw_type = Image;

        bool autotile_neighbour = true;
        bool autotile_single = false;

        bool is_solid = true;
        float slippery = 0;

        bool is_interactive = false;

        uint8_t light = 0;
        bool blocks_light = true;
    };

} // namespace Terrarium

#endif
