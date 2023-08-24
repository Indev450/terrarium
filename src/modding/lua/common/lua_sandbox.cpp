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

#include <string>
#include <cassert>

#include "lua_sandbox.hpp"
#include "lua_util.hpp"

namespace Terrarium {

    namespace LuaSandbox {

        const char *DOFILE_PATH = "DOFILE_PATH";

        std::optional<fs::path> securepath(const fs::path &parent_dir, const fs::path &relpath) {
            // It *must* be absolute.
            assert(parent_dir.is_absolute());

            // We don't accept absolute path.
            if (!relpath.is_relative()) {
                return std::nullopt;
            }

            // Combine parent_dir and relpath, and remove all the .. for
            // security.
            fs::path path = (parent_dir / relpath).lexically_normal();

            std::string strpath(path);
            std::string strparentpath(parent_dir);

            // If parent path length is higher than requested file path length
            // then we are definitely somewhere outside of parent_dir.
            if (strparentpath.size() > strpath.size()) {
                return std::nullopt;
            }

            // If absolute path to required file doesn't start with allowed
            // path, that means it is somewhere outside of saves directory,
            // which is not allowed.
            if (strpath.compare(0, strparentpath.size(), strparentpath) != 0) {
                return std::nullopt;
            }

            // Ok file seems to be located in parent_dir, we can return it
            return path;
        }

        void set_dofile_path(lua_State *L, const fs::path &path) {
            assert(path.is_absolute());
            assert(fs::is_directory(path));

            lua_pushstring(L, path.c_str());
            lua_setfield(L, LUA_REGISTRYINDEX, DOFILE_PATH);
        }

        void openlibs(lua_State *L) {
            luaL_requiref(L, "base", luaopen_base, 0);
            sandbox_base(L);

            luaL_requiref(L, LUA_COLIBNAME, luaopen_coroutine, 1);
            luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);

            // tbh only want the file metatable but ok
            luaL_requiref(L, LUA_IOLIBNAME, luaopen_io, 0);

            luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
            luaL_requiref(L, LUA_UTF8LIBNAME, luaopen_utf8, 1);
            luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);

            // TODO - maybe allow loading debug library in some conditions
        }

        void sandbox_base(lua_State *L) {
            // If used incorrectly, can lead to memory leaks
            lua_pushnil(L);
            lua_setglobal(L, "collectgarbage");

            // Adds additional check if it is legal to execute specified script
            lua_pushcfunction(L, dofile);
            lua_setglobal(L, "dofile");

            // Eh maybe i could sandbox loadfile but for now i won't. If it
            // would be required, i will sandbox it.
            lua_pushnil(L);
            lua_pushnil(L);
            lua_setglobal(L, "load");
            lua_setglobal(L, "loadfile");

            // Other functions seem to be more or less safe.
            //
            // I know, blacklisting is not as safe as whitelisting but for now i
            // do that.
        }

        int dofile(lua_State *L) {
            // No we aren't allowing to execute standard input.
            fs::path relpath = luaL_checkstring(L, 1);

            lua_getfield(L, LUA_REGISTRYINDEX, DOFILE_PATH);

            // Just like that, we can disable dofile completely if we want.
            if (!lua_isstring(L, -1)) {
                return luaL_error(L, "dofile is not available currently");
            }

            fs::path parent_dir = lua_tostring(L, -1);

            lua_pop(L, 1);

            std::optional<fs::path> script_path = securepath(parent_dir, relpath);

            if (!script_path) {
                return luaL_error(L, "cannot open %s: permission denied", relpath.c_str());
            }

            if (!LuaUtil::run_script(L, script_path->c_str())) {
                return luaL_error(L, "failed to execute %s", script_path->c_str());
            }

            return 0;
        }

    }
}
