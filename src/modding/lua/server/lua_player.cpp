/*
 * Copyright 2022 Indev
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

#include "lua_player.hpp"
#include "lua_interface.hpp"
#include "lua_inventory.hpp"
#include "lua_util.hpp"

namespace Terrarium {

    namespace LuaPlayerAPI {

        LuaPlayerUD::LuaPlayerUD(std::weak_ptr<Player> player_ref):
            LuaEntityAPI::LuaEntityUD(player_ref)
        {}

        const PlayerControls &LuaPlayerUD::getControls() {
            std::shared_ptr<Player> player = checkedLock();

            return player->controls;
        }

        float LuaPlayerUD::getJumpForce() {
            std::shared_ptr<Player> player = checkedLock();

            return player->stats.jump_force;
        }

        void LuaPlayerUD::setJumpForce(float jump_force) {
            std::shared_ptr<Player> player = checkedLock();

            player->stats.jump_force = jump_force;
        }

        float LuaPlayerUD::getAcceleration() {
            std::shared_ptr<Player> player = checkedLock();

            return player->stats.acceleration;
        }

        void LuaPlayerUD::setAcceleration(float acceleration) {
            std::shared_ptr<Player> player = checkedLock();

            player->stats.acceleration = acceleration;
        }

        float LuaPlayerUD::getMaxSpeed() {
            std::shared_ptr<Player> player = checkedLock();

            return player->stats.max_speed;
        }

        void LuaPlayerUD::setMaxSpeed(float max_speed) {
            std::shared_ptr<Player> player = checkedLock();

            player->stats.max_speed = max_speed;
        }

        bool LuaPlayerUD::isControlled() {
            std::shared_ptr<Player> player = checkedLock();

            return player->controlled;
        }

        void LuaPlayerUD::setControlled(bool controlled) {
            std::shared_ptr<Player> player = checkedLock();

            player->controlled = controlled;
        }

        std::weak_ptr<Inventory> LuaPlayerUD::getInventory() {
            std::shared_ptr<Player> player = checkedLock();

            return player->inventory;
        }

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_get_player", get_player);

            lua_State *L = lua_interface.getLuaState();

            luaL_newmetatable(L, LUA_PLAYERREF.c_str()); // push metatable

            lua_newtable(L); // push __index table

            luaL_getmetatable(L, LuaEntityAPI::LUA_ENTITYREF.c_str()); // push entity ref metatable
            lua_setmetatable(L, -2); // pop entity ref metatable for __index

            lua_pushboolean(L, true);
            lua_setfield(L, -2, "is_player");

            lua_pushcfunction(L, player_get_player_controls);
            lua_setfield(L, -2, "get_player_controls");

            lua_pushcfunction(L, player_get_player_jump_force);
            lua_setfield(L, -2, "get_player_jump_force");

            lua_pushcfunction(L, player_set_player_jump_force);
            lua_setfield(L, -2, "set_player_jump_force");

            lua_pushcfunction(L, player_get_player_acceleration);
            lua_setfield(L, -2, "get_player_acceleration");

            lua_pushcfunction(L, player_set_player_acceleration);
            lua_setfield(L, -2, "set_player_acceleration");

            lua_pushcfunction(L, player_get_player_max_speed);
            lua_setfield(L, -2, "get_player_max_speed");

            lua_pushcfunction(L, player_set_player_max_speed);
            lua_setfield(L, -2, "set_player_max_speed");

            lua_pushcfunction(L, player_is_player_controlled);
            lua_setfield(L, -2, "is_player_controlled");

            lua_pushcfunction(L, player_set_player_controlled);
            lua_setfield(L, -2, "set_player_controlled");

            lua_pushcfunction(L, player_get_player_inventory);
            lua_setfield(L, -2, "get_player_inventory");

            // For now, this function just access global game state to get
            // the camera. Maybe, if multiplayer will be added, it will
            // access some kind of "player's camera" instead.
            lua_interface.pushClosure(player_get_player_camera);
            lua_setfield(L, -2, "get_player_camera");

            lua_setfield(L, -2, "__index"); // pop index table

            // Add destructor
            lua_pushcfunction(L, LuaUtil::call_destructor<LuaPlayerUD>);
            lua_setfield(L, -2, "__gc");

            lua_pop(L, 1); // pop metatable
        }

        int get_player(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            push_player(L, lua_interface->game->player);

            return 1;
        }

        int player_get_player_controls(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                const PlayerControls &controls = player_ref->getControls();

                lua_newtable(L);

                lua_pushboolean(L, controls.left);
                lua_setfield(L, -2, "left");

                lua_pushboolean(L, controls.right);
                lua_setfield(L, -2, "right");

                lua_pushboolean(L, controls.jump);
                lua_setfield(L, -2, "jump");

                lua_pushboolean(L, controls.lmb);
                lua_setfield(L, -2, "lmb");

                lua_pushboolean(L, controls.rmb);
                lua_setfield(L, -2, "rmb");

                LuaUtil::push_vector2f(L, controls.mouse_pos);
                lua_setfield(L, -2, "mouse_pos");

                return 1;

            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_get_player_jump_force(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                lua_pushnumber(L, player_ref->getJumpForce());

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_set_player_jump_force(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                player_ref->setJumpForce(luaL_checknumber(L, 2));

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_get_player_acceleration(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                lua_pushnumber(L, player_ref->getAcceleration());

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_set_player_acceleration(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                player_ref->setAcceleration(luaL_checknumber(L, 2));

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_get_player_max_speed(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                lua_pushnumber(L, player_ref->getMaxSpeed());

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_set_player_max_speed(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                player_ref->setMaxSpeed(luaL_checknumber(L, 2));

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_is_player_controlled(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                lua_pushboolean(L, player_ref->isControlled());

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_set_player_controlled(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                player_ref->setControlled(lua_toboolean(L, 2));

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_get_player_inventory(lua_State *L) {
            LuaPlayerUD *player_ref = reinterpret_cast<LuaPlayerUD*>(luaL_checkudata(L, 1, LUA_PLAYERREF.c_str()));

            try {
                LuaInventoryAPI::push_inventory(L, player_ref->getInventory());

                return 1;
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }
        }

        int player_get_player_camera(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            LuaUtil::push_floatrect(L, lua_interface->game->camera);

            return 1;
        }

        void push_player(lua_State *L, std::weak_ptr<Player> player_ref) {
            void *player_ref_mem = lua_newuserdatauv(L, sizeof(LuaPlayerUD), 0);

            new (player_ref_mem) LuaPlayerUD(player_ref);

            luaL_setmetatable(L, LUA_PLAYERREF.c_str());
        }

    }

} // namespace Terrarium
