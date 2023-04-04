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

#ifndef HUD_HPP
#define HUD_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <optional>
#include <list>

#include <SFML/Graphics.hpp>

#include "element.hpp"
#include "bar.hpp"

namespace Terrarium {

    class GameState;

    class Hud {
        std::unordered_map<std::string, std::unique_ptr<UIElement>> elements;
        std::unordered_map<std::string, std::unique_ptr<Bar>> bars;

        std::list<std::string> focused_elements;

        sf::Vector2f screen_size;

    public:
        std::optional<std::string> tip;

    public:
        // Has to do custom logic when screen size changed so thats a setter
        void setScreenSize(const sf::Vector2f &size);
        sf::Vector2f getScreenSize();

        void addElement(const std::string &name, std::unique_ptr<UIElement> element, bool focus = true);

        void replaceElement(const std::string &name, std::unique_ptr<UIElement> element, bool focus = true);

        void setVisible(const std::string &name, bool visible, bool focus = true);

        bool isVisible(const std::string &name);

        bool hideFocused();

        UIElement *getElement(const std::string &name);

        void delElement(const std::string &name);

        void addBar(const std::string &name, std::unique_ptr<Bar> bar);

        Bar *getBar(const std::string &name);

        void delBar(const std::string &name);

        void hover(GameState &game, const sf::Vector2f &position);

        bool click(GameState &game, const sf::Vector2f &position);

        bool scroll(GameState &game, const sf::Vector2f &position, float delta);

        void render(sf::RenderTarget &target, GameState &game);
    };

}

#endif
