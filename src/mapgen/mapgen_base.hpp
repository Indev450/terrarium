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

#ifndef MAPGEN_BASE_HPP
#define MAPGEN_BASE_HPP

#include <vector>
#include <algorithm>
#include <memory>

#include "../tile/tile.hpp"
#include "../world/world.hpp"

namespace Terrarium {

    // Trees, grass, etc
    struct Decoration {
        sf::Vector2i origin;

        uint16_t width;
        uint16_t height;

        std::vector<Tile> tiles;

        float place_chance = 0.5;

        bool canPlace(const World &world, int x, int y) const {
            // TODO - maybe add something like "check_tile" or "place_on_tile"
            // into decor definition?
            const Tile *place_on_tile = world.getTile(x, y + 1);

            if (place_on_tile == nullptr) { return false; }

            if (place_on_tile->fg == 0) { return false; }

            for (int off_x = 0; off_x < static_cast<int>(width); ++off_x) {
                for (int off_y = 0; off_y < static_cast<int>(height); ++off_y) {
                    int check_x = x + (off_x - origin.x);
                    int check_y = y + (off_y - origin.y);

                    const Tile *tile = world.getTile(check_x, check_y);
                    const Tile &place_tile = tiles[off_y*width + off_x];

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

        void place(World &world, int x, int y) const {
            for (int off_x = 0; off_x < static_cast<int>(width); ++off_x) {
                for (int off_y = 0; off_y < static_cast<int>(height); ++off_y) {
                    int place_x = x + (off_x - origin.x);
                    int place_y = y + (off_y - origin.y);

                    const Tile &place_tile = tiles[off_y*width + off_x];

                    if (place_tile.fg != 0) {
                        world.setBlock(place_x, place_y, place_tile.fg);
                    }

                    if (place_tile.bg != 0) {
                        world.setWall(place_x, place_y, place_tile.bg);
                    }
                }
            }
        }
    };

    struct Biome {
        float humidity_min = -1.0;
        float humidity_max = 1.0;

        float heat_min = -1.0;
        float heat_max = 1.0;

        Tile top = { 0, 0 };
        unsigned int top_depth = 1;

        Tile filler = { 0, 0 };
        unsigned int filler_depth = 1;

        Tile stone = { 0, 0 };

        float min_depth = 0.0;

        float max_depth = 1.0;

        // Lowest number -> lowest priority. Biomes with lowest priority generated
        // first, then they "overriden" by higher priority biomes.
        unsigned int priority = 0;

        std::vector<std::shared_ptr<Decoration>> decorations;

        // for sorting
        bool operator<(const Biome &other) {
            return priority < other.priority;
        }
    };

    struct Ore {
        unsigned int cluster_tiles = 12; // Max number of tiles placed

        unsigned int distribution = 48; // minimum distance between ore clusters

        float min_depth = 0.0; // Min depth from world top

        float max_depth = 1.0; // Max depth from world top

        Tile tile = { 0, 0 }; // Tile to be placed. blockid 0 means nothing is placed
    };

    class MapgenBase {
    protected:
        std::vector<Biome> biomes;
        std::vector<Ore> ores;

        enum SearchLayer {
            FOREGROUND,
            BACKGROUND,
        };

        virtual void generate(World &world) = 0;

        bool findAir(World &world, int cx, int cy, int radius, SearchLayer where) {
            for (int x = cx - radius; x <= cx + radius; ++x) {
                for (int y = cy - radius; y <= cy + radius; ++y) {
                    const Tile *tile = world.getTile(x, y);

                    if (tile == nullptr) {
                        continue;
                    }

                    switch (where) {
                        case FOREGROUND:
                            if (tile->fg == 0)
                                return true;
                        break;

                        case BACKGROUND:
                            if (tile->bg == 0)
                                return true;
                        break;
                    }
                }
            }

            return false;
        }

    public:
        virtual void setFiller(const Tile &filler) = 0;

        void addBiome(const Biome &biome) {
            biomes.push_back(biome);
        }

        void addOre(const Ore &ore) {
            ores.push_back(ore);
        }

        void run(World &world) {
            std::sort(biomes.begin(), biomes.end());

            // Do not waste memory
            world.stopSavingUpdatedBlocks();
            generate(world);
            world.startSavingUpdatedBlocks();
        }
    };

}

#endif
