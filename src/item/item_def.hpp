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

#ifndef ITEM_DEF_HPP
#define ITEM_DEF_HPP

#include <string>
#include <cstdint>
#include <set>

#include <SFML/Graphics.hpp>

namespace Terrarium {

    struct ItemDef {
        std::string name = "default:name";
        std::string description = "Default description";
        std::set<std::string> tags;

        sf::Sprite inventory_image;

        uint16_t max_count = 1;
    };

}

#endif
