#include <cmath>

#include "player.hpp"
#include "../game.hpp"

namespace Terrarium {

    // I need to "define" this const here... For some reason. C++ weird...
    const unsigned int Player::HOTBAR_SIZE; // How is this "definition"?!

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
        }
    }

    sf::Vector2f Player::getPosition(GameState &game) {
        sf::Vector2f result;

        result.x = hitbox.left + hitbox.width/2;
        result.y = hitbox.top + hitbox.height/2;

        return result;
    }

} // namespace Terrarium
