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

#include <algorithm>
#include <memory>

#include "world.hpp"
#include "../game.hpp"
#include "../utils/binary_io.hpp"

namespace Terrarium {

    World& World::operator=(World &&moved) {
        tiles = std::move(moved.tiles);

        width = moved.width;
        moved.width = 0;

        height = moved.height;
        moved.height = 0;

        return *this;
    }

    World::World(World &&moved) {
        *this = std::move(moved);
    }

    void World::create(uint16_t _width, uint16_t _height) {
        width = _width;
        height = _height;

        tiles.resize(width*height);

        std::fill(tiles.begin(), tiles.end(), Tile{0, 0});
    }

    void World::load(std::istream &file, GameState &game) {
        width = read<uint16_t>(file);
        height = read<uint16_t>(file);

        tiles.resize(width*height);

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                tiles[y*width + x] = read<Tile>(file);
            }
        }
    }

    void World::save(std::ostream &file) {
        write(file, width);
        write(file, height);

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                write(file, tiles[y*width + x]);
            }
        }
    }

}
