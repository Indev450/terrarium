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

#include <stdexcept>
#include <string>
#include <system_error>
#include <iostream>

#include "lua_interface.hpp"
#include "lua_util.hpp"
#include "lua_entity.hpp"
#include "lua_item.hpp"
#include "lua_block.hpp"
#include "lua_player.hpp"
#include "lua_mapgen.hpp"

#include "../../utils/path_guard.hpp"

namespace Terrarium {

    LuaModdingInterface::LuaModdingInterface(std::shared_ptr<GameState> game):
        ModdingInterface(game), L(luaL_newstate())
    {
        luaL_openlibs(L);

        lua_newtable(L);
        lua_setglobal(L, "core");

        LuaEntityAPI::init(*this);
        LuaItemAPI::init(*this);
        LuaBlockAPI::init(*this);
        LuaPlayerAPI::init(*this);

        if (!LuaUtil::run_script(L, "wrappers/init.lua")) {
            throw std::runtime_error("could not initialize lua wrappers");
        }

        loadMods("mods");
    }

    void LuaModdingInterface::update(float dtime) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_update");

        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L, dtime);

            // Pops _update and dtime
            if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
                LuaUtil::printerr(L);
            }
        } else {
            lua_pop(L, 1); // pops _update
        }

        lua_pop(L, 1); // pop 'core'
    }

    void LuaModdingInterface::handleEvent(Event &event) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_on_event");

        if (lua_isfunction(L, -1)) {
            lua_newtable(L); // push event table

            lua_pushstring(L, event.getName());
            lua_setfield(L, -2, "name");

            switch (event.type) {
                case Event::ItemUseStart:
                case Event::ItemUseStop:
                case Event::ItemAltUseStart:
                case Event::ItemAltUseStop:
                case Event::ItemSelect:
                {
                    pushItemEvent(*event.item);
                    lua_setfield(L, -2, "item_event");
                }
                break;
            }

            // Pops _on_event and event table
            if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
                LuaUtil::printerr(L);
            }

        } else {
            lua_pop(L, 1); // pops _on_event
        }

        lua_pop(L, 1); // pops core
    }

    void LuaModdingInterface::initMapgen(MapgenBase &mapgen) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_get_mapgen_data");

        if (lua_isfunction(L, -1)) {
            if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
                LuaUtil::printerr(L);
            } else {
                if (!lua_istable(L, -1)) {
                    throw std::runtime_error("core._get_mapgen_data() should return table");
                }

                lua_getfield(L, -1, "filler");
                mapgen.setFiller(LuaMapgenAPI::checktile(L, -1));
                lua_pop(L, 1);

                lua_getfield(L, -1, "biomes");

                if (!lua_istable(L, -1)) {
                    throw std::runtime_error("table returned by core._get_mapgen_data() should have 'biomes' table");
                }

                lua_pushnil(L);
                while (lua_next(L, -2) != 0) {
                    mapgen.addBiome(LuaMapgenAPI::checkbiome(L, -1));

                    lua_pop(L, 1);
                }

                lua_pop(L, 2); // pops biomes and "mapgen data" table
            }


        } else {
            lua_pop(L, 1); // pops _get_mapgen_data
        }

        lua_pop(L, 1); // pops core
    }

    void LuaModdingInterface::loadMods(const fs::path &path) {
        const fs::path working_dir = fs::current_path();

        PathGuard guard(working_dir);

        const fs::path mods_path = working_dir / path;

        std::error_code ec;

        if (!fs::is_directory(mods_path, ec)) {
            std::string msg = "Terrarium::LuaModdingInterface::loadMods: cannot open ";
            msg += mods_path.c_str();
            msg += ": ";
            msg += ec.message();
            throw std::runtime_error(msg);
        }

        for (const auto &entry: fs::directory_iterator(mods_path)) {
            if (entry.is_directory(ec)) {
                try {
                    fs::current_path(entry);

                    const fs::path textures_dir = entry.path() / "textures";

                    // If there is textures directory, load it
                    if (fs::is_directory(textures_dir, ec)) {
                        game->gfx.textures.addSearchPath(textures_dir);

                        for (const auto &texture_path: fs::directory_iterator(textures_dir)) {
                            game->gfx.textures.load(texture_path.path().filename());
                        }
                    }

                    LuaUtil::run_script(L, (entry.path() / "init.lua").c_str());
                } catch (const fs::filesystem_error &e) {
                    std::cerr<<"Terrarium::LuaModdingInterface::loadMods: cannot open ";
                    std::cerr<<entry;
                    std::cerr<<": "<<e.what()<<std::endl;
                }
            } else {
                std::cerr<<"Terrarium::LuaModdingInterface::loadMods: cannot open ";
                std::cerr<<entry;
                std::cerr<<": "<<ec.message()<<std::endl;
            }
        }
    }

    void LuaModdingInterface::pushClosure(lua_CFunction fn) {
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, fn, 1);
    }

    void LuaModdingInterface::registerFunction(const char *name, lua_CFunction fn) {
        lua_getglobal(L, "core");

        pushClosure(fn);
        lua_setfield(L, -2, name);

        lua_pop(L, 1); // pop core
    }

    void LuaModdingInterface::pushItemEvent(ItemEvent &item_event) {
        lua_newtable(L);

        // item_event.def always valid, so we able to skip null-checks
        LuaItemAPI::push_itemstack(L, item_event.item_stack);
        lua_setfield(L, -2, "item_stack");

        LuaPlayerAPI::push_player(L, item_event.user);
        lua_setfield(L, -2, "user");

        LuaUtil::push_vector2f(L, item_event.position);
        lua_setfield(L, -2, "position");
    }

} // namespace Terrarium
