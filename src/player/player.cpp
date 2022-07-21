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

#include "player.hpp"
#include "../game.hpp"

namespace Terrarium {

    // I need to "define" this const here... For some reason. C++ weird...
    const unsigned int Player::HOTBAR_SIZE; // How is this "definition"?!
    const unsigned int Player::INVENTORY_ROWS;

    void Player::update(GameState &game, float dtime) {
        Entity::update(game, dtime);

        if ((controls.left && speed.x < 0) || (controls.right && speed.x > 0))
            physics.slippery = 1;

        if (controls.left) {
            speed.x = std::max(
                speed.x - stats.acceleration * dtime,
                -stats.max_speed);
        }

        if (controls.right) {
            speed.x = std::min(
                speed.x + stats.acceleration * dtime,
                stats.max_speed);
        }

        if (controls.jump && collision_info.blockd) {
            speed.y = -stats.jump_force;
        }

        if (controls.lmb && !alt_using_item) {
            if (!using_item && !hotbar[hotbar_selected]->empty()) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                // Can't pass `this` because that will make multiple unrelated
                // smart pointers, which can cause segfault
                item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

                item_event->position = controls.mouse_pos;

                game.events.emplace(Event::ItemUseStart, item_event);

                using_item = true;
            }
        } else if (using_item) {
            if (!hotbar[hotbar_selected]->empty()) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

                item_event->position = controls.mouse_pos;

                game.events.emplace(Event::ItemUseStop, item_event);

                using_item = false;
            }
        }

        if (controls.rmb && !using_item) {
            if (!alt_using_item && !hotbar[hotbar_selected]->empty()) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

                item_event->position = controls.mouse_pos;

                game.events.emplace(Event::ItemAltUseStart, item_event);

                alt_using_item = true;
            }
        } else if (alt_using_item) {
            if (!hotbar[hotbar_selected]->empty()) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

                item_event->position = controls.mouse_pos;

                game.events.emplace(Event::ItemAltUseStop, item_event);

                alt_using_item = false;
            }
        }

        if (hotbar_scroll != 0) {
            // If we were using some item, send event so we stop
            if (using_item || alt_using_item) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

                item_event->position = controls.mouse_pos;

                game.events.emplace(using_item ? Event::ItemUseStop : Event::ItemAltUseStop, item_event);

                using_item = alt_using_item = false;
            }

            // Then we can safely scroll
            if (hotbar_scroll > 0) {
                ++hotbar_selected;

                if (hotbar_selected == HOTBAR_SIZE) {
                    hotbar_selected = 0;
                }

            } else {
                if (hotbar_selected == 0) {
                    hotbar_selected = HOTBAR_SIZE;
                }

                --hotbar_selected;
            }

            hotbar_scroll = 0;

            ItemEvent *item_event = new ItemEvent();

            item_event->item_stack = hotbar[hotbar_selected];

            item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

            item_event->position = controls.mouse_pos;

            game.events.emplace(Event::ItemSelect, item_event);
        }
    }

    sf::Vector2f Player::getPosition(GameState &game) {
        sf::Vector2f result;

        result.x = hitbox.left + hitbox.width/2;
        result.y = hitbox.top + hitbox.height/2;

        return result;
    }

} // namespace Terrarium
