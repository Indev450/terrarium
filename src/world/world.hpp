#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_set>

#include "../tile/tile.hpp"
#include "../utils/vector2i_hash.hpp"

namespace Terrarium {

    class World {
        Tile* tiles = nullptr;

        unsigned int width, height;

        float save_updated_blocks = true;
        // Store updated block positions to redraw them
        std::unordered_set<sf::Vector2i, HashVector2i> updated_blocks;

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
