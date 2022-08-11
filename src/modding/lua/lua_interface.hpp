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

#ifndef MODDING_LUA_INTERFACE_HPP
#define MODDING_LUA_INTERFACE_HPP

#include <filesystem>

#include <lua.hpp>

#include "../interface.hpp"
#include "../../game.hpp"

namespace fs = std::filesystem;

namespace Terrarium {

    class LuaModdingInterface: public ModdingInterface {
        lua_State *L = nullptr;

        void pushItemEvent(ItemEvent &item_event);
        void pushUIEvent(UIEvent &ui_event);
        void pushBlockEvent(BlockEvent &block_event);

    public:
        LuaModdingInterface(std::shared_ptr<GameState> game);

        void update(float dtime) override;
        void handleEvent(Event &event) override;
        void initMapgen(MapgenBase &mapgen) override;
        void onPlayerJoin(std::shared_ptr<Player> player) override;

        // Makes closure with 1 upvalue - `this` pointer and leaves it on stack top
        void pushClosure(lua_CFunction fn);

        // Adds a function into core module in lua. Function is pushed as C closure,
        // with 1 upvalue - light userdata, which is `this` pointer
        void registerFunction(const char *name, lua_CFunction fn);

        void loadMods(const fs::path &path);

        inline lua_State *getLuaState() { return L; }
    };

} // namespace Terrarium

#endif
