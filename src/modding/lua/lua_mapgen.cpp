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

    }

}
