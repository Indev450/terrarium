#ifndef GAME_HPP
#define GAME_HPP

#include <queue>

#include <SFML/Graphics/Rect.hpp>

#include "entity/entity_manager.hpp"
#include "world/world.hpp"
#include "tile/block_def_holder.hpp"
#include "player/player.hpp"
#include "event/event.hpp"

namespace Terrarium {

    struct GameState {
        GameState(unsigned int world_width, unsigned int world_height):
            world(world_width, world_height)
        {}

        World world;
        EntityManager entity_mgr;
        BlockDefHolder block_defs;
        Player player;

        std::queue<Event> events;

        sf::FloatRect camera = { 0, 0, 0, 0 };
    };

} // namespace Terrarium

#endif
