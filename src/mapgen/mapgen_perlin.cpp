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

#include <cmath>

#include "mapgen_perlin.hpp"

namespace Terrarium {

    const int MapgenPerlin::DENSITY = 0;

    const int MapgenPerlin::HUMIDITY = 10;

    const int MapgenPerlin::HEAT = 20;

    MapgenPerlin::MapgenPerlin(unsigned int seed):
        perlin(seed), rng(seed)
    {}

    void MapgenPerlin::generate(World &world) {
        int base_height = world.getHeight() * settings.base_ground_height;

        // First, generate ground with caves
        for (int x = 0; x < static_cast<int>(world.getWidth()); ++x) {
            int column_height = base_height;
            column_height += base_height * (perlin.noise(x*settings.ground_gen_scale, 0, DENSITY) * settings.height_amp);

            for (int y = column_height; y < static_cast<int>(world.getHeight()); ++y) {
                float density = perlin.noise(
                    x*settings.cave_gen_scale_x,
                    y*settings.cave_gen_scale_y,
                    DENSITY);

                // Depending on density, place walls and blocks
                if (density > settings.min_wall_density) {
                    world.setWall(x, y, settings.filler.bg);
                }

                if (density > settings.min_block_density) {
                    world.setBlock(x, y, settings.filler.fg);
                }
            }
        }

        // Now process biomes
        for (int x = 0; x < static_cast<int>(world.getWidth()); ++x) {
            int column_height = base_height;
            column_height += base_height * (perlin.noise(x*settings.ground_gen_scale, 0, DENSITY) * settings.height_amp);

            for (int y = 0; y < static_cast<int>(world.getHeight()); ++y) {
                float humidity = perlin.noise(
                    x*settings.biome_gen_scale_x,
                    y*settings.biome_gen_scale_y,
                    HUMIDITY);

                float heat = perlin.noise(
                    x*settings.biome_gen_scale_x,
                    y*settings.biome_gen_scale_y,
                    HEAT);

                int depth = y - column_height;

                // Starting from biomes with less priority. Biomes with higher
                // priority will override all changes done by previous biomes.
                for (auto it = biomes.cbegin(); it != biomes.cend(); ++it) {
                    int min_depth = (world.getHeight() - column_height) * it->min_depth;
                    int max_depth = (world.getHeight() - column_height) * it->max_depth;

                    if (depth >= min_depth && depth <= max_depth &&
                        humidity > it->humidity_min && humidity < it->humidity_max &&
                        heat > it->heat_min && heat < it->heat_max) {

                        // All changes are based on initial "skeleton" world.
                        // Only non-air blocks are changed
                        if (world.getBlock(x, y) != 0) {
                            if (findAir(world, x, y, it->top_depth, FOREGROUND)) {
                                world.setBlock(x, y, it->top.fg);
                            } else if (findAir(world, x, y, it->filler_depth, FOREGROUND)) {
                                world.setBlock(x, y, it->filler.fg);
                            } else {
                                world.setBlock(x, y, it->stone.fg);
                            }
                        }

                        if (world.getWall(x, y) != 0) {
                            if (findAir(world, x, y, it->top_depth, BACKGROUND)) {
                                world.setWall(x, y, it->top.bg);
                            } else if (findAir(world, x, y, it->filler_depth, BACKGROUND)) {
                                world.setWall(x, y, it->filler.bg);
                            } else {
                                world.setWall(x, y, it->stone.bg);
                            }
                        }
                    }
                }
            }
        }

        // Ores
        for (auto it = ores.begin(); it != ores.end(); ++it) {
            // For distributing ores in world
            std::uniform_int_distribution<unsigned int> rng_int_dist(0, it->distribution);

            // for deforming ore clusters.
            std::uniform_int_distribution<int> rng_cluster_dist(-1, 1);

            int start_y = it->min_depth * world.getHeight() + rng_int_dist(rng);
            int end_y = it->max_depth * world.getHeight();

            for (int y = start_y; y < end_y; y += it->distribution + rng_int_dist(rng)) {
                for (int x = rng_int_dist(rng); x < static_cast<int>(world.getWidth()); x += it->distribution + rng_int_dist(rng)) {
                    unsigned int placed_tiles = 0;

                    int place_x = x;
                    int place_y = y;

                    while (placed_tiles < it->cluster_tiles) {
                        if (it->tile.fg != 0) {
                            if (world.getBlock(place_x, place_y) != 0) {
                                world.setBlock(place_x, place_y, it->tile.fg);
                            }
                        }

                        if (it->tile.bg != 0) {
                            if (world.getWall(place_x, place_y) != 0) {
                                world.setWall(place_x, place_y, it->tile.bg);
                            }
                        }

                        ++placed_tiles;

                        ++place_x;
                        if (place_x - x > sqrt(it->cluster_tiles)) {
                            place_x = x + rng_cluster_dist(rng);
                            ++place_y;
                        }
                    }
                }
            }
        }
    }

} // namespace Terrarium
