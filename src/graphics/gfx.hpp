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

#ifndef GFX_HPP
#define GFX_HPP

#include <SFML/Graphics.hpp>

#include "../utils/resource_holder.hpp"

namespace Terrarium {

    class TextureHolder: public ResourceHolder<sf::Texture> {
        sf::RenderTexture combine_temp;
        sf::Sprite combine_temp_sprite;

    public:
        // For textures that have + in name will create a "combine texture"
        sf::Texture &get(const std::string &desc);
    };

    // Structure with data required to draw stuff
    struct Gfx {
        sf::Font font;

        TextureHolder textures;
    };

}

#endif
