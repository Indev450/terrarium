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

#ifndef UI_TEXT_INPUT_HPP
#define UI_TEXT_INPUT_HPP

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace Terrarium {

    class GameState;

    class TextInput {
    protected:
        sf::String buffer;
        size_t pos = 0;

        // Can override handleEvent logic. Return true to override default, or
        // false to keep it.
        virtual bool preHandleEvent(GameState &game, const sf::Event &event) { return false; };

        virtual void onBufferUpdated(GameState &game) {};
        virtual void onPosUpdated(GameState &game) {};
    public:
        // Returns copy of underlying buffer and clears it.
        sf::String takeString();

        // Returns true if event actually was handled.
        bool handleEvent(GameState &game, const sf::Event &event);
    };

}

#endif
