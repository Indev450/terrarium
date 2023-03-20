/*
 * Copyright 2022 Inner Devil
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

        auto begin() {
            return OverflowingMap<entityid, Entity>::begin();
        }

        auto end() {
            return OverflowingMap<entityid, Entity>::end();
        }

        void update(GameState &game, float dtime);

        void draw(GameState &game, sf::RenderTarget &target);
    };

} // namespace Terrarium

#endif
