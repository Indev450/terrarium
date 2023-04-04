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

#include <stdexcept>

#include "lua_mod_config.hpp"
#include "lua_util.hpp"

namespace Terrarium {

    LuaModConfig load_config(lua_State *L, const fs::path &root) {
        if (!LuaUtil::run_script(L, (root / "config.lua").string().c_str())) {
            std::string error_msg = "Failed to load config at ";
            error_msg += (root / "config.lua").string();

            throw std::runtime_error(error_msg);
        }

        LuaModConfig config;

        config.root = root;

        if (lua_getglobal(L, "config") != LUA_TTABLE) {
            lua_pop(L, 1);

            std::string error_msg = "'config' global expected to be a table in ";
            error_msg += (root / "config.lua").string().c_str();

            throw std::runtime_error(error_msg);
        }

        lua_getfield(L, -1, "name");
        config.name = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        if (!lua_getfield(L, -1, "depends") == LUA_TNIL) {
            lua_Integer len = luaL_len(L, -1);

            for (lua_Integer i = 1; i <= len; ++i) {
                lua_geti(L, -1, i);

                config.depends.insert(luaL_checkstring(L, -1));

                lua_pop(L, 1);
            }
        }

        lua_pop(L, 1);

        if (!lua_getfield(L, -1, "optional_depends") == LUA_TNIL) {
            lua_Integer len = luaL_len(L, -1);

            for (lua_Integer i = 1; i < len; ++i) {
                lua_geti(L, -1, i);

                config.optional_depends.insert(luaL_checkstring(L, -1));

                lua_pop(L, 1);
            }
        }

        lua_pop(L, 2); // Also pops "config"

        // Remove that table to avoid conflicts with other mods
        lua_pushnil(L);
        lua_setglobal(L, "config");

        return config;
    }

}
