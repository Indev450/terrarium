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

#include "rect_button.hpp"

namespace Terrarium {

    RectButton::RectButton(Gfx &gfx, const sf::Vector2f &size,
                           std::function<void(GameState&)> on_click,
                           const std::string &text_string,
                           unsigned int text_size):
        Button(size, on_click), shape(size), text(text_string, gfx.font, text_size)
    { resetTextPosition(); }

    void RectButton::setTextString(const std::string &text_string) {
        text.setString(text_string);

        resetTextPosition();
    }

    void RectButton::setTextColor(const sf::Color &color) {
        text.setFillColor(color);
    }

    void RectButton::setBackgroundColor(const sf::Color &color) {
        shape.setFillColor(color);
    }

    void RectButton::setOutlineColor(const sf::Color &color) {
        shape.setOutlineColor(color);
    }

    void RectButton::setOutlineThickness(float thickness) {
        shape.setOutlineThickness(thickness);
    }

    void RectButton::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        sf::Transform combined_transform = getTransform() * parent_transform;

        target.draw(shape, combined_transform);
        target.draw(text, combined_transform);
    }

    void RectButton::resetTextPosition() {
        sf::FloatRect text_bounds = text.getLocalBounds();

        text.setOrigin(text_bounds.width / 2, text_bounds.height / 2);

        text.setPosition(size.x / 2, size.y / 2);
    }

}
