#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/System/Vector2.hpp>

#include "../entity/entity.hpp"
#include "../tile/tile.hpp"
#include "../item/inventory.hpp"

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

        float hotbar_scroll = 0;

        PlayerStats stats;

        // For now it has same size as hotbar, but later i'll change it,
        // so this is why i need separe hotbar array
        Inventory inventory = Inventory(HOTBAR_SIZE);

        entityid entity_id = 0;

        Player() {
            for (unsigned int i = 0; i < HOTBAR_SIZE; ++i) {
                hotbar[i] = inventory.get(i);
            }
        }

        void update(GameState &game, float dtime);

        sf::Vector2f getPosition(GameState &game);

    private:
        static const unsigned int HOTBAR_SIZE = 8;

        std::shared_ptr<ItemStack> hotbar[HOTBAR_SIZE];
        unsigned int hotbar_selected = 0;

        bool using_item = false;
        bool alt_using_item = false;
    };

} // namespace Terrarium

#endif
