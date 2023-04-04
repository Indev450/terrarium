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

#ifndef UI_INVENTORY_HPP
#define UI_INVENTORY_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "element.hpp"
#include "item_cell.hpp"
#include "../game.hpp"
#include "../item/inventory.hpp"
#include "../graphics/gfx.hpp"

namespace Terrarium {

    class InventoryUI: public UIElement {
        const sf::Color BG_COLOR;

        unsigned int width; // Item cells in row
        unsigned int height; // rows count

        ItemCellRenderer item_cell_renderer;

        sf::RectangleShape background;
    public:
        std::shared_ptr<Inventory> inventory;

        InventoryUI(const Gfx &gfx, unsigned int _width, unsigned int _height);

        std::optional<std::string> hover(GameState &game, const sf::Vector2f &position) override;

        bool click(GameState &game, const sf::Vector2f &position) override;

        void render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) override;
    };

}

#endif
