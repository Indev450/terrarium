/*
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

#include <iostream>

#include "entity_manager.hpp"
#include "../game.hpp"

namespace Terrarium {

    entity_prefabid EntityManager::addPrefab(std::shared_ptr<EntityPrefab> prefab) {
        return prefabs.add(prefab);
    }

    void EntityManager::overridePrefab(entity_prefabid prefab_id, std::shared_ptr<EntityPrefab> prefab) {
        prefabs.set(prefab_id, prefab);
    }

    void EntityManager::update(GameState &game, float dtime) {
        for (auto &pair: map) {
            pair.second->update(game, dtime);
        }
    }

    void EntityManager::draw(GameState &game, sf::RenderTarget &target) {
        for (auto &pair: map) {
            if (pair.second->hitbox.intersects(game.camera)) {
                sf::Vector2f position = {
                    pair.second->hitbox.left - game.camera.left,
                    pair.second->hitbox.top - game.camera.top,
                };

                pair.second->anims.draw(target, game.blocks_to_pixels.transformPoint(position));
            }
        }
    }

} // namespace Terrarium
