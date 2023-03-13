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

#include <utility>
#include <queue>
#include <iostream>
#include <cstring>
#include <string>
#include <stdexcept>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>

#include "entity/entity_manager.hpp"
#include "world/world.hpp"
#include "tile/block_def_holder.hpp"
#include "item/item_def_manager.hpp"
#include "item/inventory_storage.hpp"
#include "craft/craft.hpp"
#include "player/player.hpp"
#include "player/world_interact.hpp"
#include "event/event.hpp"
#include "ui/hud.hpp"
#include "graphics/gfx.hpp"
#include "sounds/sfx.hpp"
#include "utils/binary_io.hpp"
#include "utils/saves.hpp"
#include "modding/interface.hpp"
#include "modding/client_interface.hpp"

namespace Terrarium {

    const uint32_t WORLD_SAVE_VERSION = 3;
    const uint32_t INVENTORIES_SAVE_VERSION = 1;
    const uint32_t PLAYER_SAVE_VERSION = 1;

    struct DayNightCycleEntry {
        sf::Color sky_color;
        float length;
        uint8_t light;
    };

    struct GameState {
        World world;
        EntityManager entity_mgr;
        BlockDefHolder block_defs;
        ItemDefManager item_defs;
        CraftManager crafts;
        InventoryStorage inventories;

        SavesManager saves;
        std::string save_name;

        std::vector<DayNightCycleEntry> day_night_cycle;
        float time = 0;
        float day_length = 60*12;
        uint8_t natural_light = 255;

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

        std::unique_ptr<ModdingInterface> modding_interface;
        std::unique_ptr<ClientModdingInterface> client_modding_interface;

        GameState(const std::string &_save_name);

        // World save file format:
        // char[8]         signature    should be "terrwrld"
        // u32             version      format version
        //
        // WorldSave       world        (See src/world/world.hpp)
        // BlockIdsSave    block_ids    (See src/tile/block_def_holder.hpp)
        void loadWorld(std::istream &file);

        void saveWorld(std::ostream &file);

        // Inventories save file format:
        // char[8]              signature       should be "terrinvs"
        // u32                  version         format version
        //
        // InventoryStorage     inventories     (see src/items/inventory_storage.hpp)
        void loadInventories(std::istream &file);

        void saveInventories(std::ostream &file);

        // Player save file format:
        // char[8]       signature    should be "terrplay"
        // u32           version      format version
        //
        // PlayerSave    player       (see src/player/player.hpp)
        void loadPlayer(std::istream &file);

        void savePlayer(std::ostream &file);

        void save();
    };

} // namespace Terrarium

#endif
