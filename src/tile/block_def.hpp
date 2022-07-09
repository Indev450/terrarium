#ifndef BLOCK_DEF_HPP
#define BLOCK_DEF_HPP

#include <cstdint>

#include <SFML/Graphics.hpp>

namespace Terrarium {

    typedef uint16_t blockid;

    struct BlockDef {
        sf::Sprite sprite;

        bool is_solid = true;
        float slippery = 0;
    };

} // namespace Terrarium

#endif
