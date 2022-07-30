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

#ifndef UTIL_SCREEN_TRANSFORMABLE_HPP
#define UTIL_SCREEN_TRANSFORMABLE_HPP

#include <SFML/Graphics.hpp>

class ScreenTransformable: sf::Transformable {
    // Relative position. Positive coords mean offset from top left corner,
    // negative - offset from bottom right corner
    sf::Vector2f position;

public:
    void setScreenSize(const sf::Vector2f &size) {
        sf::Vector2f new_position = position;

        if (position.x < 0) {
            new_position.x += size.x;
        }

        if (position.y < 0) {
            new_position.y += size.y;
        }

        sf::Transformable::setPosition(new_position);
    }

    void setPosition(const sf::Vector2f &position) {
        this->position = position;
    }

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    const sf::Transform &getTransform() const {
        return sf::Transformable::getTransform();
    }

    const sf::Transform &getInverseTransform() const {
        return sf::Transformable::getInverseTransform();
    }

};

#endif
