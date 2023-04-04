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

#ifndef MAPGEN_BASE_HPP
#define MAPGEN_BASE_HPP

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <optional>

#include "../tile/tile.hpp"
#include "../world/world.hpp"

namespace Terrarium {

    namespace PlaceConditions {
        struct Condition {
            virtual bool satisfied(blockid block) = 0;
        };

        struct AnySolidBlock: public Condition {
            bool satisfied(blockid block) override {
                return block != 0;
            }
        };

        struct AirBlock: public Condition {
            bool satisfied(blockid block) override {
                return block == 0;
            }
        };

        struct AnyOfBlocks: public Condition {
            std::unordered_set<blockid> blocks;

            AnyOfBlocks(const std::unordered_set<blockid> &_blocks):
                blocks(_blocks)
            {}

            bool satisfied(blockid block) override {
                return blocks.count(block) != 0;
            }
        };

        struct NoneOfBlocks: public Condition {
            std::unordered_set<blockid> blocks;

            NoneOfBlocks(const std::unordered_set<blockid> &_blocks):
                blocks(_blocks)
            {}

            bool satisfied(blockid block) override {
                return blocks.count(block) == 0;
            }
        };

        struct ExactBlock: public Condition {
            blockid block;

            ExactBlock(blockid _block):
                block(_block)
            {}

            bool satisfied(blockid block) override {
                return block == this->block;
            }
        };
    }

    struct TileCondition {
        sf::Vector2i position;

        std::unique_ptr<PlaceConditions::Condition> fg;
        std::unique_ptr<PlaceConditions::Condition> bg;

        bool satisfied(const Tile &tile) const {
            if (fg != nullptr) {
                if (!fg->satisfied(tile.fg)) {
                    return false;
                }
            }

            if (bg != nullptr) {
                if (!bg->satisfied(tile.bg)) {
                    return false;
                }
            }

            return true;
        }
    };

    // Trees, grass, etc
    struct Decoration {
        sf::Vector2i origin;

        uint16_t width;
        uint16_t height;

        std::vector<Tile> tiles;

        float place_chance = 0.5;

        std::vector<TileCondition> conditions;

        std::optional<Tile> getLocalTile(int x, int y) const;

        // Checks if decoration can be placed without replacing other blocks
        bool canPlace(const World &world, int x, int y) const;

        // Places decoration into the world. This function does NOT checks
        // if it can be placed or not. That lies on the mapgen
        void place(World &world, int x, int y) const;
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
