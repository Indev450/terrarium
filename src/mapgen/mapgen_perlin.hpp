#ifndef MAPGEN_V1_HPP
#define MAPGEN_V1_HPP

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

        static const int DENSITY;
        static const int HUMIDITY;
        static const int HEAT;

        void generate(World &world);

    public:
        MapgenPerlinSettings settings;

        MapgenPerlin(unsigned int seed);
    };

}

#endif
