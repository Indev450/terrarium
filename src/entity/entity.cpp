#include <cmath>

#include "entity.hpp"
#include "../game.hpp"

namespace Terrarium {

    void Entity::update(GameState &game, float dtime) {
        anims.update(dtime);

        if (!collision_info.blockd) {
            speed.y += physics.gravity * dtime;
        } else {
            speed.x *= physics.slippery;
        }

        // FIXME - skipping blocks when speed.x * dtime > 1
        hitbox.left += speed.x * dtime;
        if (physics.enable_collision) {
            collide(game, true);
        }

        // FIXME - ^
        hitbox.top += speed.y * dtime;
        if (physics.enable_collision) {
            collide(game, false);
        }
    }

    void Entity::collide(GameState &game, bool by_x) {
        if (by_x) {
            collision_info.blockl = false;
            collision_info.blockr = false;
        } else {
            collision_info.blocku = false;
            collision_info.blockd = false;
        }

        int y_min = floor(hitbox.top);
        float y_max = hitbox.top + hitbox.height;

        int x_min = floor(hitbox.left);
        float x_max = hitbox.left + hitbox.width;

        for (int y = y_min; y < y_max; ++y) {
            for (int x = x_min; x < x_max; ++x) {
                blockid block = game.world.getBlock(x, y);

                if (block == 0) {
                    continue;
                }

                BlockDef &block_def = game.block_defs.getOrUnknown(block);

                if (block_def.is_solid) {

                    if (by_x) {

                        if (speed.x > 0) {
                            hitbox.left = x - hitbox.width;
                            collision_info.blockr = true;
                        } else {
                            hitbox.left = x + 1;
                            collision_info.blockl = true;
                        }

                        speed.x = 0;
                        return;

                    } else {

                        if (speed.y > 0) {
                            hitbox.top = y - hitbox.height;
                            collision_info.blockd = true;

                            physics.slippery = block_def.slippery;
                        } else {
                            hitbox.top = y + 1;
                            collision_info.blocku = true;
                        }

                        speed.y = 0;
                        return;

                    }

                }
            }
        }
    }

} // namespace Terrarium
