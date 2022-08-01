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
#include <unordered_map>

#include "../tile/tile.hpp"
#include "../utils/vector2i_hash.hpp"
#include "../item/inventory.hpp"

namespace Terrarium {

    class World {
        Tile* tiles = nullptr;

        unsigned int width, height;

        bool save_updated_blocks = true;
        // Store updated block positions to redraw them
        std::unordered_set<sf::Vector2i, HashVector2i> updated_blocks;

        std::unordered_map<sf::Vector2i, std::shared_ptr<Inventory>, HashVector2i> block_inventories;

    public:
        World(unsigned int _width, unsigned int _height):
            width(_width), height(_height)
        {
            tiles = new Tile[width*height];
        }

        inline bool isInRange(int x, int y) {
            return x >= 0 && x < static_cast<int>(width) &&
                   y >= 0 && y < static_cast<int>(height);
        }

        inline const Tile *getTile(int x, int y) {
            if (!isInRange(x, y)) {
                return nullptr;
            }

            return &tiles[y*width + x];
        }

        blockid getBlock(int x, int y) {
            if (!isInRange(x, y)) {
                return 0;
            }

            return tiles[y*width + x].fg;
        }

        blockid getWall(int x, int y) {
            if (!isInRange(x, y)) {
                return 0;
            }

            return tiles[y*width + x].bg;
        }

        void setBlock(int x, int y, blockid block) {
            if (!isInRange(x, y)) {
                return;
            }

            tiles[y*width + x].fg = block;

            if (save_updated_blocks) {
                updated_blocks.emplace(x, y);
            }
        }

        void setWall(int x, int y, blockid block) {
            if (!isInRange(x, y)) {
                return;
            }

            tiles[y*width + x].bg = block;

            if (save_updated_blocks) {
                updated_blocks.emplace(x, y);
            }
        }

        std::shared_ptr<Inventory> getBlockInventory(const sf::Vector2i &position) {
            if (block_inventories.count(position) == 0) {
                block_inventories[position] = std::make_shared<Inventory>(0);
            }

            return block_inventories[position];
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
        inline bool isUpdated() {
            return !updated_blocks.empty();
        }

        inline bool isUpdated(sf::Vector2i pos) {
            return updated_blocks.count(pos) != 0;
        }

        inline void resetUpdated() {
            updated_blocks.clear();
        }

        inline unsigned int getWidth() {
            return width;
        }

        inline unsigned int getHeight() {
            return height;
        }

        ~World() {
            delete[] tiles;
        }
    };

}

#endif
