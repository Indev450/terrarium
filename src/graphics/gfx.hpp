#ifndef GFX_HPP
#define GFX_HPP

#include <SFML/Graphics.hpp>

#include "../utils/resource_holder.hpp"

namespace Terrarium {

    // Structure with data required to draw stuff
    struct Gfx {
        sf::Font font;

        ResourceHolder<sf::Texture> textures;
    };

}

#endif
