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

#ifndef MODDING_LUA_MAPGEN_HPP
#define MODDING_LUA_MAPGEN_HPP

#include <lua.hpp>

#include "../../mapgen/mapgen_base.hpp"

namespace Terrarium {

    namespace LuaMapgenAPI {

        // Functions to call from C++

        Tile checktile(lua_State *L, int idx);

        Biome checkbiome(lua_State *L, int idx);

    }

}

#endif
