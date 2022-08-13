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

#ifndef GAME_ACTIVITY_HPP
#define GAME_ACTIVITY_HPP

#include <memory>
#include <string>

#include "activity.hpp"
#include "../game.hpp"
#include "../graphics/world_renderer.hpp"
#include "../ui/item_cell.hpp"

namespace Terrarium {

    class GameActivity: public Activity {
        std::shared_ptr<GameState> game;
        std::unique_ptr<WorldRenderer> world_renderer;

        ItemCellRenderer item_cell_renderer;

    public:
        GameActivity(ActivityManager &am, std::shared_ptr<GameState> _game, const std::string &_save_name);

        void update(ActivityManager &am, float dtime) override;
        void render(sf::RenderTarget &target) override;

        void onEvent(ActivityManager &am, sf::Event event) override;

        ~GameActivity();
    };

}

#endif