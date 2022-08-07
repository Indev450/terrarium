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

#ifndef GAME_HPP
#define GAME_HPP

#include <queue>
#include <iostream>
#include <cstring>
#include <stdexcept>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>

#include "entity/entity_manager.hpp"
#include "world/world.hpp"
#include "tile/block_def_holder.hpp"
#include "item/item_def_manager.hpp"
#include "item/cis.hpp"
#include "craft/craft.hpp"
#include "player/player.hpp"
#include "player/world_interact.hpp"
#include "event/event.hpp"
#include "ui/hud.hpp"
#include "graphics/gfx.hpp"
#include "sounds/sfx.hpp"
#include "utils/binary_io.hpp"

namespace Terrarium {

    const uint32_t WORLD_SAVE_VERSION = 2;
    const uint32_t INVENTORIES_SAVE_VERSION = 1;
    const uint32_t PLAYER_SAVE_VERSION = 1;

    struct GameState {
        GameState() {
            blocks_to_pixels.scale(Tile::SIZE, Tile::SIZE);
            pixels_to_blocks.scale(1./Tile::SIZE, 1./Tile::SIZE);
        }

        // World save file format:
        // char[8]         signature    should be "terrwrld"
        // u32             version      format version
        //
        // WorldSave       world        (See src/world/world.hpp)
        // BlockIdsSave    block_ids    (See src/tile/block_def_holder.hpp)
        void loadWorld(std::istream &file) {
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

        void saveWorld(std::ostream &file) {
            file.write("terrwrld", 8);
            write<uint32_t>(file, WORLD_SAVE_VERSION);

            world.save(file);
            block_defs.save(file);
        }

        // Inventories save file format:
        // char[8]    signature       should be "terrinvs"
        // u32        version         format version
        //
        // CIS        inventories     (see src/items/cis.hpp)
        void loadInventories(std::istream &file) {
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

        void saveInventories(std::ostream &file) {
            file.write("terrinvs", 8);
            write<uint32_t>(file, INVENTORIES_SAVE_VERSION);

            inventories.save(file);
        }

        // Player save file format:
        // char[8]       signature    should be "terrplay"
        // u32           version      format version
        //
        // PlayerSave    player       (see src/player/player.hpp)
        void loadPlayer(std::istream &file) {
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

        void savePlayer(std::ostream &file) {
            file.write("terrplay", 8);
            write<uint32_t>(file, PLAYER_SAVE_VERSION);

            player->save(file, *this);
        }

        World world;
        EntityManager entity_mgr;
        BlockDefHolder block_defs;
        ItemDefManager item_defs;
        CraftManager crafts;
        CIS inventories;

        // TODO - maybe move this into Player class?
        WorldInteractHelper world_interact;

        std::shared_ptr<Player> player;

        std::queue<Event> events;

        Gfx gfx;
        Sfx sfx;

        Hud hud;

        sf::FloatRect camera = { 0, 0, 0, 0 };

        sf::Transform blocks_to_pixels;
        sf::Transform pixels_to_blocks;
    };

} // namespace Terrarium

#endif
