#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/System/Vector2.hpp>

#include "../entity/entity.hpp"
#include "../tile/tile.hpp"

namespace Terrarium {

    class GameState;

    struct PlayerStats {
        float jump_force = Tile::SIZE*25;
        float acceleration = Tile::SIZE*16;

        float max_speed = Tile::SIZE*16;
    };

    struct Player {
        bool left = false;
        bool right = false;

        bool jump = false;

        bool lmb = false;
        bool rmb = false;

        sf::Vector2f mouse_pos;

        PlayerStats stats;

        entityid entity_id = 0;

        void update(GameState &game, float dtime);

        sf::Vector2f getPosition(GameState &game);
    };

} // namespace Terrarium

#endif
