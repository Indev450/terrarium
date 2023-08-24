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

#include <filesystem>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include "lua_interface.hpp"
#include "lua_save.hpp"
#include "../common/lua_sandbox.hpp"
#include "../../../game.hpp"

// Because lua doesn't expose that :/
namespace LuaIO {

    static int closef(lua_State *L) {
        luaL_Stream *s = (luaL_Stream*)((luaL_Stream*)luaL_checkudata(L, 1, LUA_FILEHANDLE));
        int res = fclose(s->f);
        return luaL_fileresult(L, (res == 0), nullptr);
    }

    static luaL_Stream *newprepfile(lua_State *L) {
        luaL_Stream *s = (luaL_Stream*)lua_newuserdatauv(L, sizeof(luaL_Stream), 0);
        s->closef = nullptr;
        luaL_setmetatable(L, LUA_FILEHANDLE);
        return s;
    }

    static luaL_Stream *newfile(lua_State *L) {
        luaL_Stream *s = newprepfile(L);
        s->f = nullptr;
        s->closef = closef;
        return s;
    }

    static int l_checkmode (const char *mode) {
        return (*mode != '\0' && strchr("rwa", *(mode++)) != NULL &&
             (*mode != '+' || ((void)(++mode), 1)) &&  /* skip if char is '+' */
             (strspn(mode, "b") == strlen(mode)));  /* check extensions */
    }


}

namespace Terrarium {

    namespace LuaSaveAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_open", open);
        }

        int open(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            fs::path allowed_path = fs::absolute(lua_interface->game->saves.getSavePath(lua_interface->game->save_name));

            // We expect relative path within saves directory
            fs::path relpath = luaL_checkstring(L, 1);

            std::optional<fs::path> path = LuaSandbox::securepath(allowed_path, relpath);

            if (!path) {
                return luaL_fileresult(L, 0, relpath.c_str());
            }

            const char *mode = luaL_optstring(L, 2, "r");

            const char *md = mode;
            luaL_argcheck(L, LuaIO::l_checkmode(md), 2, "invalid mode");

            // Create saves directory if it doesn't exist
            fs::create_directories(path->parent_path());

            luaL_Stream *s = LuaIO::newfile(L);

            s->f = fopen(path->c_str(), mode);

            return (s->f == nullptr) ? luaL_fileresult(L, 0, path->c_str()) : 1;
        }

    }

}
