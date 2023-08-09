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

#ifndef MODDING_LUA_CHAT_HPP
#define MODDING_LUA_CHAT_HPP

#include <lua.hpp>

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaChatAPI {

        void init(LuaModdingInterface &lua_interface);

        // void core._send_message(string message[, Color color])
        int send_message(lua_State *L);

        // void core._send_message_to(Player player, string message, [, Color color])
        // int send_message_to(lua_State *L);

    }

}

#endif
