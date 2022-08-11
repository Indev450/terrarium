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

#ifndef MAPGEN_ACTIVITY_HPP
#define MAPGEN_ACTIVITY_HPP

#include <future>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "activity.hpp"
#include "../world/world.hpp"
#include "../mapgen/mapgen_base.hpp"
#include "../game.hpp"

namespace Terrarium {

    class MapgenActivity: public Activity {
        const float text_update_time = 0.5;

        sf::Text text;
        float text_timer = text_update_time;
        int text_dots = 0;

        std::future<void> future;

        std::shared_ptr<World> world;

        std::shared_ptr<GameState> game;

        std::string save_name;

    public:
        MapgenActivity(std::shared_ptr<GameState> _game,
                       std::unique_ptr<MapgenBase> mapgen,
                       uint16_t width, uint16_t height,
                       const std::string &_save_name);

        void update(ActivityManager &am, float dtime) override;
        void render(sf::RenderTarget &target) override;

        void onEvent(ActivityManager &am, sf::Event event) override {};
    };

}

#endif
