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

#include <stdexcept>

#include "lua_mod_config.hpp"

#include "../../../utils/datafile.hpp"

namespace Terrarium {

    LuaModConfig load_config(const fs::path &root) {
        Datafile datafile;

        if (!datafile.loadFromFile(root / "config.txt")) {
            throw std::runtime_error("No mod config found in " + root.string());
        }

        LuaModConfig config;

        config.root = root;

        config.name = datafile["name"].getString();

        if (datafile.childExists("depends")) {
            for (size_t i = 0; i < datafile["depends"].getValueCount(); ++i) {
                config.depends.insert(datafile["depends"].getString(i));
            }
        }

        if (datafile.childExists("optional_depends")) {
            for (size_t i = 0; i < datafile["optional_depends"].getValueCount(); ++i) {
                config.optional_depends.insert(datafile["optional_depends"].getString(i));
            }
        }

        return config;
    }

}
