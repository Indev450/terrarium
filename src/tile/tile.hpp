#ifndef TILE_HPP
#define TILE_HPP

#include "block_def.hpp"

namespace Terrarium {

    struct Tile {
        static const int SIZE = 16;

        blockid fg;
        blockid bg;
    };

}

#endif
