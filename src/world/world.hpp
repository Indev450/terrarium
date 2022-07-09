#ifndef WORLD_HPP
#define WORLD_HPP

#include "../tile/tile.hpp"

namespace Terrarium {

    class World {
        Tile* tiles = nullptr;

        unsigned int width, height;

        bool updated = false;

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

            updated = true;
        }

        void setWall(int x, int y, blockid block) {
            if (!isInRange(x, y)) {
                return;
            }

            tiles[y*width + x].bg = block;

            updated = true;
        }

        // Should be used ONLY in WorldRenderer
        inline bool isUpdated() {
            bool result = updated;
            updated = false;

            return result;
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
