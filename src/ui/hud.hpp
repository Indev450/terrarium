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

#ifndef HUD_HPP
#define HUD_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "element.hpp"
#include "bar.hpp"

namespace Terrarium {

    class GameState;

    class Hud {
        std::unordered_map<std::string, std::unique_ptr<UIElement>> elements;
        std::unordered_map<std::string, std::unique_ptr<Bar>> bars;

    public:
        void addElement(const std::string &name, std::unique_ptr<UIElement> element);

        void setVisible(const std::string &name, bool visible);

        bool isVisible(const std::string &name);

        void addBar(const std::string &name, std::unique_ptr<Bar> bar);

        Bar *getBar(const std::string &name);

        bool click(GameState &game, const sf::Vector2f &position);

        bool scroll(GameState &game, const sf::Vector2f &position, float delta);

        void render(sf::RenderTarget &target, GameState &game);
    };

}

#endif
