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

#include "lua_mapgen.hpp"
#include "lua_util.hpp"

namespace Terrarium {

    namespace LuaMapgenAPI {

        Tile checktile(lua_State *L, int idx) {
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            idx = lua_absindex(L, idx);

            Tile tile;

            lua_getfield(L, idx, "block");
            tile.fg = LuaUtil::checkinteger_ranged<blockid>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "wall");
            tile.bg = LuaUtil::checkinteger_ranged<blockid>(L, -1);
            lua_pop(L, 1);

            return tile;
        }

        Biome checkbiome(lua_State *L, int idx) {
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            idx = lua_absindex(L, idx);

            Biome biome;

            lua_getfield(L, idx, "humidity_min");
            biome.humidity_min = LuaUtil::checknumber_ranged(L, -1, -1.0, 1.0);
            lua_pop(L, 1);

            lua_getfield(L, idx, "humidity_max");
            biome.humidity_max = LuaUtil::checknumber_ranged(L, -1, -1.0, 1.0);
            lua_pop(L, 1);

            lua_getfield(L, idx, "heat_min");
            biome.heat_min = LuaUtil::checknumber_ranged(L, -1, -1.0, 1.0);
            lua_pop(L, 1);

            lua_getfield(L, idx, "heat_max");
            biome.heat_max = LuaUtil::checknumber_ranged(L, -1, -1.0, 1.0);
            lua_pop(L, 1);

            lua_getfield(L, idx, "top");
            biome.top = checktile(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "top_depth");
            biome.top_depth = LuaUtil::checkinteger_ranged<unsigned int>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "filler");
            biome.filler = checktile(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "filler_depth");
            biome.filler_depth = LuaUtil::checkinteger_ranged<unsigned int>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "stone");
            biome.stone = checktile(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "priority");
            biome.priority = LuaUtil::checkinteger_ranged<unsigned int>(L, -1);
            lua_pop(L, 1);

            return biome;
        }

        Ore checkore(lua_State *L, int idx) {
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            idx = lua_absindex(L, idx);

            Ore ore;

            lua_getfield(L, idx, "cluster_tiles");
            ore.cluster_tiles = LuaUtil::checkinteger_ranged<unsigned int>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "distribution");
            ore.distribution = LuaUtil::checkinteger_ranged<unsigned int>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "min_depth");
            ore.min_depth = LuaUtil::checknumber_ranged(L, -1, 0.0, 1.0);
            lua_pop(L, 1);

            lua_getfield(L, idx, "max_depth");
            ore.max_depth = LuaUtil::checknumber_ranged(L, -1, 0.0, 1.0);
            lua_pop(L, 1);

            lua_getfield(L, idx, "tile");
            ore.tile = checktile(L, -1);
            lua_pop(L, 1);

            return ore;
        }

    }

}
