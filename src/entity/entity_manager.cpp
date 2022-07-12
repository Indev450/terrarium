#include <iostream>

#include "entity_manager.hpp"
#include "../game.hpp"

namespace Terrarium {

    entity_prefabid EntityManager::addPrefab(std::shared_ptr<EntityPrefab> prefab) {
        return prefabs.add(prefab);
    }

    entityid EntityManager::create(entity_prefabid prefab_id) {
        Entity *e = new Entity();

        entityid id = add(std::shared_ptr<Entity>(e));

        if (id != 0) {
            e->id = id;

            if (prefab_id != 0) {
                std::shared_ptr<EntityPrefab> prefab = prefabs.get(prefab_id);

                if (!prefab) {
                    std::cerr<<"Terrarium::EntityManager::create: attempt to use prefab id=";
                    std::cerr<<prefab_id<<" that doesn't exist"<<std::endl;
                } else {
                    prefab->initEntity(*e);
                }
            }
        }

        // If id == 0, Entity stored in std::unique_ptr should be deleted, so
        // we don't need to free 'e' manually

        return id;
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
