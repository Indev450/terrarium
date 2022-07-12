#ifndef MODDING_LUA_ENTITY_HPP
#define MODDING_LUA_ENTITY_HPP

#define LUA_ENTITYREF "EntityRef"

#include <stdexcept>
#include <memory>

#include <lua.hpp>
#include <SFML/System/Vector2.hpp>

#include "../../entity/entity.hpp"

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaEntityAPI {

        // Userdata class. Hopefully it'll work well with weak_ptr...
        class LuaEntityUD {
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

            sf::Vector2f getSpeed();
            void setSpeed(const sf::Vector2f &speed);

            const CollisionInfo &getCollisionInfo();

            bool isCollide(LuaEntityUD &other);
        };

        // Register all functions
        void init(LuaModdingInterface &lua_interface);

        // Entity-related core module functions

        // int core._new_entity_prefab(EntityPrefabSpec prefab)
        int new_entity_prefab(lua_State *L);

        // EntityRef core._new_entity([int prefab_id])
        int new_entity(lua_State *L);

        // EntityRef userdata methods

        // bool EntityRef:is_valid()
        int entity_is_valid(lua_State *L);

        // Vector2f EntityRef:get_position()
        int entity_get_position(lua_State *L);

        // void EntityRef:set_position(Vector2f position)
        int entity_set_position(lua_State *L);

        // Vector2f EntityRef:get_speed()
        int entity_get_speed(lua_State *L);

        // void EntityRef:set_speed(Vector2f speed)
        int entity_set_speed(lua_State *L);

        // CollisionInfo EntityRef:get_collision_info()
        int entity_get_collision_info(lua_State *L);

        // bool EntityRef:is_collide(Entity other)
        int entity_is_collide(lua_State *L);

        // Functions to call from C++

        void push_entity(lua_State *L, std::weak_ptr<Entity> entity);

    }; // namespace LuaEntityAPI

} // namespace Terrarium

#endif
