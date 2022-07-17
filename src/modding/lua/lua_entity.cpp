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

#include <stdexcept>

#include "lua_entity.hpp"
#include "lua_util.hpp"
#include "lua_interface.hpp"

namespace Terrarium {

    namespace LuaEntityAPI {

        LuaEntityUD::LuaEntityUD(std::weak_ptr<Entity> _entity_ref):
            entity_ref(_entity_ref)
        {}

        entityid LuaEntityUD::getID() {
            std::shared_ptr<Entity> entity = entity_ref.lock();

            if (!entity) {
                return 0;
            }

            return entity->id;
        }

        bool LuaEntityUD::isValid() {
            return !entity_ref.expired();
        }

        sf::Vector2f LuaEntityUD::getPosition() {
            std::shared_ptr<Entity> entity = checkedLock();

            return sf::Vector2f(entity->hitbox.left, entity->hitbox.top);
        }

        void LuaEntityUD::setPosition(const sf::Vector2f &position) {
            std::shared_ptr<Entity> entity = checkedLock();

            entity->hitbox.left = position.x;
            entity->hitbox.top = position.y;
        }

        sf::Vector2f LuaEntityUD::getSpeed() {
            std::shared_ptr<Entity> entity = checkedLock();

            return entity->speed;
        }

        void LuaEntityUD::setSpeed(const sf::Vector2f &speed) {
            std::shared_ptr<Entity> entity = checkedLock();

            entity->speed = speed;
        }

        const CollisionInfo &LuaEntityUD::getCollisionInfo() {
            std::shared_ptr<Entity> entity = checkedLock();

            return entity->collision_info;
        }

        bool LuaEntityUD::isCollide(LuaEntityUD &other_ref) {
            std::shared_ptr<Entity> self = checkedLock();
            std::shared_ptr<Entity> other = other_ref.checkedLock();

            return self->hitbox.intersects(other->hitbox);
        }

        bool LuaEntityUD::isCollisionEnabled() {
            std::shared_ptr<Entity> entity = checkedLock();

            return entity->physics.enable_collision;
        }

        void LuaEntityUD::setCollisionEnabled(bool enable_collision) {
            std::shared_ptr<Entity> entity = checkedLock();

            entity->physics.enable_collision = enable_collision;
        }

        float LuaEntityUD::getGravity() {
            std::shared_ptr<Entity> entity = checkedLock();

            return entity->physics.gravity;
        }

        void LuaEntityUD::setGravity(float gravity) {
            std::shared_ptr<Entity> entity = checkedLock();

            entity->physics.gravity = gravity;
        }

        const std::string &LuaEntityUD::getAnimation() {
            std::shared_ptr<Entity> entity = checkedLock();

            return entity->anims.get();
        }

        void LuaEntityUD::setAnimation(const std::string &name, bool restart) {
            std::shared_ptr<Entity> entity = checkedLock();

            entity->anims.set(name, restart);
        }

        void LuaEntityUD::setMirror(bool mirror) {
            std::shared_ptr<Entity> entity = checkedLock();

            entity->anims.setMirror(mirror);
        }

        void LuaEntityUD::kill(EntityManager &entity_mgr) {
            entity_mgr.del(getID());
        }

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_new_entity_prefab", new_entity_prefab);
            lua_interface.registerFunction("_override_entity_prefab", override_entity_prefab);
            lua_interface.registerFunction("_new_entity", new_entity);

            lua_State *L = lua_interface.getLuaState();

            luaL_newmetatable(L, LUA_ENTITYREF.c_str()); // push metatable

            lua_newtable(L); // push __index table

            lua_pushcfunction(L, entity_is_valid);
            lua_setfield(L, -2, "is_valid");

            lua_pushboolean(L, false);
            lua_setfield(L, -2, "is_player");

            lua_pushcfunction(L, entity_get_position);
            lua_setfield(L, -2, "get_position");

            lua_pushcfunction(L, entity_set_position);
            lua_setfield(L, -2, "set_position");

            lua_pushcfunction(L, entity_get_speed);
            lua_setfield(L, -2, "get_speed");

            lua_pushcfunction(L, entity_set_speed);
            lua_setfield(L, -2, "set_speed");

            lua_pushcfunction(L, entity_get_collision_info);
            lua_setfield(L, -2, "get_collision_info");

            lua_pushcfunction(L, entity_is_collide);
            lua_setfield(L, -2, "is_collide");

            lua_pushcfunction(L, entity_is_collision_enabled);
            lua_setfield(L, -2, "is_collision_enabled");

            lua_pushcfunction(L, entity_set_collision_enabled);
            lua_setfield(L, -2, "set_collision_enabled");

            lua_pushcfunction(L, entity_get_gravity);
            lua_setfield(L, -2, "get_gravity");

            lua_pushcfunction(L, entity_set_gravity);
            lua_setfield(L, -2, "set_gravity");

            lua_pushcfunction(L, entity_get_animation);
            lua_setfield(L, -2, "get_animation");

            lua_pushcfunction(L, entity_set_animation);
            lua_setfield(L, -2, "set_animation");

            lua_pushcfunction(L, entity_set_mirror);
            lua_setfield(L, -2, "set_mirror");

            lua_interface.pushClosure(entity_kill);
            lua_setfield(L, -2, "kill");

            lua_setfield(L, -2, "__index"); // pop __index table

            // Add destructor
            lua_pushcfunction(L, LuaUtil::call_destructor<LuaEntityUD>);
            lua_setfield(L, -2, "__gc");

            lua_pop(L, 1); // pop metatable
        }

        int new_entity_prefab(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            std::shared_ptr<EntityPrefab> prefab = checkentityprefab(*lua_interface, 1);

            lua_pushinteger(L, lua_interface->game->entity_mgr.addPrefab(prefab));

            return 1;
        }

        int override_entity_prefab(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            entity_prefabid prefab_id = LuaUtil::checkinteger_ranged<entity_prefabid>(L, 1);

            std::shared_ptr<EntityPrefab> prefab = checkentityprefab(*lua_interface, 2);

            lua_interface->game->entity_mgr.overridePrefab(prefab_id, prefab);

            return 0;
        }

        int new_entity(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            entity_prefabid prefab_id = 0;

            // If there is additional argument, use it as prefab id
            if (lua_gettop(L) != 0) {
                prefab_id = luaL_checkinteger(L, 1);
            }

            entityid entity_id = lua_interface->game->entity_mgr.create(prefab_id);

            // If failed to create entity, return nil (though i don't think this will ever happen)
            if (entity_id == 0) {
                return 0;
            }

            std::shared_ptr<Entity> entity = lua_interface->game->entity_mgr.get(entity_id);

            // Create and push userdata (which is just chunk of uninitialized memory!)
            void *entity_ref_mem = lua_newuserdatauv(L, sizeof(LuaEntityUD), 0);

            // Hope that calls constructor
            new (entity_ref_mem) LuaEntityUD(entity);

            luaL_setmetatable(L, LUA_ENTITYREF.c_str());

            return 1;
        }

        int entity_is_valid(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            lua_pushboolean(L, entity_ref->isValid());

            return 1;
        }

        int entity_get_position(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            sf::Vector2f position;

            try {
                position = entity_ref->getPosition();
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            lua_newtable(L);

            lua_pushnumber(L, position.x);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, position.y);
            lua_setfield(L, -2, "y");

            return 1;
        }

        int entity_set_position(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            if (!lua_istable(L, 2)) {
                return luaL_error(L, "expected table argument");
            }

            sf::Vector2f position;

            lua_getfield(L, 2, "x");
            position.x = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, 2, "y");
            position.y = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            try {
                entity_ref->setPosition(position);
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 0;
        }

        int entity_get_speed(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            sf::Vector2f speed;

            try {
                speed = entity_ref->getSpeed();
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            lua_newtable(L);

            lua_pushnumber(L, speed.x);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, speed.y);
            lua_setfield(L, -2, "y");

            return 1;
        }

        int entity_set_speed(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            if (!lua_istable(L, 2)) {
                return luaL_error(L, "expected table argument");
            }

            sf::Vector2f speed;

            lua_getfield(L, 2, "x");
            speed.x = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, 2, "y");
            speed.y = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            try {
                entity_ref->setSpeed(speed);
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 0;
        }

        int entity_get_collision_info(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            CollisionInfo collision_info;

            try {
                collision_info = entity_ref->getCollisionInfo();
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            lua_newtable(L);

            lua_pushboolean(L, collision_info.blockl);
            lua_setfield(L, -2, "blockl");

            lua_pushboolean(L, collision_info.blockr);
            lua_setfield(L, -2, "blockr");

            lua_pushboolean(L, collision_info.blocku);
            lua_setfield(L, -2, "blocku");

            lua_pushboolean(L, collision_info.blockd);
            lua_setfield(L, -2, "blockd");

            return 1;
        }

        int entity_is_collide(lua_State *L) {
            LuaEntityUD *self = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));
            LuaEntityUD *other = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 2, LUA_ENTITYREF));

            try {
                lua_pushboolean(L, self->isCollide(*other));
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        int entity_is_collision_enabled(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            try {
                lua_pushboolean(L, entity_ref->isCollisionEnabled());
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        int entity_set_collision_enabled(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            try {
                entity_ref->setCollisionEnabled(LuaUtil::checkboolean(L, 2));
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 0;
        }

        int entity_get_gravity(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            try {
                lua_pushnumber(L, entity_ref->getGravity());
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        int entity_set_gravity(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            try {
                entity_ref->setGravity(luaL_checknumber(L, 2));
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 0;
        }

        int entity_get_animation(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            try {
                lua_pushstring(L, entity_ref->getAnimation().c_str());
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        int entity_set_animation(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            try {
                const char *name = luaL_checkstring(L, 2);
                bool restart = false;

                // Optional restart argument
                if (lua_gettop(L) != 2) {
                    restart = LuaUtil::checkboolean(L, 3);
                }

                entity_ref->setAnimation(name, restart);
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 0;
        }

        int entity_set_mirror(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            try {
                entity_ref->setMirror(LuaUtil::checkboolean(L, 2));
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 0;
        }

        int entity_kill(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(LuaUtil::checksubclass(L, 1, LUA_ENTITYREF));

            entity_ref->kill(lua_interface->game->entity_mgr);

            return 0;
        }

        void push_entity(lua_State *L, std::weak_ptr<Entity> entity) {
            void *entity_ref_mem = lua_newuserdatauv(L, sizeof(LuaEntityUD), 0);

            // Hope that calls constructor
            new (entity_ref_mem) LuaEntityUD(entity);

            luaL_setmetatable(L, LUA_ENTITYREF.c_str());
        }

        std::shared_ptr<EntityPrefab> checkentityprefab(LuaModdingInterface &lua_interface, int idx) {
            lua_State *L = lua_interface.getLuaState();

            idx = lua_absindex(L, idx);

            std::shared_ptr<EntityPrefab> prefab = std::make_shared<EntityPrefab>();

            // Stack should contain one argument - table with entity prefab values
            if (!lua_istable(L, idx)) {
                luaL_error(L, "expected table as entity prefab");
            }

            lua_getfield(L, idx, "physics"); // push physics table

            if (!lua_istable(L, -1)) {
                luaL_error(L, "expected table as prefab's physics");
            }

            lua_getfield(L, -1, "gravity"); // push value
            prefab->physics.gravity = luaL_checknumber(L, -1);
            lua_pop(L, 1); // pop value

            lua_getfield(L, -1, "enable_collision"); // push value
            prefab->physics.enable_collision = LuaUtil::checkboolean(L, -1);
            lua_pop(L, 2); // pop value and physics table

            lua_getfield(L, idx, "size"); // push size table

            if (!lua_istable(L, -1)) {
                luaL_error(L, "expected table as prefab's size");
            }

            lua_getfield(L, -1, "width"); // push value
            prefab->size.x = luaL_checknumber(L, -1);
            lua_pop(L, 1); // pop value

            lua_getfield(L, -1, "height"); // push value
            prefab->size.y = luaL_checknumber(L, -1);
            lua_pop(L, 2); // pop value and size table

            lua_getfield(L, idx, "image"); // push value
            const char *image = luaL_checkstring(L, -1);
            prefab->anims.setTexture(lua_interface.game->gfx.textures.get(image));
            lua_pop(L, 1); // pop value

            lua_getfield(L, idx, "animations"); // push value

            if (!lua_istable(L, -1)) {
                luaL_error(L, "expected table as prefab's animations list");
            }

            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                const char *name = luaL_checkstring(L, -2);
                prefab->anims.add(name, LuaUtil::checkanimation(L, -1));

                lua_pop(L, 1);
            }

            lua_pop(L, 1); // pop value

            return prefab;
        }

    } // namespace LuaEntityAPI

} // namespace Terrarium
