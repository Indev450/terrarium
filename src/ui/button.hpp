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

#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include <functional>

#include "element.hpp"

namespace Terrarium {

    class Button: public UIElement {
    protected:
        sf::Vector2f size;
        std::function<void(GameState&)> on_click;

    public:
        Button(const sf::Vector2f &_size, std::function<void(GameState&)> _on_click);

        bool click(GameState &game, const sf::Vector2f &position) override;
    };

}

#endif
