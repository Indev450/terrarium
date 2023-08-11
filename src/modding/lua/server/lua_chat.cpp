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

#include "lua_interface.hpp"
#include "lua_chat.hpp"
#include "../common/lua_util.hpp"
#include "../../../game.hpp"

namespace Terrarium {

    namespace LuaChatAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_send_message", send_message);
        }

        int send_message(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            const char *message = luaL_checkstring(L, 1);
            sf::Color color = sf::Color::White;

            if (lua_gettop(L) > 1) {
                color = LuaUtil::checkcolor(L, 2);
            }

            lua_interface->game->chat.print({ message, color });

            return 0;
        }

    }

}
