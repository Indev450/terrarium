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

#ifndef UI_HOTBAR_HPP
#define UI_HOTBAR_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "element.hpp"
#include "../game.hpp"
#include "../item/item_stack.hpp"
#include "../graphics/gfx.hpp"

namespace Terrarium {

    class HotbarRenderer: public UIElement {
        static const unsigned int CELL_SIZE = 32;
        static const unsigned int SPACE = 4;

        static const int OUTLINE = 2;

        // I'd like to make this static const, but stupid compiler doesn't let me
        // to do that :blobcatreeeeeee:
        const sf::Color BG_COLOR;
        const sf::Color CELL_COLOR;
        const sf::Color SELECTED_CELL_COLOR;

        unsigned int items_count;

        sf::RectangleShape background;
        sf::RectangleShape cell;

        sf::Text count_text;
    public:
        HotbarRenderer(unsigned int _items_count, const Gfx &gfx);

        bool click(GameState &game, const sf::Vector2f &position) override;

        void render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) override;
    };

}

#endif
