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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <optional>
#include <iostream>

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
        static const unsigned int INVENTORY_ROWS = 4;

    private:
        std::shared_ptr<ItemStack> hotbar[HOTBAR_SIZE];
        unsigned int hotbar_selected = 0;

        bool using_item = false;
        bool alt_using_item = false;

    public:
        PlayerControls controls;

        float hotbar_scroll = 0;

        PlayerStats stats;

        bool controlled = true;

        std::shared_ptr<Inventory> inventory = std::make_shared<Inventory>(
            HOTBAR_SIZE*INVENTORY_ROWS);

        std::shared_ptr<ItemStack> hold_item_stack = std::make_shared<ItemStack>();

        std::optional<std::string> crafting_category;

        Player() {
            updateHotbar();
        }

        void updateHotbar() {
            for (unsigned int i = 0; i < HOTBAR_SIZE; ++i) {
                hotbar[i] = inventory->get(i);
            }
        }

        void update(GameState &game, float dtime) override;

        sf::Vector2f getPosition();

        inline std::shared_ptr<ItemStack> *getHotbar() {
            return hotbar;
        }

        inline unsigned int getHotbarSelected() {
            return hotbar_selected;
        }

        // Player save format:
        // i32          x
        // i32          y
        // u8           have_hold_item_stack    1 if hold_item_stack is not empty, 0 otherwise
        // ItemStack    hold_item_stack         item stack that player moving in inventory
        // Inventory    inventory
        void load(std::istream &s, GameState &game);

        void save(std::ostream &s, GameState &game);
    };

} // namespace Terrarium

#endif
