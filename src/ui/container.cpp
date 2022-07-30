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

#include <stdexcept>

#include "container.hpp"

namespace Terrarium {

    UIContainer::UIContainer(const sf::Vector2f &_size, float _scroll_max):
        shape(_size), size(_size), scroll_max(_scroll_max)
    {
        if (!rtexture.create(
                static_cast<unsigned int>(size.x),
                static_cast<unsigned int>(size.y))) {
            throw std::runtime_error("Terrarium::UIContainer: failed to create render texture");
        }
    }

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
        element->setScreenSize(size); // Screen size for nested elements is container size
        elements.push_back(std::move(element));
    }

    bool UIContainer::click(GameState &game, const sf::Vector2f &position) {
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        rpos.y -= scroll_current;

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

    bool UIContainer::scroll(GameState &game, const sf::Vector2f &position, float delta) {
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        rpos.y -= scroll_current;

        if (rpos.x < 0 || rpos.x > size.x ||
            rpos.y < 0 || rpos.y > size.y) {

            return false;
        }

        for (auto &element: elements) {
            if (element->scroll(game, rpos, delta)) {
                return true;
            }
        }

        scroll_current = std::min(scroll_max, std::max(scroll_current + delta * SCROLL_SPEED, 0.f));

        return true;
    }

    void UIContainer::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        // I think using render texture is not most optimal way to cut off
        // elements that are out of bounds... But it seems sfml doesn't have
        // another option
        rtexture.clear(sf::Color(0, 0, 0, 0));

        sf::Transform transform = shape.getTransform().getInverse();

        rtexture.draw(shape, transform);

        transform.translate(0, scroll_current);

        for (auto &element: elements) {
            element->render(rtexture, game, transform);
        }

        transform = getTransform() * parent_transform;

        rtexture.display();

        sprite.setTexture(rtexture.getTexture());

        target.draw(sprite, transform);
    }

}
