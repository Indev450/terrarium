#include <cmath>

#include "player.hpp"
#include "../game.hpp"

namespace Terrarium {

    void Player::update(GameState &game, float dtime) {
        if (entity_id == 0) {
            return;
        }

        Entity *player_entity = game.entity_mgr.get(entity_id);

        if (player_entity == nullptr) {
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

        if (lmb) {
            int block_x = floor(mouse_pos.x / Tile::SIZE);
            int block_y = floor(mouse_pos.y / Tile::SIZE);

            game.world.setBlock(block_x, block_y, 0);
        }
    }

    sf::Vector2f Player::getPosition(GameState &game) {
        sf::Vector2f result(0, 0);

        if (entity_id != 0) {
            Entity *player_entity = game.entity_mgr.get(entity_id);

            if (player_entity != nullptr) {
                result.x = player_entity->hitbox.left + player_entity->hitbox.width/2;
                result.y = player_entity->hitbox.top + player_entity->hitbox.height/2;
            }
        }

        return result;
    }

} // namespace Terrarium
