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
#include <optional>

#include "world_interact.hpp"
#include "../game.hpp"

namespace Terrarium {

    void WorldInteractHelper::interact(GameState &game) {
        const sf::Vector2f &mouse_pos = game.player->controls.mouse_pos;

        float found_distance_sqr = interact_distance*interact_distance;
        std::optional<sf::Vector2i> found_block;

        for (float x = mouse_pos.x - interact_distance; x < mouse_pos.x + interact_distance; x += 1) {
            for (float y = mouse_pos.y - interact_distance; y < mouse_pos.y + interact_distance; y += 1) {
                BlockDef &def = game.block_defs.getOrUnknown(
                    game.world.getBlock(floor(x), floor(y)));

                if (def.is_interactive) {
                    // Change position to be center of block.
                    sf::Vector2f pos(floor(x) + 0.5, floor(y) + 0.5);

                    float distance_sqr = (mouse_pos.x - pos.x) * (mouse_pos.x - pos.x) + (mouse_pos.y - pos.y) * (mouse_pos.y - pos.y);

                    if (distance_sqr < found_distance_sqr) {
                        found_block = sf::Vector2i(floor(x), floor(y));
                        found_distance_sqr = distance_sqr;
                    }
                }
            }
        }

        if (found_block) {
            BlockEvent *block_event = new BlockEvent();

            block_event->position = *found_block;
            block_event->user = game.player;

            game.events.emplace(Event::BlockActivate, block_event);
        }
    }

}