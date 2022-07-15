#ifndef MAPGEN_BASE_HPP
#define MAPGEN_BASE_HPP

#include <vector>
#include <algorithm>

#include "../tile/tile.hpp"
#include "../world/world.hpp"

namespace Terrarium {

    struct Biome {
        float humidity_min = -1.0;
        float humidity_max = 1.0;

        float heat_min = -1.0;
        float heat_max = 1.0;

        Tile top = { 0, 0 };
        unsigned int top_depth = 1;

        Tile filler = { 0, 0 };
        unsigned int filled_depth = 1;

        Tile stone = { 0, 0 };

        // Lowest number -> lowest priority. Biomes with lowest priority generated
        // first, then they "overriden" by higher priority biomes.
        unsigned int priority = 0;

        // for sorting
        bool operator<(const Biome &other) {
            return priority < other.priority;
        }
    };

    class MapgenBase {
    protected:
        std::vector<Biome> biomes;

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
        void addBiome(const Biome &biome) {
            biomes.push_back(biome);
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
