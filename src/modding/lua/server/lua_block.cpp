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

#include <memory>

#include "lua_block.hpp"
#include "lua_inventory.hpp"
#include "lua_interface.hpp"
#include "../common/lua_util.hpp"
#include "../common/lua_field_checker.hpp"
#include "../../../tile/block_def.hpp"

namespace Terrarium {

    namespace LuaBlockAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_register_block", register_block);
            lua_interface.registerFunction("_get_tile", get_tile);
            lua_interface.registerFunction("_get_block", get_block);
            lua_interface.registerFunction("_get_wall", get_wall);
            lua_interface.registerFunction("_get_multiblock_origin", get_multiblock_origin);
            lua_interface.registerFunction("_set_block", set_block);
            lua_interface.registerFunction("_set_wall", set_wall);
            lua_interface.registerFunction("_set_multiblock_offset", set_multiblock_offset);
            lua_interface.registerFunction("_set_multiblock", set_multiblock);
            lua_interface.registerFunction("_start_block_timer", start_block_timer);
            lua_interface.registerFunction("_start_wall_timer", start_wall_timer);
            lua_interface.registerFunction("_average_block_color", average_block_color);
            lua_interface.registerFunction("_get_block_inventory", get_block_inventory);
        }

        int register_block(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            std::shared_ptr<BlockDef> block_def = std::make_shared<BlockDef>();

            block_def->name = luaL_checkstring(L, 1);

            luaL_checktype(L, 2, LUA_TTABLE);

            LuaUtil::FieldChecker checker(L, "BlockDef", 2);

            try {
                const char *image = checker.checkstring("image");
                block_def->sprite.setTexture(lua_interface->game->gfx.textures.get(image));

                static const char *draw_type_names[] = {
                    "None",
                    "Image",
                    "Autotile",
                    "AutotilePlatform",
                    "Multiblock",
                    nullptr,
                };

                static const BlockDef::DrawType draw_type_values[] = {
                    BlockDef::DrawType::None,
                    BlockDef::DrawType::Image,
                    BlockDef::DrawType::Autotile,
                    BlockDef::DrawType::AutotilePlatform,
                    BlockDef::DrawType::Multiblock,
                };

                block_def->draw_type = draw_type_values[checker.checkoption("draw_type", "None", draw_type_names)];

                block_def->autotile_neighbour = checker.checkboolean("autotile_neighbour");

                block_def->autotile_single = checker.checkboolean("autotile_single");

                float slippery = checker.checknumber_range("slippery", 0, 1);

                block_def->slippery = slippery;

                block_def->is_solid = checker.checkboolean("solid");

                block_def->is_platform = checker.checkboolean("platform");

                block_def->is_interactive = checker.checkboolean("interactive");

                block_def->blocks_light = checker.checkboolean("blocks_light");

                sf::Color color = checker.checkcolor("light");
                block_def->light = sf::Vector3i(color.r, color.g, color.b);

                lua_pushinteger(L, lua_interface->game->block_defs.add(block_def));
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

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

        int get_multiblock_origin(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);

            LuaUtil::push_vector2i(L, lua_interface->game->world.getMultiblockOrigin(x, y));

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

        int set_multiblock_offset(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);
            uint8_t xoff = LuaUtil::checkinteger_ranged<uint8_t>(L, 3);
            uint8_t yoff = LuaUtil::checkinteger_ranged<uint8_t>(L, 4);

            lua_interface->game->world.setMultiblockOffset(x, y, xoff, yoff);

            return 0;
        }

        int set_multiblock(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);
            uint8_t width = LuaUtil::checkinteger_ranged<uint8_t>(L, 3);
            uint8_t height = LuaUtil::checkinteger_ranged<uint8_t>(L, 4);
            blockid block_id = LuaUtil::checkinteger_ranged<blockid>(L, 5);

            lua_interface->game->world.setMultiblock(x, y, width, height, block_id);

            return 0;
        }

        int start_block_timer(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);
            double timer = luaL_checknumber(L, 3);

            lua_interface->game->world.startBlockTimer(x, y, timer);

            return 0;
        }

        int start_wall_timer(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);
            double timer = luaL_checknumber(L, 3);

            lua_interface->game->world.startWallTimer(x, y, timer);

            return 0;
        }

        int average_block_color(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            blockid block_id = LuaUtil::checkinteger_ranged<blockid>(L, 1);

            sf::Color color = lua_interface->game->block_defs.getOrUnknown(block_id).getAverageColor();

            // TODO - create LuaUtil::pushcolor or something

            lua_createtable(L, 0, 3);

            lua_pushinteger(L, color.r);
            lua_setfield(L, -2, "r");

            lua_pushinteger(L, color.g);
            lua_setfield(L, -2, "g");

            lua_pushinteger(L, color.b);
            lua_setfield(L, -2, "b");

            return 1;
        }

        int get_block_inventory(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            int x = luaL_checkinteger(L, 1);
            int y = luaL_checkinteger(L, 2);

            LuaInventoryAPI::push_inventory(L, lua_interface->game->inventories.getOrCreate(sf::Vector2i(x, y)));

            return 1;
        }

    }

}
