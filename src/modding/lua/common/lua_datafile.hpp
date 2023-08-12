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

#include "../../../utils/datafile.hpp"

namespace Terrarium {

    namespace LuaDatafileAPI {

        void init(lua_State *L);

        // table datafile._from_file(string path)
        int from_file(lua_State *L);

        // table datafile._from_string(string str)
        int from_string(lua_State *L);

        // void datafile._to_file(string path, table data)
        int to_file(lua_State *L);

        // string datafile._to_string(table data)
        int to_string(lua_State *L);

        // Functions to call from C++

        // Convert datafile object to lua table. Note that it has same structure
        // as C++ version, you'll need a wrapper to actually use this table
        // properly.
        void datafile_to_table(lua_State *L, const Datafile &datafile);

        // Converts lua table to datafile object.
        void table_to_datafile(lua_State *L, int idx, Datafile &datafile);

    }

}
