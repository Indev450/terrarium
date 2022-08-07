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

#ifndef UTIL_SAVES_HPP
#define UTIL_SAVES_HPP

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace Terrarium {

    class GameState;

    class SavesManager {
        fs::path saves_dir;

    public:
        SavesManager(const fs::path &_saves_dir);

        fs::path getSavePath(const std::string &save_name, bool create = false);

        bool saveExists(const std::string &save_name);

        void loadWorldData(GameState &game, const std::string &save_name);

        void loadInventoriesData(GameState &game, const std::string &save_name);

        void loadPlayerData(GameState &game, const std::string &save_name);

        void save(GameState &game, const std::string &save_name);

    };

}

#endif
