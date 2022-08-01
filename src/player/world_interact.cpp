#include <cmath>
#include <optional>

#include "world_interact.hpp"
#include "../game.hpp"

namespace Terrarium {

    void WorldInteractHelper::interact(GameState &game) {
        const sf::Vector2f &mouse_pos = game.player->controls.mouse_pos;

        float found_distance_sqr = interact_distance*interact_distance;
        std::optional<sf::Vector2i> found_block;

        for (float x = mouse_pos.x - interact_distance; x < mouse_pos.x + interact_distance; x += 1) {
            for (float y = mouse_pos.y - interact_distance; y < mouse_pos.y + interact_distance; y += 1) {
                BlockDef &def = game.block_defs.getOrUnknown(
                    game.world.getBlock(floor(x), floor(y)));

                if (def.is_interactive) {
                    // Change position to be center of block.
                    sf::Vector2f pos(floor(x) + 0.5, floor(y) + 0.5);

                    float distance_sqr = (mouse_pos.x - pos.x) * (mouse_pos.x - pos.x) + (mouse_pos.y - pos.y) * (mouse_pos.y - pos.y);

                    if (distance_sqr < found_distance_sqr) {
                        found_block = sf::Vector2i(floor(x), floor(y));
                        found_distance_sqr = distance_sqr;
                    }
                }
            }
        }

        if (found_block) {
            BlockEvent *block_event = new BlockEvent();

            block_event->position = *found_block;
            block_event->user = game.player;

            game.events.emplace(Event::BlockActivate, block_event);
        }
    }

}
