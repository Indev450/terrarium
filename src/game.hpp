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
#include "craft/craft.hpp"
#include "player/player.hpp"
#include "player/world_interact.hpp"
#include "event/event.hpp"
#include "ui/hud.hpp"
#include "graphics/gfx.hpp"
#include "sounds/sfx.hpp"
#include "utils/binary_io.hpp"

namespace Terrarium {

    struct GameState {
        GameState() {
            blocks_to_pixels.scale(Tile::SIZE, Tile::SIZE);
            pixels_to_blocks.scale(1./Tile::SIZE, 1./Tile::SIZE);
        }

        void load(std::istream &file) {
            // Save file format:
            // char[4]         signature    should be "terr"
            // u32             version      should be 3 for this format version
            //
            // WorldSave       world        (See src/world/world.hpp)
            // BlockIdsSave    block_ids    (See src/tile/block_def_holder.hpp)
            // PlayerSave      player       (See src/player/player.hpp)
            char signature[4];
            file.read(signature, 4);

            if (strncmp(signature, "terr", 4) != 0) {
                throw std::invalid_argument("save file signature missmatch");
            }

            uint32_t version = read<uint32_t>(file);

            if (version != 3) {
                throw std::invalid_argument("save file version missmatch");
            }

            world.load(file, *this);
            block_defs.load(file);
            player->load(file, *this);
        }

        void save(std::ostream &file) {
            file.write("terr", 4);
            write<uint32_t>(file, 3);

            world.save(file);
            block_defs.save(file);
            player->save(file, *this);
        }

        World world;
        EntityManager entity_mgr;
        BlockDefHolder block_defs;
        ItemDefManager item_defs;
        CraftManager crafts;

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
