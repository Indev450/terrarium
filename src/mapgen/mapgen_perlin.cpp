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

#include <cmath>
#include <utility>
#include <optional>

#include "mapgen_perlin.hpp"
#include "../utils/math.hpp"

namespace Terrarium {

    const int MapgenPerlin::DENSITY = 0;

    const int MapgenPerlin::HUMIDITY = 10;

    const int MapgenPerlin::HEAT = 20;

    MapgenPerlin::MapgenPerlin(unsigned int seed):
        perlin(seed), rng(seed), place_decor_dist(0, 1)
    {}

    float MapgenPerlin::sampleSurface(float x) {
        float result = perlin.noise(x*settings.ground_gen_scale, 0, DENSITY);

        for (unsigned i = 1; i < 8; ++i) {
            result = combine_noise(result, perlin.noise(x*settings.ground_gen_scale/i, 0, DENSITY-i));
        }

        return perlin.noise(x*settings.ground_gen_scale, 0, DENSITY);
    }

    float MapgenPerlin::sampleDensity(float x, float y) {
        // Makes caves less common on surface
        double density_check_factor = double(y)/height;

        // Don't make it way too small so we don't have 1 block size
        // caves or around that
        double cave_scale_factor = rescale(density_check_factor, 0., 1., settings.min_cave_scale_factor, settings.max_cave_scale_factor);
        density_check_factor = rescale(density_check_factor, 0., 1., settings.min_density_factor, settings.max_density_factor);

        double result = perlin.noise(x*settings.cave_gen_scale_x, y*settings.cave_gen_scale_y*cave_scale_factor, DENSITY);

        for (unsigned i = 1; i < 4; ++i) {
            result = combine_noise(result, perlin.noise(x*settings.cave_gen_scale_x/i, y*settings.cave_gen_scale_y*cave_scale_factor/i, DENSITY-i));
        }

        return result;
    }

    float MapgenPerlin::sampleHumidity(float x, float y) {
        return perlin.noise(
                    x*settings.biome_gen_scale_x,
                    y*settings.biome_gen_scale_y,
                    HUMIDITY);
    }

    float MapgenPerlin::sampleHeat(float x, float y) {
        return perlin.noise(
                    x*settings.biome_gen_scale_x,
                    y*settings.biome_gen_scale_y,
                    HEAT);
    }

    void MapgenPerlin::generate(World &world) {
        width = world.getWidth();
        height = world.getHeight();

        int base_height = world.getHeight() * settings.base_ground_height;

        // First, generate ground with caves
        for (int x = 0; x < static_cast<int>(world.getWidth()); ++x) {
            int column_height = base_height;
            column_height += base_height + sampleSurface(x) * settings.height_amp;

            for (int y = column_height; y < static_cast<int>(world.getHeight()); ++y) {
                world.setWall(x, y, settings.filler.bg.id);

                // Makes caves less common on surface
                double density_check_factor = double(y)/world.getHeight();

                // Don't make it way too small so we don't have 1 block size
                // caves or around that
                density_check_factor = rescale(density_check_factor, 0., 1., settings.min_density_factor, settings.max_density_factor);

                double density = sampleDensity(x, y);

                double density_space = settings.max_block_density - settings.min_block_density;
                double density_avg = (settings.max_block_density + settings.min_block_density) / 2;
                double min_density = density_avg - density_space/2*density_check_factor;
                double max_density = density_avg + density_space/2*density_check_factor;


                // Depending on density, place walls and blocks
                //
                // Is the condition non-intuitive? Maybe, dunno how do i rename
                // settings then...
                if (density < min_density || density > max_density) {
                    world.setBlock(x, y, settings.filler.fg.id);
                }
            }
        }

        // Now process biomes

        // Decorations to place, to avoid placing decorations on other decorations
        std::vector<std::pair<sf::Vector2i, std::shared_ptr<Decoration>>> decor_to_place;

        for (int x = 0; x < static_cast<int>(world.getWidth()); ++x) {
            int column_height = base_height;
            column_height += base_height + sampleSurface(x) * settings.height_amp;

            for (int y = 0; y < static_cast<int>(world.getHeight()); ++y) {
                double humidity = sampleHumidity(x, y);

                double heat = sampleHeat(x, y);

                int depth = y - column_height;

                std::optional<std::pair<sf::Vector2i, std::shared_ptr<Decoration>>> place_decor;

                // Starting from biomes with less priority. Biomes with higher
                // priority will override all changes done by previous biomes.
                for (auto it = biomes.cbegin(); it != biomes.cend(); ++it) {
                    int min_depth = (world.getHeight() - column_height) * it->min_depth;
                    int max_depth = (world.getHeight() - column_height) * it->max_depth;

                    if (depth >= min_depth && depth <= max_depth &&
                        humidity > it->humidity_min && humidity < it->humidity_max &&
                        heat > it->heat_min && heat < it->heat_max) {

                        // In case previous biome tried to place decoration
                        place_decor = std::nullopt;

                        if (world.getBlock(x, y) == 0) {
                            for (auto &decor: it->decorations) {
                                if (place_decor_dist(rng) < decor->place_chance && decor->canPlace(world, x, y)) {
                                    place_decor = std::make_pair(sf::Vector2i(x, y), decor);
                                    break;
                                }
                            }
                        }

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

                if (place_decor) {
                    decor_to_place.push_back(*place_decor);
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

        // Place decorations only after everything else placed
        for (auto &pair: decor_to_place) {
            // This check fixes overlaps
            if (pair.second->canPlace(world, pair.first.x, pair.first.y)) {
                pair.second->place(world, pair.first.x, pair.first.y);
            }
        }
    }

} // namespace Terrarium
