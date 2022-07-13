#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <vector>

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

        entityid create(entity_prefabid prefab_id = 0);

        // Looks a little ugly, but probably will work without any overhead
        inline std::shared_ptr<Entity> get(entityid id) {
            return OverflowingMap<entityid, Entity>::get(id);
        }

        void update(GameState &game, float dtime);

        void draw(GameState &game, sf::RenderTarget &target);

    };

} // namespace Terrarium

#endif
