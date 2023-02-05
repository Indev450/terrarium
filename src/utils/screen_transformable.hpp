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

#include <iostream>

#include <SFML/Graphics.hpp>

class ScreenTransformable: sf::Transformable {
public:
    enum Origin {
        TopLeft,
        ScreenCenter,
    };

private:
    sf::Vector2f position;

    // This is probably bad, but it works
    sf::Vector2f screen_size;

    Origin origin = TopLeft;

public:
    void setScreenSize(const sf::Vector2f &size) {
        screen_size = size;

        updatePosition();
    }

    void setOriginType(Origin new_origin) {
        origin = new_origin;

        updatePosition();
    }

    void updatePosition() {
        switch (origin) {
            case TopLeft:
            {
                sf::Vector2f new_position = position;

                if (position.x < 0) {
                    new_position.x += screen_size.x;
                }

                if (position.y < 0) {
                    new_position.y += screen_size.y;
                }

                sf::Transformable::setPosition(new_position);
            }
            break;

            case ScreenCenter:
                sf::Transformable::setPosition(
                    screen_size.x / 2 + position.x,
                    screen_size.y / 2 + position.y);
            break;
        }
    }

    void setPosition(const sf::Vector2f &position) {
        this->position = position;

        updatePosition();
    }

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;

        updatePosition();
    }

    const sf::Vector2f &getPosition() {
        return position;
    }

    const sf::Transform &getTransform() const {
        return sf::Transformable::getTransform();
    }

    const sf::Transform &getInverseTransform() const {
        return sf::Transformable::getInverseTransform();
    }

};

#endif
