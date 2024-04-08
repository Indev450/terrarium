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

#include "../game.hpp"
#include "physical_body.hpp"

namespace Terrarium {

    void PhysicalBody::update(GameState &game, float dtime) {
        hitbox_prev = hitbox;

        // When collision gets disabled, those fields may be true, so we need to
        // fix that
        if (!physics.enable_collision) {
            collision_info.blocku = false;
            collision_info.blockd = false;
            collision_info.blockl = false;
            collision_info.blockr = false;
        }

        speed.y += physics.gravity * dtime;

        if (collision_info.blockd && speed.y >= 0) {
            speed.x *= physics.slippery;
        }

        bool collision_occured = false;
        float dx = speed.x * dtime;
        while (dx != 0 && !collision_occured) {
            float step = std::min(1.f, std::max(-1.f, dx));
            dx -= step;

            hitbox.left += step;
            if (physics.enable_collision) {
                collision_occured = isCollide(game);

                collide(game, true);
            }
        }

        collision_occured = false;
        float dy = speed.y * dtime;
        while (dy != 0 && !collision_occured) {
            float step = std::min(1.f, std::max(-1.f, dy));
            dy -= step;

            hitbox.top += step;
            if (physics.enable_collision) {
                collision_occured = isCollide(game);

                collide(game, false);
            }
        }
    }

    void PhysicalBody::collide(GameState &game, bool by_x) {
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
                bool solid = true;
                float slippery = 1;

                if (game.world.isInRange(x, y)) {
                    blockid block = game.world.getBlock(x, y);

                    if (block == 0) {
                        continue;
                    }

                    BlockDef &block_def = game.block_defs.getOrUnknown(block);

                    solid = block_def.is_solid;

                    if (block_def.is_platform) {
                        solid = solid and hitbox_prev.top + hitbox.height <= y and not physics.ignore_platforms;
                    }

                    slippery = block_def.slippery;
                }

                if (solid) {

                    if (by_x) {
                        if (y + 1 >= y_max) {
                            double dy = std::min(hitbox.height/2, 1.f);
                            hitbox.top -= dy;

                            if (isCollide(game)) {
                                hitbox.top += dy;
                            } else {
                                continue;
                            }
                        }

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

                            physics.slippery = slippery;
                        } else {
                            hitbox.top = y + 1;
                            collision_info.blocku = true;
                        }

                        if (speed.y < 0) {
                            speed.y = 0;
                        } else {
                            // Maybe not the best way to do this, but it fixes
                            // bug when collision_info.blockd blinks every frame
                            // instead of staying true when entity stands on the
                            // ground.
                            speed.y = 0.01;
                        }

                        return;

                    }

                }
            }
        }
    }

    bool PhysicalBody::isCollide(GameState &game) {
        int y_min = floor(hitbox.top);
        float y_max = hitbox.top + hitbox.height;

        int x_min = floor(hitbox.left);
        float x_max = hitbox.left + hitbox.width;

        for (int y = y_min; y < y_max; ++y) {
            for (int x = x_min; x < x_max; ++x) {
                bool solid = true;

                if (game.world.isInRange(x, y)) {
                    blockid block = game.world.getBlock(x, y);

                    if (block == 0) {
                        continue;
                    }

                    BlockDef &block_def = game.block_defs.getOrUnknown(block);

                    solid = block_def.is_solid;

                    if (block_def.is_platform) {
                        solid = solid and hitbox_prev.top + hitbox.height <= y and not physics.ignore_platforms;
                    }
                }

                if (solid) {
                    return true;
                }
            }
        }

        return false;
    }

}
