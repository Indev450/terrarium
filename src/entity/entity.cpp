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

#include "entity.hpp"
#include "../game.hpp"

namespace Terrarium {

    void Entity::update(GameState &game, float dtime) {
        anims.update(dtime);

        std::shared_ptr<Entity> parent_entity = parent.ref.lock();

        if (parent_entity != nullptr) {
            hitbox.left = parent_entity->hitbox.left + parent.local_position.x;
            hitbox.top = parent_entity->hitbox.top + parent.local_position.y;

            return;
        }

        PhysicalBody::update(game, dtime);
    }

    void Entity::draw(GameState &game, sf::RenderTarget &target) {
        sf::Vector2f position = game.blocks_to_pixels.transformPoint(
            hitbox.left + sprite_rect.left - game.camera.left,
            hitbox.top + sprite_rect.top - game.camera.top);

        anims.setPosition(position + anims.getOrigin());
        anims.draw_size.x = sprite_rect.width * Tile::SIZE;
        anims.draw_size.y = sprite_rect.height * Tile::SIZE;

        target.draw(anims);

        if (have_text) {
            // TODO - initialize font somewhere else...
            if (!text.getFont()) {
                text.setFont(game.gfx.font);
            }

            text.setPosition(position + text_offset);
            target.draw(text);
        }
    }

} // namespace Terrarium
