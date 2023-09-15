/*
 * Copyright 2022 Indev
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

#include <SFML/Graphics.hpp>

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
        auto game_ptr_copy = game;

        future = std::async(
            [mapgen = std::move(mapgen), world_ptr_copy, game_ptr_copy, save_name = save_name]() {
                mapgen->run(*world_ptr_copy);
                game_ptr_copy->world = std::move(*world_ptr_copy);
                game_ptr_copy->modding_interface->onMapgenFinish();

                // TODO - move this somewhere or idk. Minimap sure sounds useful
                // but idk where to move it atm.
                /*
                sf::Image image;

                World &w = game_ptr_copy->world;

                image.create(w.getWidth(), w.getHeight(), sf::Color::White);

                for (int x = 0; x < w.getWidth(); ++x) {
                    for (int y = 0; y < w.getHeight(); ++y) {
                        blockid block = w.getBlock(x, y);
                        blockid wall = w.getWall(x, y);

                        sf::Color pixel = sf::Color::Blue;

                        if (block) {
                            pixel = game_ptr_copy->block_defs.getOrUnknown(block).getAverageColor();
                        } else if (wall) {
                            pixel = game_ptr_copy->block_defs.getOrUnknown(wall).getAverageColor();
                            pixel *= sf::Color(127, 127, 127);
                        }

                        image.setPixel(x, y, pixel);
                    }
                }

                image.saveToFile(game_ptr_copy->saves.getSavePath(save_name, true) / "minimap.png");
                */
            });

        text.setFont(game->gfx.font);
        text.setString("Generating map, please wait.");
        text.setCharacterSize(32);
        text.setPosition(16, 16);
    }

    void MapgenActivity::update(ActivityManager &am, float dtime) {
        if (future.wait_for(std::chrono::seconds(0)) != std::future_status::timeout) {
            // If an exception happened, raise it
            future.get();

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
