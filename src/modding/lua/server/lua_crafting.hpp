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

#include <lua.hpp>

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaCraftingAPI {

        void init(LuaModdingInterface &lua_interface);

        // Crafting-related core module functions

        // void core._add_crafting_category(string name)
        int add_crafting_category(lua_State *L);

        // FIXME - function_name_is_way_too_long
        // void core._set_default_crafting_category(string name)
        int set_default_crafting_category(lua_State *L);

        // void core._register_recipe(string category, Recipe recipe)
        int register_recipe(lua_State *L);

        // void core._show_crafting_ui(string category)
        int show_crafting_ui(lua_State *L);

    }

}
