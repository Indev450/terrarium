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

#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>
#include <unordered_set>
#include <cstdint>
#include <iostream>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "../tile/tile.hpp"
#include "../item/inventory.hpp"
#include "../utils/vector2i_hash.hpp"

namespace Terrarium {

    class GameState;

    class World {
        std::vector<Tile> tiles;

        uint16_t width = 0, height = 0;

        bool save_updated_blocks = true;
        // Store updated block positions to redraw them
        std::unordered_set<sf::Vector2i> updated_blocks;

    public:
        World& operator=(World &&moved);

        World(World &&moved);

        World() = default;

        World(const World &copy) = delete;
        World& operator=(const World &copy) = delete;

        void create(uint16_t _width, uint16_t _height);

        // World save format:
        // u16                               width
        // u16                               height
        // Tile                              tiles[width*height]
        void load(std::istream &file, GameState &game);

        void save(std::ostream &file) const;

        inline bool isInRange(int x, int y) const {
            return x >= 0 && x < static_cast<int>(width) &&
                   y >= 0 && y < static_cast<int>(height);
        }

        inline const Tile *getTile(int x, int y) const {
            if (!isInRange(x, y)) {
                return nullptr;
            }

            return &tiles[y*width + x];
        }

        inline blockid getBlock(int x, int y) const {
            if (!isInRange(x, y)) {
                return 0;
            }

            return tiles[y*width + x].fg;
        }

        inline sf::Vector2i getMultiblockOrigin(int x, int y) const {
            const Tile *tile = getTile(x, y);

            if (tile == nullptr) {
                return sf::Vector2i(x, y);
            }

            return sf::Vector2i(x - int(tile->multiblock_origin.x), y - int(tile->multiblock_origin.y));
        }

        inline blockid getWall(int x, int y) const {
            if (!isInRange(x, y)) {
                return 0;
            }

            return tiles[y*width + x].bg;
        }

        inline void setBlock(int x, int y, blockid block) {
            if (!isInRange(x, y)) {
                return;
            }

            tiles[y*width + x].fg = block;

            if (save_updated_blocks) {
                updated_blocks.emplace(x, y);
                updated_blocks.emplace(x-1, y);
                updated_blocks.emplace(x+1, y);
                updated_blocks.emplace(x, y-1);
                updated_blocks.emplace(x, y+1);
            }
        }

        inline void setMultiblockOffset(int x, int y, uint8_t xoff, uint8_t yoff) {
            if (!isInRange(x, y)) {
                return;
            }

            tiles[y*width + x].multiblock_origin = sf::Vector2<uint8_t>(xoff, yoff);

            if (save_updated_blocks) {
                updated_blocks.emplace(x, y);
            }
        }

        inline void setMultiblock(int x, int y, uint8_t width, uint8_t height, blockid block) {
            if (!(isInRange(x, y) && isInRange(x+width-1, y+height-1))) {
                return;
            }

            for (uint8_t xoff = 0; xoff < width; ++xoff) {
                for (uint8_t yoff = 0; yoff < height; ++yoff) {
                    setBlock(x+int(xoff), y+int(yoff), block);
                    setMultiblockOffset(x+int(xoff), y+int(yoff), xoff, yoff);
                }
            }
        }

        inline void setWall(int x, int y, blockid block) {
            if (!isInRange(x, y)) {
                return;
            }

            tiles[y*width + x].bg = block;

            if (save_updated_blocks) {
                updated_blocks.emplace(x, y);
                updated_blocks.emplace(x-1, y);
                updated_blocks.emplace(x+1, y);
                updated_blocks.emplace(x, y-1);
                updated_blocks.emplace(x, y+1);
            }
        }

        // To be used in mapgen (mapgen usually changes EVERY world block)
        // so saving every change is huge waste of memory
        inline void stopSavingUpdatedBlocks() {
            save_updated_blocks = false;
        }

        inline void startSavingUpdatedBlocks() {
            save_updated_blocks = true;
        }

        // To be used in WorldRenderer
        inline bool isUpdated() const {
            return !updated_blocks.empty();
        }

        inline bool isUpdated(const sf::Vector2i &pos) const {
            return updated_blocks.count(pos) != 0;
        }

        inline void resetUpdated() {
            updated_blocks.clear();
        }

        inline uint16_t getWidth() const {
            return width;
        }

        inline uint16_t getHeight() const {
            return height;
        }
    };

}

#endif
