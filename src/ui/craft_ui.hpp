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

#ifndef UI_CRAFT_HPP
#define UI_CRAFT_HPP

#include <string>

#include "../graphics/gfx.hpp"
#include "element.hpp"
#include "item_cell.hpp"

namespace Terrarium {

    class CraftUI: public UIElement {
        std::string category;

        // How many recipes are shown on screen
        static const unsigned int SHOW_RECIPES = 7;

        // Gap between craft result and its recipe
        static constexpr float RECIPE_GAP = 4;

        ItemCellRenderer item_cell_renderer;

        unsigned int current_scroll = 0;

    public:
        CraftUI(const Gfx &gfx, const std::string &_category);

        bool click(GameState &game, const sf::Vector2f &position) override;

        bool scroll(GameState &game, const sf::Vector2f &position, float delta) override;

        void render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) override;
    };

}

#endif
