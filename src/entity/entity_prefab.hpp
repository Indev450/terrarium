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

#ifndef ENTITY_PREFAB_HPP
#define ENTITY_PREFAB_HPP

#include "entity.hpp"

namespace Terrarium {

    typedef uint16_t entity_prefabid;

    struct EntityPrefab {
        PhysicsParams physics;
        sf::Vector2f sprite_size;
        sf::FloatRect hitbox;

        AnimationMap anims;

        void initEntity(Entity &entity) {
            entity.hitbox.width = hitbox.width;
            entity.hitbox.height = hitbox.height;

            // Might be bit confusing but here how it works:
            // When you specify sprite size, it is, well, actual sprite. But
            // when you specify hitbox, its x and y coordinates mean how much
            // offset it needs to have within sprite. Those are negative because
            // we actually offsetting sprite, and not hitbox itself.
            entity.sprite_rect.left = -hitbox.left;
            entity.sprite_rect.top = -hitbox.top;

            entity.sprite_rect.width = sprite_size.x;
            entity.sprite_rect.height = sprite_size.y;

            entity.physics = physics;

            entity.anims = anims;
        }
    };

}

#endif
