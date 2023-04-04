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

#include "game.hpp"

namespace Terrarium {

    GameState::GameState(const std::string &_save_name):
        saves("saves"), save_name(_save_name)
    {
        blocks_to_pixels.scale(Tile::SIZE, Tile::SIZE);
        pixels_to_blocks.scale(1./Tile::SIZE, 1./Tile::SIZE);

        entity_mgr.addPrefab(std::make_shared<EntityPrefab>());
    }

    void GameState::loadWorld(std::istream &file) {
        char signature[8];
        file.read(signature, 8);

        if (strncmp(signature, "terrwrld", 8) != 0) {
            throw std::invalid_argument("world save file signature missmatch");
        }

        uint32_t version = read<uint32_t>(file);

        if (version != WORLD_SAVE_VERSION) {
            throw std::invalid_argument("world save file version missmatch");
        }

        world.load(file, *this);
        block_defs.load(file);
    }

    void GameState::saveWorld(std::ostream &file) {
        file.write("terrwrld", 8);
        write<uint32_t>(file, WORLD_SAVE_VERSION);

        world.save(file);
        block_defs.save(file);
    }

    void GameState::loadInventories(std::istream &file) {
        char signature[8];
        file.read(signature, 8);

        if (strncmp(signature, "terrinvs", 8) != 0) {
            throw std::invalid_argument("inventories save file signature missmatch");
        }

        uint32_t version = read<uint32_t>(file);

        if (version != INVENTORIES_SAVE_VERSION) {
            throw std::invalid_argument("inventories save file version missmatch");
        }

        inventories.load(file, *this);
    }

    void GameState::saveInventories(std::ostream &file) {
        file.write("terrinvs", 8);
        write<uint32_t>(file, INVENTORIES_SAVE_VERSION);

        inventories.save(file);
    }

    void GameState::loadPlayer(std::istream &file) {
        char signature[8];
        file.read(signature, 8);

        if (strncmp(signature, "terrplay", 8) != 0) {
            throw std::invalid_argument("player save file signature missmatch");
        }

        uint32_t version = read<uint32_t>(file);

        if (version != PLAYER_SAVE_VERSION) {
            throw std::invalid_argument("player save file version missmatch");
        }

        player->load(file, *this);
    }

    void GameState::savePlayer(std::ostream &file) {
        file.write("terrplay", 8);
        write<uint32_t>(file, PLAYER_SAVE_VERSION);

        player->save(file, *this);
    }

    void GameState::save() {
        saves.save(*this, save_name);
        modding_interface->save(saves.getSavePath(save_name));
    }

}
