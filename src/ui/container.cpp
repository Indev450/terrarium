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

#include "container.hpp"

namespace Terrarium {

    UIContainer::UIContainer(const sf::Vector2f &_size):
        shape(_size), size(_size)
    {}

    void UIContainer::setFillColor(const sf::Color &color) {
        shape.setFillColor(color);
    }

    void UIContainer::setOutlineColor(const sf::Color &color) {
        shape.setOutlineColor(color);
    }

    void UIContainer::setOutlineThickness(float thickness) {
        shape.setOutlineThickness(thickness);
    }

    void UIContainer::addElement(std::unique_ptr<UIElement> element) {
        elements.push_back(std::move(element));
    }

    bool UIContainer::click(GameState &game, const sf::Vector2f &position) {
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        if (rpos.x < 0 || rpos.x > size.x ||
            rpos.y < 0 || rpos.y > size.y) {

            return false;
        }

        bool clicked = false;

        for (auto &element: elements) {
            if (element->click(game, rpos)) {
                clicked = true;
            }
        }

        return clicked;
    }

    void UIContainer::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        sf::Transform combined_transform = getTransform() * parent_transform;

        target.draw(shape, combined_transform);

        for (auto &element: elements) {
            element->render(target, game, combined_transform);
        }
    }

}
