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

#include <cmath>

#include "player.hpp"
#include "../game.hpp"
#include "../utils/binary_io.hpp"

namespace Terrarium {

    // I need to "define" this const here... For some reason. C++ weird...
    const unsigned int Player::HOTBAR_SIZE; // How is this "definition"?!
    const unsigned int Player::INVENTORY_ROWS;

    void Player::update(GameState &game, float dtime) {
        bool was_on_ground = collision_info.blockd;

        Entity::update(game, dtime);

        // Stick to ground so 1-block stairs going down are not that annoying
        // when you trying to jump
        if (physics.enable_collision) {
            if (!collision_info.blockd && was_on_ground && speed.y > 0) {
                hitbox.top += 1;

                if (!isCollide(game)) {
                    hitbox.top -= 1;
                } else {
                    collide(game, false);
                }
            }
        }

        if (!controlled) {
            return;
        }

        physics.ignore_platforms = controls.down;

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

        if (collision_info.blockd)
            can_jump = 0.15;
        else
            can_jump -= dtime;

        if (controls.jump && can_jump > 0) {
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

                // Would use item stack from previously sent ItemUseStart event
                //item_event->item_stack = hotbar[hotbar_selected];

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

                // See above
                //item_event->item_stack = hotbar[hotbar_selected];

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

                // See above
                //item_event->item_stack = hotbar[hotbar_selected];

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

            if (hotbar[hotbar_selected])
                selected_item = hotbar[hotbar_selected]->getDef();
            else
                selected_item = nullptr;

            ItemEvent *item_event = new ItemEvent();

            item_event->item_stack = hotbar[hotbar_selected];

            item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

            item_event->position = controls.mouse_pos;

            game.events.emplace(Event::ItemSelect, item_event);
        } else if ((hotbar[hotbar_selected] && hotbar[hotbar_selected]->getDef() != selected_item)
                || (!hotbar[hotbar_selected] && selected_item)) {

            // If we were using some item, send event so we stop
            if (using_item || alt_using_item) {
                ItemEvent *item_event = new ItemEvent();

                // See above
                //item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

                item_event->position = controls.mouse_pos;

                game.events.emplace(using_item ? Event::ItemUseStop : Event::ItemAltUseStop, item_event);

                using_item = alt_using_item = false;
            }

            if (hotbar[hotbar_selected])
                selected_item = hotbar[hotbar_selected]->getDef();
            else
                selected_item = nullptr;

            ItemEvent *item_event = new ItemEvent();

            item_event->item_stack = hotbar[hotbar_selected];

            item_event->user = std::dynamic_pointer_cast<Player>(game.entity_mgr.get(id));

            item_event->position = controls.mouse_pos;

            game.events.emplace(Event::ItemSelect, item_event);
        }
    }

    sf::Vector2f Player::getPosition() {
        sf::Vector2f result;

        result.x = hitbox.left + hitbox.width/2;
        result.y = hitbox.top + hitbox.height/2;

        return result;
    }

    void Player::load(std::istream &s, GameState &game) {
        sf::Vector2f position;

        position.x = read<int32_t>(s);
        position.y = read<int32_t>(s);

        position = game.pixels_to_blocks.transformPoint(position);

        hitbox.left = position.x;
        hitbox.top = position.y;

        uint8_t has_hold_item_stack = read<uint8_t>(s);

        if (has_hold_item_stack != 0) {
            hold_item_stack->load(s, game);
        }

        inventory->load(s, game);

        updateHotbar();
    }

    void Player::save(std::ostream &s, GameState &game) {
        sf::Vector2f position(hitbox.left, hitbox.top);

        // For better precision
        position = game.blocks_to_pixels.transformPoint(position);

        write<int32_t>(s, position.x);
        write<int32_t>(s, position.y);

        if (hold_item_stack->empty()) {
            write<uint8_t>(s, 0);
        } else {
            write<uint8_t>(s, 1);
        }

        hold_item_stack->save(s);

        inventory->save(s);
    }

} // namespace Terrarium
