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

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_new_entity_prefab", new_entity_prefab);
            lua_interface.registerFunction("_new_entity", new_entity);

            lua_State *L = lua_interface.getLuaState();

            luaL_newmetatable(L, LUA_ENTITYREF); // push metatable

            lua_newtable(L); // push __index table

            lua_pushcfunction(L, entity_is_valid);
            lua_setfield(L, -2, "is_valid");

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

            lua_setfield(L, -2, "__index"); // pop __index table

            // Add destructor
            lua_pushcfunction(L, LuaUtil::call_destructor<LuaEntityUD>);
            lua_setfield(L, -2, "__gc");

            lua_pop(L, 1); // pop metatable
        }

        int new_entity_prefab(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            std::shared_ptr<EntityPrefab> prefab = std::make_shared<EntityPrefab>();

            // Stack should contain one argument - table with entity prefab values
            if (!lua_istable(L, 1)) {
                return luaL_error(L, "expected table as entity prefab");
            }

            lua_getfield(L, -1, "physics"); // push physics table

            if (!lua_istable(L, -1)) {
                return luaL_error(L, "expected table as prefab's physics");
            }

            lua_getfield(L, -1, "gravity"); // push value
            prefab->physics.gravity = luaL_checknumber(L, -1);
            lua_pop(L, 1); // pop value

            lua_getfield(L, -1, "enable_collision"); // push value

            // If it is not boolean, it is not big problem, but it is better to leave warning
            if (!lua_isboolean(L, -1)) {
                lua_warning(L, "<entity_prefab>.physics.enable_collision should be bool", false);
            }

            prefab->physics.enable_collision = lua_toboolean(L, -1);
            lua_pop(L, 2); // pop value and physics table

            lua_getfield(L, -1, "size"); // push size table

            if (!lua_istable(L, -1)) {
                return luaL_error(L, "expected table as prefab's size");
            }

            lua_getfield(L, -1, "width"); // push value
            prefab->size.x = luaL_checknumber(L, -1);
            lua_pop(L, 1); // pop value

            lua_getfield(L, -1, "height"); // push value
            prefab->size.y = luaL_checknumber(L, -1);
            lua_pop(L, 2); // pop value and size table

            lua_getfield(L, -1, "image"); // push value
            const char *image = luaL_checkstring(L, -1);
            prefab->anims.setTexture(lua_interface->game->gfx.getTexture(image));
            lua_pop(L, 1); // pop value

            // TODO - read animations

            lua_pushinteger(L, lua_interface->game->entity_mgr.addPrefab(prefab));

            return 1;
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

            luaL_setmetatable(L, LUA_ENTITYREF);

            return 1;
        }

        int entity_is_valid(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(luaL_checkudata(L, 1, LUA_ENTITYREF));

            lua_pushboolean(L, entity_ref->isValid());

            return 1;
        }

        int entity_get_position(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(luaL_checkudata(L, 1, LUA_ENTITYREF));

            sf::Vector2f position = entity_ref->getPosition();

            lua_newtable(L);

            lua_pushnumber(L, position.x);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, position.y);
            lua_setfield(L, -2, "y");

            return 1;
        }

        int entity_set_position(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(luaL_checkudata(L, 1, LUA_ENTITYREF));

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

            entity_ref->setPosition(position);

            return 0;
        }

        int entity_get_speed(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(luaL_checkudata(L, 1, LUA_ENTITYREF));

            sf::Vector2f speed = entity_ref->getSpeed();

            lua_newtable(L);

            lua_pushnumber(L, speed.x);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, speed.y);
            lua_setfield(L, -2, "y");

            return 1;
        }

        int entity_set_speed(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(luaL_checkudata(L, 1, LUA_ENTITYREF));

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

            entity_ref->setSpeed(speed);

            return 0;
        }

        int entity_get_collision_info(lua_State *L) {
            LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityUD*>(luaL_checkudata(L, 1, LUA_ENTITYREF));

            const CollisionInfo &collision_info = entity_ref->getCollisionInfo();

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
            LuaEntityUD *self = reinterpret_cast<LuaEntityUD*>(luaL_checkudata(L, 1, LUA_ENTITYREF));
            LuaEntityUD *other = reinterpret_cast<LuaEntityUD*>(luaL_checkudata(L, 2, LUA_ENTITYREF));

            lua_pushboolean(L, self->isCollide(*other));

            return 1;
        }

        void push_entity(lua_State *L, std::weak_ptr<Entity> entity) {
            void *entity_ref_mem = lua_newuserdatauv(L, sizeof(LuaEntityUD), 0);

            // Hope that calls constructor
            new (entity_ref_mem) LuaEntityUD(entity);

            luaL_setmetatable(L, LUA_ENTITYREF);
        }

    } // namespace LuaEntityAPI

} // namespace Terrarium
