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

#include "world.hpp"
#include "../game.hpp"
#include "../utils/binary_io.hpp"

namespace Terrarium {

    void World::create(uint16_t _width, uint16_t _height) {
        width = _width;
        height = _height;

        if (tiles != nullptr) {
            delete[] tiles;
        }

        tiles = new Tile[width*height];

        for (unsigned int i = 0; i < width*height; ++i) {
            tiles[i] = { 0, 0 };
        }
    }

    void World::load(std::istream &file, GameState &game) {
        width = read<uint16_t>(file);
        height = read<uint16_t>(file);

        tiles = new Tile[width*height];

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                tiles[y*width + x] = read<Tile>(file);
            }
        }

        uint16_t inventories_count = read<uint16_t>(file);

        for (unsigned int i = 0; i < inventories_count; ++i) {
            uint16_t x = read<uint16_t>(file);
            uint16_t y = read<uint16_t>(file);

            auto inventory = std::make_shared<Inventory>(0);

            inventory->load(file, game);

            block_inventories[sf::Vector2i(x, y)] = inventory;
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

        write<uint16_t>(file, block_inventories.size());

        for (auto &pair: block_inventories) {
            write<uint16_t>(file, pair.first.x);
            write<uint16_t>(file, pair.first.y);

            pair.second->save(file);
        }
    }

}
