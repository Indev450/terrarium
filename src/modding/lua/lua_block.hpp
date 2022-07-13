#ifndef MODDING_LUA_BLOCK_HPP
#define MODDING_LUA_BLOCK_HPP

#include <lua.hpp>

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaBlockAPI {

        void init(LuaModdingInterface &lua_interface);

        // Block-related core module functions

        // int core._register_block(BlockDef block_def)
        int register_block(lua_State *L);

        // Tile core._get_tile(int x, int y)
        int get_tile(lua_State *L);

        // int core._get_block(int x, int y)
        int get_block(lua_State *L);

        // int core._get_wall(int x, int y)
        int get_wall(lua_State *L);

        // void core._set_block(int x, int y, int block_id)
        int set_block(lua_State *L);

        // void core._set_wall(int x, int y, int block_id)
        int set_wall(lua_State *L);

    }

}

#endif
