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

#include "activity_manager.hpp"

namespace Terrarium {

    ActivityManager::ActivityManager(std::unique_ptr<sf::RenderWindow> _window):
        window(std::move(_window))
    {}

    void ActivityManager::run() {
        sf::Clock clock;

        while (activity && window->isOpen()) {
            sf::Event event;

            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window->close();
                }

                activity->onEvent(*this, event);
            }

            float dtime = clock.restart().asSeconds();

            activity->update(*this, dtime);

            activity->render(*window);

            window->display();
        }
    }

    void ActivityManager::setActivity(std::unique_ptr<Activity> new_activity) {
        activity = std::move(new_activity);
    }

    sf::RenderWindow &ActivityManager::getWindow() {
        return *window;
    }

}
