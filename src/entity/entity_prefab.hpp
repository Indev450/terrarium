#ifndef ENTITY_PREFAB_HPP
#define ENTITY_PREFAB_HPP


#include "entity.hpp"


namespace Terrarium {

    typedef uint16_t entity_prefabid;

    struct EntityPrefab {
        PhysicsParams physics;
        sf::Vector2f size;

        AnimationMap anims;

        void initEntity(Entity &entity) {
            entity.hitbox.width = size.x;
            entity.hitbox.height = size.y;

            entity.physics = physics;

            entity.anims = anims;
        }
    };

}

#endif
