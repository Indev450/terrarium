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

#include "mapgen_base.hpp"

namespace Terrarium {

    std::optional<Tile> Decoration::getLocalTile(int x, int y) const {
        if (x < 0 || x >= static_cast<int>(width) ||
            y < 0 || y >= static_cast<int>(height)) {

            return std::nullopt;
        }

        return tiles[y*width + x];
    }

    bool Decoration::canPlace(const World &world, int x, int y) const {
        for (auto &cond: conditions) {
            const Tile *check_tile = world.getTile(x + cond.position.x,
                                                   y + cond.position.y);

            if (check_tile == nullptr) {
                return false;
            }

            if (!cond.satisfied(*check_tile)) {
                return false;
            }
        }

        for (int off_x = 0; off_x < static_cast<int>(width); ++off_x) {
            for (int off_y = 0; off_y < static_cast<int>(height); ++off_y) {
                int check_x = x + (off_x - origin.x);
                int check_y = y + (off_y - origin.y);

                const Tile *tile = world.getTile(check_x, check_y);

                // Here we are sure that getLocalTile called with correct index
                Tile place_tile = *getLocalTile(off_x, off_y);

                // Out of bounds
                if (tile == nullptr) {
                    return false;
                }

                if (place_tile.fg != 0 && tile->fg != 0) {
                    return false;
                }

                if (place_tile.bg != 0 && tile->bg != 0) {
                    return false;
                }
            }
        }

        return true;
    }

    void Decoration::place(World &world, int x, int y) const {
        for (int off_x = 0; off_x < static_cast<int>(width); ++off_x) {
            for (int off_y = 0; off_y < static_cast<int>(height); ++off_y) {
                int place_x = x + (off_x - origin.x);
                int place_y = y + (off_y - origin.y);

                // Here we are sure that getLocalTile called with correct index
                Tile place_tile = *getLocalTile(off_x, off_y);

                if (place_tile.fg != 0) {
                    world.setBlock(place_x, place_y, place_tile.fg);
                    world.setMultiblockOffset(
                            place_x, place_y,
                            place_tile.multiblock_offset.x, place_tile.multiblock_offset.y);
                }

                if (place_tile.bg != 0) {
                    world.setWall(place_x, place_y, place_tile.bg);
                }
            }
        }
    }

}
