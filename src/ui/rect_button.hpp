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

#ifndef UI_RECT_BUTTON_HPP
#define UI_RECT_BUTTON_HPP

#include <SFML/Graphics.hpp>

#include "button.hpp"
#include "../graphics/gfx.hpp"

namespace Terrarium {

    // Button with simple rectangle and text as visual
    class RectButton: public Button {
        sf::RectangleShape shape;
        sf::Text text;

        void resetTextPosition();

    public:
        RectButton(Gfx &gfx,
                   const sf::Vector2f &size, std::function<void(GameState&)> on_click,
                   const std::string &text_string, unsigned int text_size);

        void setTextString(const std::string &text_string);
        void setTextColor(const sf::Color &color);

        void setBackgroundColor(const sf::Color &color);
        void setOutlineColor(const sf::Color &color);

        void setOutlineThickness(float thickness);

        void render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) override;
    };

}

#endif
