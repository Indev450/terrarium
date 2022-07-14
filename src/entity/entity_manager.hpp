#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <iostream>

#include "../utils/overflowing_map.hpp"
#include "entity.hpp"
#include "entity_prefab.hpp"

namespace Terrarium {

    // Protected because OverflowingMap::add function doesn't set entitys id, which can
    // (i think) lead to problems
    class EntityManager: protected OverflowingMap<entityid, Entity> {
        OverflowingMap<entity_prefabid, EntityPrefab> prefabs;

    public:
        entity_prefabid addPrefab(std::shared_ptr<EntityPrefab> prefab);
        void overridePrefab(entity_prefabid prefab_id, std::shared_ptr<EntityPrefab> prefab);

        // EntityType should be derived from Entity
        template<class EntityType = Entity>
        entityid create(entity_prefabid prefab_id = 0) {
            EntityType *e = new EntityType();

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

        // Looks a little ugly, but probably will work without any overhead
        inline std::shared_ptr<Entity> get(entityid id) {
            return OverflowingMap<entityid, Entity>::get(id);
        }

        inline void del(entityid id) {
            OverflowingMap<entityid, Entity>::del(id);
        }

        void update(GameState &game, float dtime);

        void draw(GameState &game, sf::RenderTarget &target);

    };

} // namespace Terrarium

#endif
