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

#ifndef MODDING_LUA_PLAYER_HPP
#define MODDING_LUA_PLAYER_HPP

#include <stdexcept>

#include <lua.hpp>

#include "lua_entity.hpp"
#include "../../player/player.hpp"
#include "lua_util.hpp"

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaPlayerAPI {

        static const std::string LUA_PLAYERREF = LuaUtil::subclass(LuaEntityAPI::LUA_ENTITYREF, "PlayerRef");

        class LuaPlayerUD: public LuaEntityAPI::LuaEntityUD {
            // Auto downcast to Player. There is no normal way to initialize
            // LuaPlayerUD with regular Entity weak_ptr, so that function
            // should never fail (except for when reference is expired)
            inline std::shared_ptr<Player> checkedLock() {
                std::shared_ptr<Entity> player = LuaEntityAPI::LuaEntityUD::checkedLock();

                return std::dynamic_pointer_cast<Player>(player);
            }

        public:
            LuaPlayerUD(std::weak_ptr<Player> player_ref);

            const PlayerControls &getControls();

            float getJumpForce();
            void setJumpForce(float jump_force);

            float getAcceleration();
            void setAcceleration(float acceleration);

            float getMaxSpeed();
            void setMaxSpeed(float max_speed);

            bool isControlled();
            void setControlled(bool controlled);

            std::weak_ptr<Inventory> getInventory();
        };

        void init(LuaModdingInterface &lua_interface);

        // Player-related core module functions

        // If multiplayer will be added, this function will have `string name` argument.
        // Currently it returns always-valid PlayerRef
        // PlayerRef core._get_player()
        int get_player(lua_State *L);

        // PlayerRef userdata methods

        // PlayerControls PlayerRef:get_player_controls()
        int player_get_player_controls(lua_State *L);

        // float PlayerRef:get_player_jump_force()
        int player_get_player_jump_force(lua_State *L);

        // void PlayerRef:set_player_jump_force(float jump_force)
        int player_set_player_jump_force(lua_State *L);

        // float PlayerRef:get_player_acceleration()
        int player_get_player_acceleration(lua_State *L);

        // void PlayerRef:set_player_acceleration(float acceleration)
        int player_set_player_acceleration(lua_State *L);

        // float PlayerRef:get_player_max_speed()
        int player_get_player_max_speed(lua_State *L);

        // void PlayerRef:set_player_max_speed(float max_speed)
        int player_set_player_max_speed(lua_State *L);

        // bool PlayerRef:is_player_controlled()
        int player_is_player_controlled(lua_State *L);

        // void PlayerRef:set_player_controlled(bool controlled)
        int player_set_player_controlled(lua_State *L);

        // InventoryRef PlayerRef:get_player_inventory()
        int player_get_player_inventory(lua_State *L);

        // FloatRect PlayerRef:get_player_camera()
        int player_get_player_camera(lua_State *L);

        // Also inherits all methods from EntityRef

        // Functions to call from C++

        void push_player(lua_State *L, std::weak_ptr<Player> player_ref);
    }

} // namespace Terrarium

#endif
