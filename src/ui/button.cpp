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

#include "button.hpp"
#include "../game.hpp"

namespace Terrarium {

    Button::Button(const sf::Vector2f &_size):
        size(_size)
    {}

    bool Button::click(GameState &game, const sf::Vector2f &position) {
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        if (rpos.x < 0 || rpos.x > size.x || rpos.y < 0 || rpos.y > size.y) {
            return false;
        }

        ready = true;

        return true;
    }

    bool Button::isReady() {
        return ready;
    }

    std::string Button::getValue() {
        if (ready) {
            ready = false;

            return "pressed";
        }

        return "";
    }
}
