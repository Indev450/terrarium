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

#include <chrono>

#include "mapgen_activity.hpp"
#include "game_activity.hpp"
#include "activity_manager.hpp"

namespace Terrarium {

    MapgenActivity::MapgenActivity(std::shared_ptr<GameState> _game,
                                   std::unique_ptr<MapgenBase> mapgen,
                                   uint16_t width, uint16_t height,
                                   const std::string &_save_name):
        world(std::make_shared<World>()), game(_game), save_name(_save_name)
    {
        world->create(width, height);

        auto world_ptr_copy = world;

        future = std::async(
            [mapgen = std::move(mapgen), world_ptr_copy]() { mapgen->run(*world_ptr_copy); });

        text.setFont(game->gfx.font);
        text.setString("Generating map, please wait.");
        text.setCharacterSize(32);
        text.setPosition(16, 16);
    }

    void MapgenActivity::update(ActivityManager &am, float dtime) {
        if (future.wait_for(std::chrono::seconds(0)) != std::future_status::timeout) {
            game->world = std::move(*world);

            game->modding_interface->onMapgenFinish();

            // Replace activity with game activity
            am.setActivity(std::make_unique<GameActivity>(am, game, save_name));

            // `this` is now invalid pointer. E S C A P E !
            return;
        }

        text_timer -= dtime;

        if (text_timer < 0) {
            text_timer = text_update_time;

            ++text_dots;

            std::string text_string = "Generating map, please wait.";

            text_string += std::string(text_dots % 3, '.');

            text.setString(text_string);
        }
    }

    void MapgenActivity::render(sf::RenderTarget &target) {
        target.clear(sf::Color::Black);

        target.draw(text);
    }

}
