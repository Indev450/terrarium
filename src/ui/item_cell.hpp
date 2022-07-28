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

#ifndef UI_ITEM_CELL_HPP
#define UI_ITEM_CELL_HPP

#include <optional>

#include <SFML/Graphics.hpp>

#include "../graphics/gfx.hpp"
#include "../item/item_stack.hpp"

namespace Terrarium {

    // Something like sf::RenderStates, but for ItemCellRenderer
    struct ItemCellRendererSettings {
        int grid_x = 0;
        int grid_y = 0;

        std::optional<sf::Color> text_color;

        std::optional<sf::Color> cell_color;
        std::optional<sf::Color> outline_color;

        inline void reset() {
            text_color = std::nullopt;

            cell_color = std::nullopt;
            outline_color = std::nullopt;
        }
    };

    // Utility class to draw item cells
    class ItemCellRenderer: sf::Transformable {
        static const unsigned int CELL_SIZE = 32;
        static const unsigned int SPACE = 4;

        static const int OUTLINE = 2;

        static const unsigned int COUNT_TEXT_SIZE = 16;

        const sf::Color DEF_TEXT_COLOR;

        const sf::Color DEF_CELL_COLOR;
        const sf::Color DEF_OUTLINE_COLOR;

        sf::Text count_text;

        sf::RectangleShape cell;

    public:
        // Constructoor takes default text, cell and background colors, as well
        // as gfx for game font
        ItemCellRenderer(const Gfx &gfx,
                         const sf::Color &text_color,
                         const sf::Color &cell_color,
                         const sf::Color &outline_color);

        // Returns size of rectangle, occupied by grid. Used in inventory and hotbar
        // hud elements.
        static sf::Vector2f realGridSize(unsigned int width, unsigned int height);

        // Get position of clicked cell in grid from click position
        static sf::Vector2i clickedCellPosition(const sf::Vector2f &position);

        void render(sf::RenderTarget &target,
                    const ItemStack &item_stack,
                    const sf::Transform &transform,
                    const ItemCellRendererSettings &settings);
    };

}

#endif
