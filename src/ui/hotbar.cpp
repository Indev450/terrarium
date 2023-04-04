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

#include <string>
#include <algorithm>
#include <cmath>

#include "hotbar.hpp"

namespace Terrarium {

    HotbarRenderer::HotbarRenderer(const Gfx &gfx, unsigned int _items_count):
        SELECTED_CELL_COLOR(64, 196, 196), items_count(_items_count),
        item_cell_renderer(gfx, sf::Color::White, sf::Color::Transparent, sf::Color(127, 127, 127))
    {
        background.setSize(item_cell_renderer.realGridSize(items_count, 1));
        background.setFillColor(sf::Color(196, 196, 196, 127));
    }

    bool HotbarRenderer::click(GameState &game, const sf::Vector2f &position) {
        // Calculate relative position
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        sf::Vector2f size = item_cell_renderer.realGridSize(items_count, 1);

        // Click out of bounds
        if (rpos.x < 0 || rpos.x > size.x ||
            rpos.y < 0 || rpos.y > size.y) {

            return false;
        }

        unsigned int clicked_index = item_cell_renderer.clickedCellPosition(rpos).x;

        auto clicked_item = game.player->getHotbar()[clicked_index];

        if (game.player->hold_item_stack->canMerge(*clicked_item) &&
            !game.player->hold_item_stack->empty() &&
            !clicked_item->empty()) {

            clicked_item->merge(*game.player->hold_item_stack);
        } else {
            game.player->hold_item_stack->swap(*clicked_item);
        }

        return true;
    }

    void HotbarRenderer::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        sf::Transform combined_transform = getTransform() * parent_transform;

        target.draw(background, combined_transform);

        std::shared_ptr<ItemStack> *hotbar = game.player->getHotbar();

        ItemCellRendererSettings settings;

        for (unsigned int i = 0; i < game.player->HOTBAR_SIZE; ++i) {
            settings.reset();

            if (i == game.player->getHotbarSelected()) {
                settings.outline_color = SELECTED_CELL_COLOR;
            }

            item_cell_renderer.render(target, *hotbar[i], combined_transform, settings);

            settings.grid_x++;
        }
    }

}
