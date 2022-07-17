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

#include <memory>

#include "lua_block.hpp"
#include "lua_interface.hpp"
#include "lua_util.hpp"
#include "../../tile/block_def.hpp"

namespace Terrarium {

    namespace LuaBlockAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_register_block", register_block);
            lua_interface.registerFunction("_get_tile", get_tile);
            lua_interface.registerFunction("_get_block", get_block);
            lua_interface.registerFunction("_get_wall", get_wall);
            lua_interface.registerFunction("_set_block", set_block);
            lua_interface.registerFunction("_set_wall", set_wall);
        }

        int register_block(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            std::shared_ptr<BlockDef> block_def = std::make_shared<BlockDef>();

            if (!lua_istable(L, 1)) {
                return luaL_error(L, "expected table as block def");
            }

            // image, slippery, is_solid

            lua_getfield(L, 1, "image");
            const char *image = luaL_checkstring(L, -1);
            block_def->sprite.setTexture(lua_interface->game->gfx.textures.get(image));
            lua_pop(L, 1);

            lua_getfield(L, 1, "slippery");
            float slippery = LuaUtil::checknumber_ranged(L, -1, 0, 1);

            block_def->slippery = slippery;
            lua_pop(L, 1);

            lua_getfield(L, 1, "is_solid");
            block_def->is_solid = LuaUtil::checkboolean(L, -1);
            lua_pop(L, 1);

            lua_pushinteger(L, lua_interface->game->block_defs.add(block_def));

            return 1;
        }

        int get_tile(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);

            const Tile *tile = lua_interface->game->world.getTile(x, y);

            blockid block = 0, wall = 0;

            if (tile != nullptr) {
                block = tile->fg;
                wall = tile->bg;
            }

            lua_newtable(L);

            lua_pushinteger(L, block);
            lua_setfield(L, -2, "block");

            lua_pushinteger(L, wall);
            lua_setfield(L, -2, "wall");

            return 1;
        }

        int get_block(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);

            lua_pushinteger(L, lua_interface->game->world.getBlock(x, y));

            return 1;
        }

        int get_wall(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);

            lua_pushinteger(L, lua_interface->game->world.getWall(x, y));

            return 1;
        }

        int set_block(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);

            blockid block_id = LuaUtil::checkinteger_ranged<blockid>(L, 3);

            lua_interface->game->world.setBlock(x, y, block_id);

            return 0;
        }

        int set_wall(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);

            blockid block_id = LuaUtil::checkinteger_ranged<blockid>(L, 3);

            lua_interface->game->world.setWall(x, y, block_id);

            return 0;
        }

    }

}
