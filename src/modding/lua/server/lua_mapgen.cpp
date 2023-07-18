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

#include "lua_interface.hpp"
#include "lua_mapgen.hpp"
#include "lua_util.hpp"

namespace Terrarium {

    namespace LuaMapgenAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_get_world_size", get_world_size);
        }

        // Vector2u core._get_world_size()
        int get_world_size(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            sf::Vector2i world_size(
                    lua_interface->game->world.getWidth(),
                    lua_interface->game->world.getHeight());
            LuaUtil::push_vector2i(L, world_size);

            return 1;
        }

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

            lua_getfield(L, idx, "multiblock_offset");
            if (!lua_isnil(L, -1)) {
                tile.multiblock_offset = LuaUtil::checkvector<uint8_t>(L, -1);
            }
            lua_pop(L, 1);

            return tile;
        }

        Decoration checkdecor(lua_State *L, int idx) {
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            Decoration decor;

            idx = lua_absindex(L, idx);

            lua_getfield(L, idx, "origin");
            decor.origin = LuaUtil::checkvector<int>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "width");
            decor.width = LuaUtil::checkinteger_ranged<uint16_t>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "height");
            decor.height = LuaUtil::checkinteger_ranged<uint16_t>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "tiles");
            for (unsigned int i = 1; i <= decor.width * decor.height; ++i) {
                lua_geti(L, -1, i);

                decor.tiles.push_back(checktile(L, -1));

                lua_pop(L, 1);
            }
            lua_pop(L, 1);

            lua_getfield(L, idx, "conditions");
            size_t len = luaL_len(L, -1);
            for (size_t i = 1; i <= len; ++i) {
                lua_geti(L, -1, i);

                sf::Vector2i position;
                std::unique_ptr<PlaceConditions::Condition> fg, bg;

                lua_getfield(L, -1, "position");
                position = LuaUtil::checkvector<int>(L, -1);
                lua_pop(L, 1);

                lua_getfield(L, -1, "fg");
                fg = checkcondition(L, -1);
                lua_pop(L, 1);

                lua_getfield(L, -1, "bg");
                bg = checkcondition(L, -1);
                lua_pop(L, 1);

                decor.conditions.emplace_back(
                    TileCondition { position, std::move(fg), std::move(bg) }
                );

                lua_pop(L, 1);
            }
            lua_pop(L, 1);

            lua_getfield(L, idx, "place_chance");
            decor.place_chance = LuaUtil::checknumber_ranged(L, -1, 0, 1);
            lua_pop(L, 1);

            return decor;
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

            lua_getfield(L, idx, "min_depth");
            biome.min_depth = LuaUtil::checknumber_ranged(L, -1, -1.0, 1.0);
            lua_pop(L, 1);

            lua_getfield(L, idx, "max_depth");
            biome.max_depth = LuaUtil::checknumber_ranged(L, -1, -1.0, 1.0);
            lua_pop(L, 1);

            lua_getfield(L, idx, "priority");
            biome.priority = LuaUtil::checkinteger_ranged<unsigned int>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "decorations");
            if (!lua_istable(L, idx)) {
                luaL_error(L, "expected table as biome decorations");
            }

            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                biome.decorations.push_back(std::make_shared<Decoration>(checkdecor(L, -1)));
                lua_pop(L, 1);
            }

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

        std::unique_ptr<PlaceConditions::Condition> checkcondition(lua_State *L, int idx) {
            // Null conditions are valid
            if (lua_isnil(L, idx)) {
                return nullptr;
            }

            idx = lua_absindex(L, idx);

            static const char* conditions[] = {
                "AnySolidBlock",
                "AirBlock",
                "AnyOfBlocks",
                "NoneOfBlocks",
                "ExactBlock",
                nullptr
            };

            lua_getfield(L, idx, "type");
            int condition_type = luaL_checkoption(L, -1, "AnySolidBlock", conditions);
            lua_pop(L, 1);

            switch (condition_type) {
                case 0: return std::make_unique<PlaceConditions::AnySolidBlock>();

                case 1: return std::make_unique<PlaceConditions::AirBlock>();

                case 2:
                case 3:
                {
                    std::unordered_set<blockid> blocks;

                    lua_getfield(L, idx, "blocks");
                    size_t len = luaL_len(L, -1);
                    for (size_t i = 1; i <= len; ++i) {
                        lua_geti(L, -1, i);

                        blocks.insert(LuaUtil::checkinteger_ranged<blockid>(L, -1));

                        lua_pop(L, 1);
                    }
                    lua_pop(L, 1);

                    if (condition_type == 2)
                        return std::make_unique<PlaceConditions::AnyOfBlocks>(blocks);

                    return std::make_unique<PlaceConditions::NoneOfBlocks>(blocks);
                }

                case 4:
                {
                    lua_getfield(L, idx, "block");
                    blockid block = LuaUtil::checkinteger_ranged<blockid>(L, -1);
                    lua_pop(L, 1);

                    return std::make_unique<PlaceConditions::ExactBlock>(block);
                }

                default: luaL_error(L, "Unreachable statement reached in LuaMapgenAPI::checkcondition");
            }

            // luaL_error does long jump, so this statement will never be
            // executed
            return nullptr;
        }
    }

}
