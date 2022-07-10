#ifndef ITEM_DEF_HPP
#define ITEM_DEF_HPP

#include <string>
#include <cstdint>

#include <SFML/Graphics.hpp>

namespace Terrarium {

    struct ItemDef {
        std::string name = "default:name";
        std::string description = "Default description";

        sf::Sprite inventory_image;

        uint16_t max_count = 1;
    };

}

#endif
