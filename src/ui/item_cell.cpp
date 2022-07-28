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

#include <cmath>

#include "item_cell.hpp"

namespace Terrarium {

    ItemCellRenderer::ItemCellRenderer(const Gfx &gfx,
                                       const sf::Color &text_color,
                                       const sf::Color &cell_color,
                                       const sf::Color &outline_color):
        DEF_TEXT_COLOR(text_color), DEF_CELL_COLOR(cell_color), DEF_OUTLINE_COLOR(outline_color)
    {
        cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setOutlineThickness(OUTLINE);

        count_text.setFont(gfx.font);
        count_text.setCharacterSize(COUNT_TEXT_SIZE);
    }

    sf::Vector2f ItemCellRenderer::realGridSize(unsigned int width, unsigned int height) {
        return sf::Vector2f(
            (CELL_SIZE * width) + (width+1) * SPACE,
            (CELL_SIZE * height) + (height+1) * SPACE);
    }

    sf::Vector2i ItemCellRenderer::clickedCellPosition(const sf::Vector2f &position) {
        return sf::Vector2i(
            static_cast<int>(floor(position.x / (CELL_SIZE + SPACE))),
            static_cast<int>(floor(position.y / (CELL_SIZE + SPACE))));
    }

    void ItemCellRenderer::render(sf::RenderTarget &target,
                                  const ItemStack &item_stack,
                                  const sf::Transform &transform,
                                  const ItemCellRendererSettings &settings) {

        sf::Vector2f cell_position(
            (CELL_SIZE * settings.grid_x) + (settings.grid_x+1) * SPACE,
            (CELL_SIZE * settings.grid_y) + (settings.grid_y+1) * SPACE);

        count_text.setFillColor(settings.text_color.value_or(DEF_TEXT_COLOR));

        cell.setFillColor(settings.cell_color.value_or(DEF_CELL_COLOR));
        cell.setOutlineColor(settings.outline_color.value_or(DEF_OUTLINE_COLOR));

        cell.setPosition(cell_position);

        target.draw(cell, transform);

        if (item_stack.empty()) {
            return;
        }

        std::shared_ptr<ItemDef> item_def = item_stack.getDef();

        item_def->inventory_image.setPosition(cell_position);

        target.draw(item_def->inventory_image, transform);

        if (item_stack.getCount() > 1) {
            count_text.setString(std::to_string(item_stack.getCount()));
            count_text.setPosition(cell_position);
            target.draw(count_text, transform);
        }
    }

}
