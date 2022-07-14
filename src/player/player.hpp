#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/System/Vector2.hpp>

#include "../entity/entity.hpp"
#include "../tile/tile.hpp"
#include "../item/inventory.hpp"

namespace Terrarium {

    class GameState;

    struct PlayerStats {
        float jump_force = 25;
        float acceleration = 16;

        float max_speed = 16;
    };

    struct PlayerControls {
        bool left = false;
        bool right = false;

        bool jump = false;

        bool lmb = false;
        bool rmb = false;

        sf::Vector2f mouse_pos;
    };

    class Player: public Entity {
    public:
        // Need it there because it required for hotbar[] array
        static const unsigned int HOTBAR_SIZE = 8;

    private:
        std::shared_ptr<ItemStack> hotbar[HOTBAR_SIZE];
        unsigned int hotbar_selected = 0;

        bool using_item = false;
        bool alt_using_item = false;

    public:
        PlayerControls controls;

        float hotbar_scroll = 0;

        PlayerStats stats;

        // For now it has same size as hotbar, but later i'll change it,
        // so this is why i need separe hotbar array
        std::shared_ptr<Inventory> inventory = std::make_shared<Inventory>(HOTBAR_SIZE);

        Player() {
            for (unsigned int i = 0; i < HOTBAR_SIZE; ++i) {
                hotbar[i] = inventory->get(i);
            }
        }

        void update(GameState &game, float dtime) override;

        sf::Vector2f getPosition(GameState &game);

        inline std::shared_ptr<ItemStack> *getHotbar() {
            return hotbar;
        }

        inline unsigned int getHotbarSelected() {
            return hotbar_selected;
        }
    };

} // namespace Terrarium

#endif
