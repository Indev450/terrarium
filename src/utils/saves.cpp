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

#include <system_error>
#include <stdexcept>
#include <fstream>

#include "saves.hpp"
#include "../game.hpp"

namespace Terrarium {

    SavesManager::SavesManager(const fs::path &_saves_dir):
        saves_dir(_saves_dir) {

        if (fs::exists(saves_dir)) {
            if (!fs::is_directory(saves_dir)) {
                throw std::runtime_error("saves path exists and it is not a directory");
            }
        } else {
            fs::create_directories(saves_dir);
        }
    }

    fs::path SavesManager::getSavePath(const std::string &save_name, bool create) {
        fs::path dir = saves_dir/save_name;

        if (fs::exists(dir)) {
            if (!fs::is_directory(dir)) {
                throw std::runtime_error("save path exists, but it is not a directory");
            }
        } else {
            if (!create) {
                throw std::runtime_error("save path does not exists");
            } else {
                fs::create_directory(dir);
            }
        }

        return dir;
    }

    bool SavesManager::saveExists(const std::string &save_name) {
        fs::path dir = saves_dir/save_name;

        return fs::exists(dir) && fs::is_directory(dir);
    }

    void SavesManager::loadWorldData(GameState &game, const std::string &save_name) {
        fs::path world_save = getSavePath(save_name)/"world.bin";

        std::ifstream file(world_save, std::ios::binary);

        if (file.is_open()) {
            game.loadWorld(file);
        } else {
            throw std::runtime_error("failed to load world data");
        }
    }

    void SavesManager::loadInventoriesData(GameState &game, const std::string &save_name) {
        fs::path inventories_save = getSavePath(save_name)/"inventories.bin";

        std::ifstream file(inventories_save, std::ios::binary);

        if (file.is_open()) {
            game.loadInventories(file);
        } else {
            throw std::runtime_error("failed to load inventories data");
        }
    }

    void SavesManager::loadPlayerData(GameState &game, const std::string &save_name) {
        fs::path player_save = getSavePath(save_name)/"player.bin";

        std::ifstream file(player_save, std::ios::binary);

        if (file.is_open()) {
            game.loadPlayer(file);
        } else {
            throw std::runtime_error("failed to load player data");
        }
    }

    void SavesManager::save(GameState &game, const std::string &save_name) {
        fs::path save_dir = getSavePath(save_name, true);

        fs::path world_save = save_dir/"world.bin";
        fs::path inventories_save = save_dir/"inventories.bin";
        fs::path player_save = save_dir/"player.bin";

        std::ofstream file;

        file.open(world_save, std::ios::binary);

        if (file.is_open()) {
            game.saveWorld(file);
        } else {
            throw std::runtime_error("failed to save world data");
        }

        file.close();

        file.open(inventories_save, std::ios::binary);

        if (file.is_open()) {
            game.saveInventories(file);
        } else {
            throw std::runtime_error("failed to save inventories data");
        }

        file.close();

        file.open(player_save, std::ios::binary);

        if (file.is_open()) {
            game.savePlayer(file);
        } else {
            throw std::runtime_error("failed to save player data");
        }

        file.close();
    }

}
