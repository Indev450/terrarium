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

#ifndef UI_CONTAINER_HPP
#define UI_CONTAINER_HPP

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "element.hpp"

namespace Terrarium {

    class UIContainer: public UIElement {
        sf::RectangleShape shape;
        sf::Vector2f size;

        std::vector<std::unique_ptr<UIElement>> elements;

    public:
        UIContainer(const sf::Vector2f &_size);

        void setFillColor(const sf::Color &color);
        void setOutlineColor(const sf::Color &color);
        void setOutlineThickness(float thickness);

        void addElement(std::unique_ptr<UIElement> element);

        bool click(GameState &game, const sf::Vector2f &position) override;

        void render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) override;

    };

}

#endif
