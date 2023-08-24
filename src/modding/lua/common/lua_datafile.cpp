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

#include <sstream>
#include <iostream>

#include "lua_datafile.hpp"

namespace Terrarium {

    namespace LuaDatafileAPI {

        void init(lua_State *L) {
            lua_newtable(L);

            lua_pushcfunction(L, from_string);
            lua_setfield(L, -2, "_from_string");

            lua_pushcfunction(L, to_string);
            lua_setfield(L, -2, "_to_string");

            lua_setglobal(L, "datafile");
        }

        int from_string(lua_State *L) {
            const char *str = luaL_checkstring(L, 1);

            char listSep = ',';

            if (lua_isstring(L, 2) && luaL_len(L, 2) > 0) {
                listSep = lua_tostring(L, 2)[0];
            }

            Datafile datafile;

            std::istringstream iss(str);

            datafile.loadFromStream(iss, listSep);

            datafile_to_table(L, datafile);

            return 1;
        }

        int to_string(lua_State *L) {
            Datafile datafile;

            table_to_datafile(L, 1, datafile);

            const char *indent = "";

            if (lua_isstring(L, 2)) {
                indent = lua_tostring(L, 2);
            }

            char listSep = ',';

            if (lua_isstring(L, 3) && luaL_len(L, 3) > 0) {
                listSep = lua_tostring(L, 3)[0];
            }

            std::ostringstream oss;

            datafile.saveToStream(oss, indent, listSep);

            lua_pushstring(L, oss.str().c_str());

            return 1;
        }

        void datafile_to_table(lua_State *L, const Datafile &datafile) {
            lua_createtable(L, 0, 2);

            lua_newtable(L);
            lua_setfield(L, -2, "children");

            lua_newtable(L);
            lua_setfield(L, -2, "children_ids");

            const auto &children = datafile.getChildren();
            for (size_t i = 0; i < children.size(); ++i) {
                auto pair = children[i];

                lua_getfield(L, -1, "children_ids");
                lua_pushinteger(L, i+1);
                lua_setfield(L, -2, pair.first.c_str()); // ["field_name"] = i+1
                lua_pop(L, 1); // Pop children_ids

                lua_getfield(L, -1, "children");
                lua_createtable(L, 2, 0); // { "field_name", data }
                lua_pushstring(L, pair.first.c_str());
                lua_rawseti(L, -2, 1);
                datafile_to_table(L, pair.second);
                lua_rawseti(L, -2, 2);
                lua_rawseti(L, -2, i+1);
                lua_pop(L, 1);
            }

            lua_createtable(L, datafile.getValueCount(), 0);
            for (size_t i = 0; i < datafile.getValueCount(); ++i) {
                lua_pushstring(L, datafile.getString(i).c_str());
                lua_rawseti(L, -2, i+1);
            }
            lua_setfield(L, -2, "content");
        }

        void table_to_datafile(lua_State *L, int idx, Datafile &datafile) {
            idx = lua_absindex(L, idx);

            lua_getfield(L, idx, "content");
            lua_Integer size = luaL_len(L, -1);
            for (lua_Integer i = 1; i <= size; ++i) {
                lua_rawgeti(L, -1, i);

                datafile.setString(lua_tostring(L, -1), i-1);

                lua_pop(L, 1);
            }
            lua_pop(L, 1);

            lua_getfield(L, idx, "children");
            size = luaL_len(L, -1);
            for (lua_Integer i = 1; i <= size; ++i) {
                lua_rawgeti(L, -1, i); // Push pair { "field_name", data }

                lua_rawgeti(L, -1, 1); // Get name
                std::string name = lua_tostring(L, -1);
                lua_pop(L, 1);

                Datafile &child = datafile[name];

                lua_rawgeti(L, -1, 2); // Get data
                table_to_datafile(L, -1, child);
                lua_pop(L, 1);

                lua_pop(L, 1);
            }
            lua_pop(L, 1);
        }

    }

}
