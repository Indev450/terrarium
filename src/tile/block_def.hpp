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

#ifndef BLOCK_DEF_HPP
#define BLOCK_DEF_HPP

#include <cstdint>

#include <SFML/Graphics.hpp>

#include "tile.hpp"

namespace Terrarium {

    typedef uint16_t blockid;

    // BB stands for Block Bitmask
    static const uint8_t BB_TOP =         0b00000001;
    static const uint8_t BB_BOTTOM =      0b00000010; // uwu
    static const uint8_t BB_LEFT =        0b00000100;
    static const uint8_t BB_RIGHT =       0b00001000;
    static const uint8_t BB_TOPLEFT =     0b00010000;
    static const uint8_t BB_TOPRIGHT =    0b00100000;
    static const uint8_t BB_BOTTOMLEFT =  0b01000000;
    static const uint8_t BB_BOTTOMRIGHT = 0b10000000;

    // 8-bit masked autotiles have 47 unique visual variants. Extra 2 "variants"
    // are just empty.

    // Precalculated lookup table for positions in spritesheet.
    static const sf::IntRect block_autotile_rects[49] = {
        { 0*16, 0*16, 16, 16 }, { 1*16, 0*16, 16, 16 },
        { 2*16, 0*16, 16, 16 }, { 3*16, 0*16, 16, 16 },
        { 4*16, 0*16, 16, 16 }, { 5*16, 0*16, 16, 16 },
        { 6*16, 0*16, 16, 16 }, { 0*16, 1*16, 16, 16 },
        { 1*16, 1*16, 16, 16 }, { 2*16, 1*16, 16, 16 },
        { 3*16, 1*16, 16, 16 }, { 4*16, 1*16, 16, 16 },
        { 5*16, 1*16, 16, 16 }, { 6*16, 1*16, 16, 16 },
        { 0*16, 2*16, 16, 16 }, { 1*16, 2*16, 16, 16 },
        { 2*16, 2*16, 16, 16 }, { 3*16, 2*16, 16, 16 },
        { 4*16, 2*16, 16, 16 }, { 5*16, 2*16, 16, 16 },
        { 6*16, 2*16, 16, 16 }, { 0*16, 3*16, 16, 16 },
        { 1*16, 3*16, 16, 16 }, { 2*16, 3*16, 16, 16 },
        { 3*16, 3*16, 16, 16 }, { 4*16, 3*16, 16, 16 },
        { 5*16, 3*16, 16, 16 }, { 6*16, 3*16, 16, 16 },
        { 0*16, 4*16, 16, 16 }, { 1*16, 4*16, 16, 16 },
        { 2*16, 4*16, 16, 16 }, { 3*16, 4*16, 16, 16 },
        { 4*16, 4*16, 16, 16 }, { 5*16, 4*16, 16, 16 },
        { 6*16, 4*16, 16, 16 }, { 0*16, 5*16, 16, 16 },
        { 1*16, 5*16, 16, 16 }, { 2*16, 5*16, 16, 16 },
        { 3*16, 5*16, 16, 16 }, { 4*16, 5*16, 16, 16 },
        { 5*16, 5*16, 16, 16 }, { 6*16, 5*16, 16, 16 },
        { 0*16, 6*16, 16, 16 }, { 1*16, 6*16, 16, 16 },
        { 2*16, 6*16, 16, 16 }, { 3*16, 6*16, 16, 16 },
        { 4*16, 6*16, 16, 16 }, { 5*16, 6*16, 16, 16 },
        { 6*16, 6*16, 16, 16 },
    };

    // When calculating bitmask, value may vary from 0 to 255. This lookup table
    // transforms bitmask to index in the rectangle lookup table.
    //
    // Entries with value 255 should never be picked. Array is used to avoid
    // overhead from using actual hash map.
    static const uint8_t block_autotile_rects_indices[256] {
        24, 17, 3, 10, 23, 48, 34, 41, 21, 46, 32, 39, 22, 47, 33, 40,
        255, 255, 255, 255, 255, 16, 255, 29, 255, 255, 255, 255, 255, 37, 255, 25,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 14, 255, 28, 255, 38, 255, 19,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 15, 255, 27,
        255, 255, 255, 255, 255, 255, 2, 36, 255, 255, 255, 255, 255, 255, 30, 18,
        255, 255, 255, 255, 255, 255, 255, 9, 255, 255, 255, 255, 255, 255, 255, 20,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 45,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 35, 255, 255, 31, 26,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 44,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 7, 255, 255, 255, 13,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 5,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 6,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 11,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 12,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 8,
    };

    // Platforms can only have neighbours from left and right (for now)

    // PB stands for Platform Bitmask
    static const uint8_t PB_LEFT =   0b00000001;
    static const uint8_t PB_RIGHT =  0b00000010;

    // There are only 4 variants for all possible values so no need for indices
    // lookup table.
    static const sf::IntRect platform_autotile_rects[4] = {
        { 16*0, 0, 16, 16 }, // No neighbours
        { 16*3, 0, 16, 16 }, // Neighbour from left
        { 16*1, 0, 16, 16 }, // Neighbour from right
        { 16*2, 0, 16, 16 }, // Both neighbours
    };

    struct BlockDef {
        std::string name;

        sf::Sprite sprite;

        enum DrawType {
            None,
            Image,
            Autotile,
            AutotilePlatform,
            Multiblock,
        } draw_type = Image;

        bool autotile_neighbour = true;
        bool autotile_single = false;

        bool is_solid = true;
        bool is_platform = false;
        float slippery = 0;

        bool is_interactive = false;

        sf::Vector3i light;
        bool blocks_light = true;
    };

} // namespace Terrarium

#endif
