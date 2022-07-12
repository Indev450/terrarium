#ifndef GAME_HPP
#define GAME_HPP

#include <queue>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>

#include "entity/entity_manager.hpp"
#include "world/world.hpp"
#include "tile/block_def_holder.hpp"
#include "item/item_def_manager.hpp"
#include "player/player.hpp"
#include "event/event.hpp"
#include "graphics/gfx.hpp"

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
        Player player;

        std::queue<Event> events;

        Gfx gfx;

        sf::FloatRect camera = { 0, 0, 0, 0 };

        sf::Transform blocks_to_pixels;
        sf::Transform pixels_to_blocks;
    };

} // namespace Terrarium

#endif
