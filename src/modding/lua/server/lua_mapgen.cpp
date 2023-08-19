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
#include "../common/lua_util.hpp"

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

        Tile checktile(LuaUtil::FieldChecker &checker) {
            checker.checkself();

            Tile tile;

            tile.fg = checker.checkunsigned("block");

            tile.bg = checker.checkunsigned("wall");

            if (checker.havefield("multiblock_offset")) {
                tile.multiblock_offset = checker.checkvector<uint8_t>("multiblock_offset");
            }

            return tile;
        }

        Decoration checkdecor(LuaUtil::FieldChecker &checker) {
            lua_State *L = checker.getState();
            LuaUtil::TopKeeper top_keeper(L);
            checker.checkself();

            Decoration decor;

            decor.origin = checker.checkvector<int>("origin");

            decor.width = checker.checkunsigned("width");

            decor.height = checker.checkunsigned("height");

            checker.getfield("tiles", LUA_TTABLE);
            for (unsigned int i = 1; i <= decor.width * decor.height; ++i) {
                lua_geti(L, -1, i);

                LuaUtil::FieldChecker tile_checker(L, "Tile", -1);

                try {
                    decor.tiles.push_back(checktile(tile_checker));
                } catch (const std::invalid_argument &e) {
                    checker.rfielderror("tiles", e.what());
                }

                lua_pop(L, 1);
            }
            lua_pop(L, 1);

            checker.getfield("conditions", LUA_TTABLE);
            size_t len = luaL_len(L, -1);
            for (size_t i = 1; i <= len; ++i) {
                lua_geti(L, -1, i);

                LuaUtil::FieldChecker place_cond_checker(L, "PlaceCondition", -1);

                sf::Vector2i position;
                std::unique_ptr<PlaceConditions::Condition> fg, bg;

                try {
                    position = place_cond_checker.checkvector<int>("position");

                    place_cond_checker.rfield("fg", [&] () {
                        LuaUtil::FieldChecker cond_checker(L, "Condition", -1);
                        fg = checkcondition(cond_checker);
                    }, true);

                    place_cond_checker.rfield("bg", [&] () {
                        LuaUtil::FieldChecker cond_checker(L, "Condition", -1);
                        bg = checkcondition(cond_checker);
                    }, true);

                    decor.conditions.emplace_back(
                        TileCondition { position, std::move(fg), std::move(bg) }
                    );
                } catch (const std::invalid_argument &e) {
                    checker.rfielderror("conditions", e.what());
                }

                lua_pop(L, 1);
            }
            lua_pop(L, 1);

            decor.place_chance = checker.checknumber_range("place_chance", 0, 1);

            return decor;
        }

        Biome checkbiome(LuaUtil::FieldChecker &checker) {
            lua_State *L = checker.getState();
            LuaUtil::TopKeeper top_keeper(L);
            checker.checkself();

            Biome biome;

            biome.humidity_min = checker.checknumber_range("humidity_min", -1.0, 1.0);

            biome.humidity_max = checker.checknumber_range("humidity_max", -1.0, 1.0);

            biome.heat_min = checker.checknumber_range("heat_min", -1.0, 1.0);

            biome.heat_max = checker.checknumber_range("heat_max", -1.0, 1.0);

            checker.getfield("top", LUA_TTABLE);
            LuaUtil::FieldChecker tile_checker(L, "Tile", -1);
            try {
                biome.top = checktile(tile_checker);
            } catch (const std::invalid_argument &e) {
                checker.rfielderror("top", e.what());
            }
            lua_pop(L, 1);

            biome.top_depth = checker.checkunsigned("top_depth");

            checker.getfield("filler", LUA_TTABLE);
            try {
                // Reuse tile_checker
                biome.filler = checktile(tile_checker);
            } catch (const std::invalid_argument &e) {
                checker.rfielderror("filler", e.what());
            }
            lua_pop(L, 1);

            biome.filler_depth = checker.checkunsigned("filler_depth");

            checker.getfield("stone", LUA_TTABLE);
            try {
                // Reuse tile_checker
                biome.stone = checktile(tile_checker);
            } catch (const std::invalid_argument &e) {
                checker.rfielderror("stone", e.what());
            }
            lua_pop(L, 1);

            biome.min_depth = checker.checknumber_range("min_depth", -1.0, 1.0);

            biome.max_depth = checker.checknumber_range("max_depth", -1.0, 1.0);

            biome.priority = checker.checkunsigned("priority");

            checker.getfield("decorations", LUA_TTABLE);

            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                LuaUtil::FieldChecker decor_checker(L, "Decoration", -1);

                try {
                    biome.decorations.push_back(std::make_shared<Decoration>(checkdecor(decor_checker)));
                } catch (const std::invalid_argument &e) {
                    checker.rfielderror("decorations", e.what());
                }
                lua_pop(L, 1);
            }

            lua_pop(L, 1);

            return biome;
        }

        Ore checkore(LuaUtil::FieldChecker &checker) {
            lua_State *L = checker.getState();
            LuaUtil::TopKeeper top_keeper(L);
            checker.checkself();

            Ore ore;

            ore.cluster_tiles = checker.checkunsigned("cluster_tiles");

            ore.distribution = checker.checkunsigned("distribution");

            ore.min_depth = checker.checknumber_range("min_depth", 0.0, 1.0);

            ore.max_depth = checker.checknumber_range("max_depth", 0.0, 1.0);

            checker.getfield("tile", LUA_TTABLE);
            LuaUtil::FieldChecker tile_checker(L, "Tile", -1);

            try {
                ore.tile = checktile(tile_checker);
            } catch (const std::invalid_argument &e) {
                checker.rfielderror("tile", e.what());
            }

            lua_pop(L, 1);

            return ore;
        }

        std::unique_ptr<PlaceConditions::Condition> checkcondition(LuaUtil::FieldChecker &checker) {
            lua_State *L = checker.getState();
            LuaUtil::TopKeeper top_keeper(L);

            checker.checkself();

            static const char* conditions[] = {
                "AnySolidBlock",
                "AirBlock",
                "AnyOfBlocks",
                "NoneOfBlocks",
                "ExactBlock",
                nullptr
            };

            int condition_type = checker.checkoption("type", "AnySolidBlock", conditions);

            switch (condition_type) {
                case 0: return std::make_unique<PlaceConditions::AnySolidBlock>();

                case 1: return std::make_unique<PlaceConditions::AirBlock>();

                case 2:
                case 3:
                {
                    std::unordered_set<blockid> blocks;

                    checker.getfield("blocks", LUA_TTABLE);
                    size_t len = luaL_len(L, -1);
                    for (size_t i = 1; i <= len; ++i) {
                        lua_geti(L, -1, i);

                        checker.fieldcheck("blocks", lua_isinteger(L, -1) && lua_tointeger(L, -1) >= 0, "expected array of unsigned integers");

                        blocks.insert(static_cast<blockid>(lua_tointeger(L, -1)));

                        lua_pop(L, 1);
                    }
                    lua_pop(L, 1);

                    if (condition_type == 2)
                        return std::make_unique<PlaceConditions::AnyOfBlocks>(blocks);

                    return std::make_unique<PlaceConditions::NoneOfBlocks>(blocks);
                }

                case 4:
                {
                    blockid block = checker.checkunsigned("block");
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
