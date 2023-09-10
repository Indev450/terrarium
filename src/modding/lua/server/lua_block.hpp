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

        // Vector2i core._get_multiblock_origin(int x, int y)
        int get_multiblock_origin(lua_State *L);

        // int core._get_wall(int x, int y)
        int get_wall(lua_State *L);

        // void core._set_block(int x, int y, int block_id)
        int set_block(lua_State *L);

        // void core._set_wall(int x, int y, int block_id)
        int set_wall(lua_State *L);

        // void core._set_multiblock_offset(int x, int y, unsigned xoff, unsigned yoff)
        int set_multiblock_offset(lua_State *L);

        // void core._set_multiblock(int x, int y, int width, int height, int block_id)
        int set_multiblock(lua_State *L);

        // void core._start_block_timer(int x, int y, double timer)
        int start_block_timer(lua_State *L);

        // void core._start_wall_timer(int x, int y, double timer)
        int start_wall_timer(lua_State *L);

        // InventoryRef core._get_block_inventory(int x, int y)
        int get_block_inventory(lua_State *L);

    }

}

#endif
