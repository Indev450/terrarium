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

#include <algorithm>
#include <memory>

#include "world.hpp"
#include "../game.hpp"
#include "../utils/binary_io.hpp"

namespace Terrarium {

    static const int WORLD_TIME_PRECISION = 100;

    World& World::operator=(World &&moved) {
        tiles = std::move(moved.tiles);

        return *this;
    }

    World::World(World &&moved) {
        *this = std::move(moved);
    }

    void World::create(uint16_t width, uint16_t height) {
        tiles.create(width, height, Tile{0, 0});
    }

    void World::load(std::istream &file, GameState &game) {
        uint64_t time = read<uint64_t>(file);
        game_time = double(time) / WORLD_TIME_PRECISION;

        uint16_t width = read<uint16_t>(file);
        uint16_t height = read<uint16_t>(file);

        tiles.create(width, height, Tile{0, 0});

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                *tiles.get(x, y) = read<Tile>(file);
            }
        }
    }

    void World::save(std::ostream &file) const {
        write<uint64_t>(file, game_time * WORLD_TIME_PRECISION);
        write<uint16_t>(file, tiles.width);
        write<uint16_t>(file, tiles.height);

        for (unsigned int y = 0; y < tiles.height; ++y) {
            for (unsigned int x = 0; x < tiles.width; ++x) {
                write(file, *tiles.get(x, y));
            }
        }
    }

    bool World::tickBlockInternal(Block &block) {
        if (block.timer == 0) {
            return false;
        }

        bool ticked = (game_time - block.timer_start) > block.timer;

        if (ticked) {
            block.timer = 0;
        }

        return ticked;
    }

    void World::tickArea(sf::IntRect rect, std::list<TickedBlock> &ticked) {
        int left = std::max(rect.left, 0);
        int right = std::min(rect.left+rect.width, int(tiles.width));
        int top = std::max(rect.top, 0);
        int bottom = std::min(rect.top+rect.height, int(tiles.height));

        for (int x = left; x < right; ++x) {
            for (int y = top; y < bottom; ++y) {
                Tile *tile = tiles.get(x, y);

                if (tickBlockInternal(tile->fg)) {
                    ticked.push_back({ .x = x, .y = y, .id = tile->fg.id, .is_wall = false });
                }

                if (tickBlockInternal(tile->bg)) {
                    ticked.push_back({ .x = x, .y = y, .id = tile->bg.id, .is_wall = true });
                }
            }
        }
    }

}
