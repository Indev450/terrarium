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
