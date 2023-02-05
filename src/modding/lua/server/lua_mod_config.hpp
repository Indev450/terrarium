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

#ifndef MODDING_LUA_MOD_CONFIG
#define MODDING_LUA_MOD_CONFIG

#include <string>
#include <filesystem>
#include <set>

#include <lua.hpp>

namespace fs = std::filesystem;

namespace Terrarium {

    struct LuaModConfig {
        std::string name;
        fs::path root;

        std::set<std::string> depends;
        std::set<std::string> optional_depends;

        std::set<std::string> unsatisfied_depends;
    };

    LuaModConfig load_config(lua_State *L, const fs::path &root);

}

#endif
