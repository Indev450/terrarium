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

#ifndef PHYSICAL_BODY_HPP
#define PHYSICAL_BODY_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace Terrarium {

    class GameState;

    struct CollisionInfo {
        bool blocku = false;
        bool blockd = false;
        bool blockl = false;
        bool blockr = false;
    };

    struct PhysicsParams {
        float gravity = 40;
        float slippery = 0;

        bool enable_collision = true;
        bool ignore_platforms = false;
    };

    class PhysicalBody {
        sf::FloatRect hitbox_prev;

    protected:
        CollisionInfo collision_info;

        void collide(GameState &game, bool by_x);
        bool isCollide(GameState &game);

    public:
        sf::FloatRect hitbox;
        sf::Vector2f speed;

        PhysicsParams physics;

        virtual void update(GameState &game, float dtime);

        const CollisionInfo &getCollisionInfo() const {
            return collision_info;
        }
    };

}

#endif
