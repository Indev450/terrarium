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

#include "lua_client_interface.hpp"

// TODO - create 'common' directory for this
#include "../server/lua_util.hpp"

#include "lua_cmd.hpp"
#include "lua_rendering.hpp"
#include "lua_ui.hpp"

namespace Terrarium {

    LuaClientModdingInterface::LuaClientModdingInterface(std::shared_ptr<GameState> game):
        ClientModdingInterface(game), L(luaL_newstate())
    {
        // TODO - open only safe libs
        luaL_openlibs(L);

        lua_newtable(L);
        lua_setglobal(L, "core");

        LuaClientCmdAPI::init(*this);
        LuaClientRenderingAPI::init(*this);
        LuaClientUIAPI::init(*this);

        if (!LuaUtil::run_script(L, "client_wrappers/init.lua")) {
            throw std::runtime_error("could not initialize client lua wrappers");
        }

        lua_getglobal(L, "vec2");
        lua_getfield(L, -1, "new");

        lua_setfield(L, LUA_REGISTRYINDEX, "vec2.new");

        lua_pop(L, 1);
    }

    void LuaClientModdingInterface::pushClosure(lua_CFunction fn) {
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, fn, 1);
    }

    void LuaClientModdingInterface::registerFunction(const char *name, lua_CFunction fn) {
        lua_getglobal(L, "core");

        pushClosure(fn);
        lua_setfield(L, -2, name);

        lua_pop(L, 1); // pop core
    }

    void LuaClientModdingInterface::loadScript(const fs::path &path) {
        if (!LuaUtil::run_script(L, path.string().c_str())) {
            throw std::runtime_error("LuaClientModdingInterface::loadMod: unexpected error when loading client mod");
        }
    }

    void LuaClientModdingInterface::update(float dtime) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_update");

        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L, dtime);

            // Pops _update and dtime
            if (LuaUtil::pcall(L, 1, 0) != LUA_OK) {
                LuaUtil::error(L, "client-side core._update() generated a lua error");
            }
        } else {
            lua_pop(L, 1); // pops _update
        }

        lua_pop(L, 1); // pop 'core'
    }

    bool LuaClientModdingInterface::ui_click(const std::string &id, const sf::Vector2f &position) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_ui_click");

        bool result = false;

        if (lua_isfunction(L, -1)) {
            lua_pushstring(L, id.c_str());
            LuaUtil::push_vector2f(L, position);

            // Pops _ui_click, id and position, pushes bool
            if (LuaUtil::pcall(L, 2, 1) != LUA_OK) {
                LuaUtil::error(L, "client-side core._ui_click() generated a lua error");
            }

            result = lua_toboolean(L, -1);

            lua_pop(L, 1); // pops boolean
        } else {
            lua_pop(L, 1); // pops _ui_click
        }

        lua_pop(L, 1); // pop 'core'

        return result;
    }

    bool LuaClientModdingInterface::ui_scroll(const std::string &id, const sf::Vector2f &position, float delta) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_ui_scroll");

        bool result = false;

        if (lua_isfunction(L, -1)) {
            lua_pushstring(L, id.c_str());
            LuaUtil::push_vector2f(L, position);
            lua_pushnumber(L, delta);

            // Pops _ui_scroll, id, position and delta, pushes bool
            if (LuaUtil::pcall(L, 3, 1) != LUA_OK) {
                LuaUtil::error(L, "client-side core._ui_scroll() generated a lua error");
            }

            result = lua_toboolean(L, -1);

            lua_pop(L, 1); // pops boolean
        } else {
            lua_pop(L, 1); // pops _ui_scroll
        }

        lua_pop(L, 1); // pop 'core'

        return result;
    }

    void LuaClientModdingInterface::ui_render(const std::string &id) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_ui_render");

        if (lua_isfunction(L, -1)) {
            lua_pushstring(L, id.c_str());

            // Pops _ui_click, id and target
            if (LuaUtil::pcall(L, 1, 0) != LUA_OK) {
                LuaUtil::error(L, "client-side core._ui_render() generated a lua error");
            }
        } else {
            lua_pop(L, 1); // pops _ui_render
        }

        lua_pop(L, 1); // pop 'core'
    }

    void LuaClientModdingInterface::handleModCmd(ModCmdEvent &cmd_event) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_mod_cmd");

        if (lua_isfunction(L, -1)) {
            pushModCmdEvent(cmd_event);

            // Pops _mod_cmd and cmd_event
            if (LuaUtil::pcall(L, 1, 0) != LUA_OK) {
                LuaUtil::error(L, "client-side core._mod_cmd() generated a lua error");
            }
        } else {
            lua_pop(L, 1); // pops _mod_cmd
        }

        lua_pop(L, 1); // pop 'core'
    }

    void LuaClientModdingInterface::pushModCmdEvent(ModCmdEvent &cmd_event) {
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

        // Doesn't push player because here commands always come from server,
        // and we always have access to local player
    }
}
