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

#include <optional>
#include <filesystem>

#include <lua.hpp>

namespace fs = std::filesystem;

namespace Terrarium {

    namespace LuaSandbox {

        // Index in registry with currently allowed path for dofile
        extern const char *DOFILE_PATH;

        // Returns absolute path to requiested file in specified parent
        // directory. Returns std::nullopt if resulting path points outside of
        // parent_dir. Note that parent dir should be absolute path.
        std::optional<fs::path> securepath(const fs::path &parent_dir, const fs::path &relpath);

        // Set currently legal dofile path
        void set_dofile_path(lua_State *L, const fs::path &path);

        // Open only trusted libs
        void openlibs(lua_State *L);

        void sandbox_base(lua_State *L);

        int dofile(lua_State *L);

    }

}
