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

#ifndef MODDING_LUA_ENTITY_HPP
#define MODDING_LUA_ENTITY_HPP

#include <stdexcept>
#include <memory>
#include <string>

#include <lua.hpp>
#include <SFML/System/Vector2.hpp>

#include "../../entity/entity.hpp"
#include "../../entity/entity_manager.hpp"
#include "../../entity/entity_prefab.hpp"

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaEntityAPI {

        static const std::string LUA_ENTITYREF = "EntityRef";

        // Userdata class. Hopefully it'll work well with weak_ptr...
        class LuaEntityUD {
        protected:
            std::weak_ptr<Entity> entity_ref;

            // If entity ref is expired, throw exception, otherwise return
            // valid shared_ptr
            inline std::shared_ptr<Entity> checkedLock() {
                std::shared_ptr<Entity> entity = entity_ref.lock();

                if (!entity) {
                    throw std::invalid_argument("entity ref is expired");
                }

                return entity;
            }

        public:
            LuaEntityUD(std::weak_ptr<Entity> _entity_ref);

            entityid getID();

            bool isValid();

            sf::Vector2f getPosition();
            void setPosition(const sf::Vector2f &position);

            sf::Vector2f getLocalPosition();
            void setLocalPosition(const sf::Vector2f &position);

            sf::Vector2f getSpeed();
            void setSpeed(const sf::Vector2f &speed);

            const CollisionInfo &getCollisionInfo();

            bool isCollide(LuaEntityUD &other_ref);

            bool isAttachedTo(LuaEntityUD &other_ref);

            void attachTo(LuaEntityUD &other_ref);

            void deattach();

            bool isCollisionEnabled();

            void setCollisionEnabled(bool enable_collision);

            float getGravity();

            void setGravity(float gravity);

            const std::string &getAnimation();

            void setAnimation(const std::string &name, bool restart);

            void setMirror(bool mirror);

            void setTexture(const sf::Texture &texture);

            void kill(EntityManager &entity_mgr);

            std::weak_ptr<Entity> getPtr() { return entity_ref; }
        };

        // Register all functions
        void init(LuaModdingInterface &lua_interface);

        // Entity-related core module functions

        // int core._new_entity_prefab(EntityPrefabSpec prefab)
        int new_entity_prefab(lua_State *L);

        // void core._override_entity_prefab(int prefab_id, EntityPrefabSpec prefab)
        int override_entity_prefab(lua_State *L);

        // EntityRef core._new_entity([int prefab_id])
        int new_entity(lua_State *L);

        // EntityRef userdata methods

        // bool EntityRef:is_valid()
        int entity_is_valid(lua_State *L);

        // Vector2f EntityRef:get_position()
        int entity_get_position(lua_State *L);

        // void EntityRef:set_position(Vector2f position)
        int entity_set_position(lua_State *L);

        // Vector2f EntityRef:get_local_position()
        int entity_get_local_position(lua_State *L);

        // void EntityRef:set_local_position(Vector2f position)
        int entity_set_local_position(lua_State *L);

        // Vector2f EntityRef:get_speed()
        int entity_get_speed(lua_State *L);

        // void EntityRef:set_speed(Vector2f speed)
        int entity_set_speed(lua_State *L);

        // CollisionInfo EntityRef:get_collision_info()
        int entity_get_collision_info(lua_State *L);

        // bool EntityRef:is_collide(EntityRef other)
        int entity_is_collide(lua_State *L);

        // bool EntityRef:is_attached_to(EntityRef other)
        int entity_is_attached_to(lua_State *L);

        // void EntityRef:attach_to(EntityRef other)
        int entity_attach_to(lua_State *L);

        // void EntityRef:deattach()
        int entity_deattach(lua_State *L);

        // bool EntityRef:is_collision_enabled()
        int entity_is_collision_enabled(lua_State *L);

        // void EntityRef:set_collision_enabled(bool collision_enabled)
        int entity_set_collision_enabled(lua_State *L);

        // float EntityRef:get_gravity()
        int entity_get_gravity(lua_State *L);

        // void EntityRef:set_gravity(float gravity)
        int entity_set_gravity(lua_State *L);

        // string EntityRef:get_animation()
        int entity_get_animation(lua_State *L);

        // void EntityRef:set_animation(string name[, bool restart = false])
        int entity_set_animation(lua_State *L);

        // void EntityRef:set_mirror(bool mirror)
        int entity_set_mirror(lua_State *L);

        // void EntityRef:set_texture(string texture)
        int entity_set_texture(lua_State *L);

        // void EntityRef:kill()
        int entity_kill(lua_State *L);

        // Functions to call from C++

        void push_entity(lua_State *L, std::weak_ptr<Entity> entity);

        std::shared_ptr<EntityPrefab> checkentityprefab(LuaModdingInterface &lua_interface, int idx);

    }; // namespace LuaEntityAPI

} // namespace Terrarium

#endif
