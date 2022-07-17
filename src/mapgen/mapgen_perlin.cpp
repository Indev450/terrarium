/*
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

#include "mapgen_perlin.hpp"

namespace Terrarium {

    const int MapgenPerlin::DENSITY = 0;

    const int MapgenPerlin::HUMIDITY = 10;

    const int MapgenPerlin::HEAT = 20;

    MapgenPerlin::MapgenPerlin(unsigned int seed):
        perlin(seed)
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
            for (int y = 0; y < static_cast<int>(world.getHeight()); ++y) {
                float humidity = perlin.noise(
                    x*settings.biome_gen_scale_x,
                    y*settings.biome_gen_scale_y,
                    HUMIDITY);

                float heat = perlin.noise(
                    x*settings.biome_gen_scale_x,
                    y*settings.biome_gen_scale_y,
                    HEAT);

                // Starting from biomes with less priority. Biomes with higher
                // priority will override all changes done by previous biomes.
                for (auto it = biomes.cbegin(); it != biomes.cend(); ++it) {
                    if (humidity > it->humidity_min && humidity < it->humidity_max &&
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
    }

} // namespace Terrarium
