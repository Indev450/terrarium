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
#ifndef MODDING_LUA_CLIENT_INTERFACE_HPP
#define MODDING_LUA_CLIENT_INTERFACE_HPP

#include <queue>

#include <lua.hpp>

#include "../../client_interface.hpp"

namespace Terrarium {

    class LuaClientModdingInterface: public ClientModdingInterface {
        lua_State *L = nullptr;

        void pushModCmdEvent(ModCmdEvent &cmd_event);
    public:
        LuaClientModdingInterface(std::shared_ptr<GameState> game);

        // Makes closure with 1 upvalue - `this` pointer and leaves it on stack top
        void pushClosure(lua_CFunction fn);

        // Adds a function into core module in lua. Function is pushed as C closure,
        // with 1 upvalue - light userdata, which is `this` pointer
        void registerFunction(const char *name, lua_CFunction fn);

        lua_State *getLuaState() { return L; }

        void loadScript(const fs::path &path) override;

        void update(float dtime) override;
        bool ui_click(const std::string &id, const sf::Vector2f &position) override;
        bool ui_scroll(const std::string &id, const sf::Vector2f &position, float delta) override;
        void ui_render(const std::string &id) override;

        void handleModCmd(ModCmdEvent &cmd_event) override;
    };

}

#endif
