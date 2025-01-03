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

#include <stdexcept>
#include <string>
#include <list>
#include <functional>
#include <system_error>
#include <iostream>

#include "lua_interface.hpp"
#include "lua_mod_config.hpp"
#include "../common/lua_util.hpp"
#include "../common/lua_datafile.hpp"
#include "../common/lua_sandbox.hpp"
#include "lua_entity.hpp"
#include "lua_particles.hpp"
#include "lua_item.hpp"
#include "lua_crafting.hpp"
#include "lua_inventory.hpp"
#include "lua_block.hpp"
#include "lua_player.hpp"
#include "lua_cmd.hpp"
#include "lua_chat.hpp"
#include "lua_mapgen.hpp"
#include "lua_hud_bar.hpp"
#include "lua_sound.hpp"
#include "lua_save.hpp"
#include "../../../utils/path_guard.hpp"
#include "../../../world/world.hpp"

namespace Terrarium {

    LuaModdingInterface::LuaModdingInterface(std::shared_ptr<GameState> game):
        ModdingInterface(game), L(luaL_newstate())
    {
        LuaSandbox::openlibs(L);

        lua_newtable(L);
        lua_setglobal(L, "core");

        LuaDatafileAPI::init(L);

        LuaSaveAPI::init(*this);
        LuaEntityAPI::init(*this);
        LuaParticleAPI::init(*this);
        LuaItemAPI::init(*this);
        LuaCraftingAPI::init(*this);
        LuaInventoryAPI::init(*this);
        LuaBlockAPI::init(*this);
        LuaMapgenAPI::init(*this);
        LuaPlayerAPI::init(*this);
        LuaCmdAPI::init(*this);
        LuaChatAPI::init(*this);
        LuaHudBarAPI::init(*this);
        LuaSoundAPI::init(*this);

        // Maybe change it to wrappers path later
        LuaSandbox::set_dofile_path(L, fs::current_path());

        if (!LuaUtil::run_script(L, "wrappers/init.lua")) {
            throw std::runtime_error("could not initialize lua wrappers");
        }

        lua_getglobal(L, "vec2");
        lua_getfield(L, -1, "new");

        lua_setfield(L, LUA_REGISTRYINDEX, "vec2.new");

        lua_pop(L, 1);

        loadMods("mods");
    }

    void LuaModdingInterface::update(float dtime) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_update");

        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L, dtime);

            // Pops _update and dtime
            if (LuaUtil::pcall(L, 1, 0) != LUA_OK) {
                LuaUtil::error(L, "core._update() generated a lua error");
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

                case Event::ModCmd:
                {
                    pushModCmdEvent(*event.cmd);
                    lua_setfield(L, -2, "mod_cmd");
                }
                break;

                case Event::ChatCmd:
                {
                    pushModCmdEvent(*event.cmd);
                    lua_setfield(L, -2, "chat_cmd");
                }
                break;

                case Event::BlockActivate:
                {
                    pushBlockEvent(*event.block);
                    lua_setfield(L, -2, "block_event");
                }
                break;
            }

            // Pops _on_event and event table
            if (LuaUtil::pcall(L, 1, 0) != LUA_OK) {
                LuaUtil::error(L, "core._on_event() generated a lua error");
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
            if (LuaUtil::pcall(L, 0, 1) != LUA_OK) {
                LuaUtil::error(L, "core._get_mapgen_data() generated a lua error");
            } else {
                if (!lua_istable(L, -1)) {
                    throw std::runtime_error("core._get_mapgen_data() should return table");
                }

                LuaUtil::FieldChecker checker(L, "MapgenData", -1);

                checker.rfield("filler", [&] () {
                    LuaUtil::FieldChecker tile_checker(L, "Tile", -1);
                    mapgen.setFiller(LuaMapgenAPI::checktile(tile_checker));
                });

                checker.rfield("biomes", [&] () {
                    lua_pushnil(L);
                    while (lua_next(L, -2) != 0) {
                        LuaUtil::FieldChecker biome_checker(L, "BiomeDef", -1);
                        mapgen.addBiome(LuaMapgenAPI::checkbiome(biome_checker));

                        lua_pop(L, 1);
                    }
                });

                checker.rfield("ores", [&] () {
                    lua_pushnil(L);
                    while (lua_next(L, -2) != 0) {
                        LuaUtil::FieldChecker ore_checker(L, "OreDef", -1);
                        mapgen.addOre(LuaMapgenAPI::checkore(ore_checker));

                        lua_pop(L, 1);
                    }
                });

                lua_pop(L, 1); // "mapgen data" table
            }


        } else {
            lua_pop(L, 1); // pops _get_mapgen_data
        }

        lua_pop(L, 1); // pops core
    }

    void LuaModdingInterface::onPlayerJoin(std::shared_ptr<Player> player) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_on_player_join");

        if (lua_isfunction(L, -1)) {
            LuaPlayerAPI::push_player(L, player);

            // Pops _on_player_join and player
            if (LuaUtil::pcall(L, 1, 0) != LUA_OK) {
                LuaUtil::error(L, "core._on_player_join() generated a lua error");
            }
        } else {
            lua_pop(L, 1); // pops _on_player_join
        }

        lua_pop(L, 1); // pops core
    }

    void LuaModdingInterface::onBlocksTicked(const std::list<World::TickedBlock> &ticked) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_on_block_timer");

        if (lua_isfunction(L, -1)) {
            for (const auto &block: ticked) {
                lua_pushvalue(L, -1);
                lua_pushinteger(L, block.x);
                lua_pushinteger(L, block.y);
                lua_pushinteger(L, block.id);
                lua_pushboolean(L, block.is_wall);

                // Pops function and arguments
                if (LuaUtil::pcall(L, 4, 0) != LUA_OK) {
                    LuaUtil::error(L, "core._on_block_timer() generated a lua error");
                }
            }
        }

        lua_pop(L, 1); // pops _on_block_timer

        lua_pop(L, 1); // pops core
    }

    void LuaModdingInterface::onMapgenFinish() {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_on_mapgen_finish");

        if (lua_isfunction(L, -1)) {
            if (LuaUtil::pcall(L, 0, 0) != LUA_OK) {
                LuaUtil::error(L, "core._on_mapgen_finish() generated a lua error");
            }
        } else {
            lua_pop(L, 1);
        }

        lua_pop(L, 1);
    }

    void LuaModdingInterface::load() {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_load");

        if (lua_isfunction(L, -1)) {
            if (LuaUtil::pcall(L, 0, 0) != LUA_OK) {
                LuaUtil::error(L, "core._load() generated a lua error");
            }
        } else {
            lua_pop(L, 1);
        }

        lua_pop(L, 1);
    }

    void LuaModdingInterface::save() {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_save");

        if (lua_isfunction(L, -1)) {
            if (LuaUtil::pcall(L, 0, 0) != LUA_OK) {
                LuaUtil::error(L, "core._save() generated a lua error");
            }
        } else {
            lua_pop(L, 1);
        }

        lua_pop(L, 1);
    }

    void LuaModdingInterface::loadMods(const fs::path &path) {
        const fs::path working_dir = fs::current_path();

        PathGuard guard(working_dir);

        const fs::path mods_path = working_dir / path;

        std::error_code ec;

        if (!fs::is_directory(mods_path, ec)) {
            std::string msg = "Terrarium::LuaModdingInterface::loadMods: cannot open ";
            msg += mods_path.string();
            msg += ": ";
            msg += ec.message();
            throw std::runtime_error(msg);
        }

        std::vector<LuaModConfig> mods_unsorted;

        // Load configs
        for (const auto &entry: fs::directory_iterator(mods_path)) {
            if (entry.is_directory(ec)) {
                try {
                    mods_unsorted.push_back(load_config(entry.path()));
                } catch (const std::runtime_error &e) {
                    std::cerr<<"Terrarium::LuaModdingInterface::loadMods: cannot open ";
                    std::cerr<<entry;
                    std::cerr<<": "<<e.what()<<std::endl;
                }
            }
        }

        // This dependency resolving algorithm is pretty much copy of one from
        // minetest.

        // First, make a set of mod names
        std::set<std::string> mod_names;
        for (const auto &mod: mods_unsorted) {
            mod_names.insert(mod.name);
        }

        // Now add dependencies that are going to be checked, including optional
        std::list<std::reference_wrapper<LuaModConfig>> satisfied, unsatisfied;
        for (auto &mod: mods_unsorted) {
            mod.unsatisfied_depends = mod.depends;

            for (const auto &mod_name: mod.optional_depends) {
                if (mod_names.count(mod_name) != 0) {
                    mod.unsatisfied_depends.insert(mod_name);
                }
            }

            if (mod.unsatisfied_depends.empty()) {
                satisfied.push_back(mod);
            } else {
                unsatisfied.push_back(mod);
            }
        }

        // Finally, add mods without unsatisfied dependencies into this
        // sorted_mods list. Each time all other mods remove this 'satisfied'
        // one from unsatisfied list. If all mods have their dependencies
        // satisfied, after some time both satisfied and unsatisfied lists will
        // become empty. If some mod misses its dependency, it will be left in
        // the unsatisfied list.
        std::list<std::reference_wrapper<LuaModConfig>> sorted_mods;
        while (!satisfied.empty()) {
            LuaModConfig &mod = satisfied.back();
            sorted_mods.push_back(mod);
            satisfied.pop_back();

            for (auto it = unsatisfied.begin(); it != unsatisfied.end();) {
                LuaModConfig &other_mod = it->get();

                other_mod.unsatisfied_depends.erase(mod.name);

                // Hooray, this mod have all its dependencies satisfied, now
                // we can add it to the satisfied list!
                if (other_mod.unsatisfied_depends.empty()) {
                    satisfied.push_back(other_mod);

                    // Remove this mod from unsatisfied list and pick next one
                    it = unsatisfied.erase(it);
                } else {
                    ++it;
                }
            }
        }

        if (!unsatisfied.empty()) {
            std::cerr<<"Terrarium::LuaModdingInterface::loadMods: "
                     <<"mods with unsatisfied dependencies detected"<<std::endl;

            for (auto mod_ref: unsatisfied) {
                LuaModConfig &mod = mod_ref.get();

                std::cerr<<"Mod: "<<mod.name<<" ("<<mod.root<<")"<<std::endl;
                std::cerr<<"Missing dependencies:"<<std::endl;

                for (auto &dependency_name: mod.unsatisfied_depends) {
                    std::cerr<<'\t'<<dependency_name<<std::endl;
                }
            }

            throw std::runtime_error("Terrarium::LuaModdingInterface::loadMods: mods with unsatisfied dependencies detected");
        }

        for (auto mod_ref: sorted_mods) {
            LuaModConfig &mod = mod_ref.get();

            try {
                fs::current_path(mod.root);
                const fs::path textures_dir = mod.root / "textures";

                // If there is textures directory, load it
                if (fs::is_directory(textures_dir, ec)) {
                    game->gfx.textures.addSearchPath(textures_dir);

                    for (const auto &texture_path: fs::directory_iterator(textures_dir)) {
                        game->gfx.textures.load(texture_path.path().filename().string());
                    }
                }

                const fs::path sounds_dir = mod.root / "sounds";

                // If there is sounds directory, load it
                if (fs::is_directory(sounds_dir, ec)) {
                    game->sfx.sounds.addSearchPath(sounds_dir);

                    for (const auto &sound_path: fs::directory_iterator(sounds_dir)) {
                        game->sfx.sounds.load(sound_path.path().filename().string());
                    }
                }

                const fs::path music_dir = mod.root / "music";

                // If there is music directory, load it
                if (fs::is_directory(music_dir, ec)) {
                    game->sfx.music.addSearchPath(music_dir);

                    for (const auto &music_path: fs::directory_iterator(music_dir)) {
                        game->sfx.music.open(music_path.path().filename().string());
                    }
                }

                // This may look really stupid, loading client scripts while in
                // "server" environment, but later resources will be downloaded
                // from server and loaded from some kind of "download" directory
                const fs::path client_scripts_dir = mod.root / "client";

                // If there is scripts directory, load it
                if (fs::is_directory(client_scripts_dir, ec)) {
                    game->client_modding_interface->loadScript(client_scripts_dir / "init.lua");
                }

                LuaSandbox::set_dofile_path(L, fs::absolute(mod.root));

                if (!LuaUtil::run_script(L, (mod.root / "init.lua").string().c_str())) {
                    throw std::runtime_error("Terrarium::LuaModdingInterface::loadMods: unexpected error when loading mod");
                }
            } catch (const fs::filesystem_error &e) {
                std::cerr<<"Terrarium::LuaModdingInterface::loadMods: cannot open ";
                std::cerr<<mod.root;
                std::cerr<<": "<<e.what()<<std::endl;

                throw std::runtime_error("Terrarium::LuaModdingInterface::loadMods: unexpected error when loading mod");
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

    void LuaModdingInterface::pushModCmdEvent(ModCmdEvent &cmd_event) {
        lua_newtable(L);

        lua_pushstring(L, cmd_event.name.c_str());
        lua_setfield(L, -2, "name");

        size_t argv = cmd_event.args.size();
        lua_createtable(L, argv, 0);
        for (size_t i = 1; i <= argv; ++i) {
            lua_pushstring(L, cmd_event.args[i-1].c_str());
            lua_seti(L, -2, i);
        }
        lua_setfield(L, -2, "args");

        LuaPlayerAPI::push_player(L, cmd_event.user);
        lua_setfield(L, -2, "user");
    }

    void LuaModdingInterface::pushBlockEvent(BlockEvent &block_event) {
        lua_newtable(L);

        LuaUtil::push_vector2i(L, block_event.position);
        lua_setfield(L, -2, "position");

        LuaPlayerAPI::push_player(L, block_event.user);
        lua_setfield(L, -2, "user");
    }

} // namespace Terrarium
