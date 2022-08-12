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
        float ground_gen_scale = 1.0;

        // Distance from world top
        float base_ground_height = 0.1;

        // Each x column Base height is multiplied by value from -height_amp
        // to height_amp, depending from perlin value.
        float height_amp = 0.2;

        // Same as ground_gen_scale, but for caves
        float cave_gen_scale_x = 1.0;
        float cave_gen_scale_y = 1.0;

        // ...and for biomes
        float biome_gen_scale_x = 1.0;
        float biome_gen_scale_y = 1.0;

        // If density value is higher than this, foreground filler block
        // placed. Same goes for background. Usually, background density
        // should be lower.
        float min_block_density = 0.0;
        float min_wall_density = -0.2;

        // Default filler for world, usually this is regular stone
        Tile filler = { 0, 0 };

    };

    class MapgenPerlin: public MapgenBase {
        // The noise is 3d, so for density, humidity and heat i can just
        // use different z values
        Perlin::PerlinNoise perlin;

        std::default_random_engine rng;

        std::uniform_real_distribution<float> place_decor_dist;

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
