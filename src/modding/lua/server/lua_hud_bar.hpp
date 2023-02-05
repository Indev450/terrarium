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

#ifndef MODDING_LUA_HUD_BAR_HPP
#define MODDING_LUA_HUD_BAR_HPP

#include <lua.hpp>

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaHudBarAPI {

        void init(LuaModdingInterface &lua_interface);

        // Hud Bar-related core module functions

        // core._hud_bar_create(string name, BarDef bar_def)
        int hud_bar_create(lua_State *L);

        // core._hud_bar_set_value(string name, float value)
        int hud_bar_set_value(lua_State *L);

        // core._hud_bar_set_max_value(string name, float max_value)
        int hud_bar_set_max_value(lua_State *L);

        // core._hud_bar_remove(string name)
        int hud_bar_remove(lua_State *L);

    }

}

#endif
