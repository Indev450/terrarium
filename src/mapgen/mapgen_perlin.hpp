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

#ifndef MAPGEN_V1_HPP
#define MAPGEN_V1_HPP

#include <random>

#include "mapgen_base.hpp"
#include "../lib/perlin/perlin.hpp"

namespace Terrarium {

    struct MapgenPerlinSettings {
        // Each x coordinate multiplied by this value when passed into
        // noise function (usually it very small value, like 1/world_width.
        // Scale depending on world size isn't very good though)
        double ground_gen_scale = 1.0;

        // Distance from world top
        double base_ground_height = 0.1;

        // Each column of blocks height is offset by this value multiplied by
        // perlin noise value
        int height_amp = 50;

        // Same as ground_gen_scale, but for caves
        double cave_gen_scale_x = 1.0;
        double cave_gen_scale_y = 1.0;

        // ...and for biomes
        double biome_gen_scale_x = 1.0;
        double biome_gen_scale_y = 1.0;

        // If density value is outside of this range, solid block is placed
        double min_block_density = 0.0;
        double max_block_density = 0.2;

        // Minimal value for density factor. Density factor increases with
        // depth, so at surface level it is quite small, so to avoid 1-block
        // caves, this minimum value picked if actual value is too small.
        double min_density_factor = 0.5;
        double max_density_factor = 0.5;

        // Same, but for scaling caves
        double min_cave_scale_factor = 0.5;
        double max_cave_scale_factor = 1.0;

        // Default filler for world, usually this is regular stone
        Tile filler = { 0, 0 };

    };

    class MapgenPerlin: public MapgenBase {
        // The noise is 3d, so for density, humidity and heat i can just
        // use different z values
        Perlin::PerlinNoise perlin;

        std::default_random_engine rng;

        std::uniform_real_distribution<double> place_decor_dist;

        static const int DENSITY;
        static const int HUMIDITY;
        static const int HEAT;

        void generate(World &world) override;

    public:
        MapgenPerlinSettings settings;

        MapgenPerlin(unsigned int seed);

        void setFiller(const Tile &filler) override {
            settings.filler = filler;
        }
    };

}

#endif
