#include <cmath>

#include "player.hpp"
#include "../game.hpp"

namespace Terrarium {

    void Player::update(GameState &game, float dtime) {
        if (entity_id == 0) {
            return;
        }

        std::shared_ptr<Entity> player_entity = game.entity_mgr.get(entity_id);

        if (!player_entity) {
            return;
        }

        if ((left && player_entity->speed.x < 0) || (right && player_entity->speed.x > 0))
            player_entity->physics.slippery = 1;

        if (left) {
            player_entity->speed.x = std::max(
                player_entity->speed.x - stats.acceleration * dtime,
                -stats.max_speed);
        }

        if (right) {
            player_entity->speed.x = std::min(
                player_entity->speed.x + stats.acceleration * dtime,
                stats.max_speed);
        }

        if (jump && player_entity->collision_info.blockd) {
            player_entity->speed.y = -stats.jump_force;
        }

        if (lmb && !alt_using_item) {
            if (!using_item && !hotbar[hotbar_selected]->empty()) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = player_entity;

                item_event->position = mouse_pos;

                game.events.emplace(Event::ItemUseStart, item_event);

                using_item = true;
            }
        } else if (using_item) {
            if (!hotbar[hotbar_selected]->empty()) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = player_entity;

                item_event->position = mouse_pos;

                game.events.emplace(Event::ItemUseStop, item_event);

                using_item = false;
            }
        }

        if (rmb && !using_item) {
            if (!alt_using_item && !hotbar[hotbar_selected]->empty()) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = player_entity;

                item_event->position = mouse_pos;

                game.events.emplace(Event::ItemAltUseStart, item_event);

                alt_using_item = true;
            }
        } else if (alt_using_item) {
            if (!hotbar[hotbar_selected]->empty()) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = player_entity;

                item_event->position = mouse_pos;

                game.events.emplace(Event::ItemAltUseStop, item_event);

                alt_using_item = false;
            }
        }

        if (hotbar_scroll != 0) {
            // If we were using some item, send event so we stop
            if (using_item || alt_using_item) {
                ItemEvent *item_event = new ItemEvent();

                item_event->item_stack = hotbar[hotbar_selected];

                item_event->user = player_entity;

                item_event->position = mouse_pos;

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
        sf::Vector2f result(0, 0);

        if (entity_id != 0) {
            std::shared_ptr<Entity> player_entity = game.entity_mgr.get(entity_id);

            if (player_entity) {
                result.x = player_entity->hitbox.left + player_entity->hitbox.width/2;
                result.y = player_entity->hitbox.top + player_entity->hitbox.height/2;
            }
        }

        return result;
    }

} // namespace Terrarium
