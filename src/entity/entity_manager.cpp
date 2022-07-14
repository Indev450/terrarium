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
