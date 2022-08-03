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

namespace Terrarium {

    struct GameState {
        GameState(unsigned int world_width, unsigned int world_height):
            world(world_width, world_height)
        {
            blocks_to_pixels.scale(Tile::SIZE, Tile::SIZE);
            pixels_to_blocks.scale(1./Tile::SIZE, 1./Tile::SIZE);
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
