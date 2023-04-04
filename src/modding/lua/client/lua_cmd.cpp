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

#include "lua_client_interface.hpp"
#include "lua_cmd.hpp"
#include "../../../game.hpp"

namespace Terrarium {

    namespace LuaClientCmdAPI {

        void init(LuaClientModdingInterface &lua_interface) {
            lua_interface.registerFunction("_send_cmd", send_cmd);
        }

        int send_cmd(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            if (lua_gettop(L) == 0) {
                return luaL_error(L, "expected at least cmd name");
            }

            ModCmdEvent *cmd_event = new ModCmdEvent();

            cmd_event->name = luaL_checkstring(L, 1);

            int argc = lua_gettop(L);

            for (int i = 2; i <= argc; ++i) {
                cmd_event->args.push_back(luaL_checkstring(L, i));
            }

            cmd_event->user = lua_interface->game->player;

            lua_interface->game->events.emplace(Event::ModCmd, cmd_event);

            return 0;
        }

    }

}
