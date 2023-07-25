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

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../tile/tile.hpp"
#include "../graphics/animation_map.hpp"

namespace Terrarium {

    typedef uint16_t entityid;

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

    class Entity;

    struct Parent {
        std::weak_ptr<Entity> ref;
        sf::Vector2f local_position;
    };

    class Entity {
    protected:
        void collide(GameState &game, bool by_x);
        bool isCollide(GameState &game);

    public:
        sf::FloatRect sprite_rect = {0, 0, 0, 0};
        sf::FloatRect hitbox = {0, 0, 0, 0};
        sf::FloatRect hitbox_prev = {0, 0, 0, 0};

        CollisionInfo collision_info;

        PhysicsParams physics;

        sf::Vector2f speed = {0, 0};

        sf::Vector3i light;

        entityid id = 0;

        AnimationMap anims;

        sf::Text text; // Text for names/damage indicators/etc
        bool have_text = false;
        sf::Vector2f text_offset;

        Parent parent;

        virtual void update(GameState &game, float dtime);

        virtual void draw(GameState &game, sf::RenderTarget &target);
    };

} // namespace Terrarium

#endif
