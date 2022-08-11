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

#ifndef ACTIVITY_MANAGER_HPP
#define ACTIVITY_MANAGER_HPP

#include <SFML/Graphics.hpp>

#include <memory>

#include "activity.hpp"

namespace Terrarium {

    class ActivityManager {
        std::unique_ptr<Activity> activity;
        std::unique_ptr<sf::RenderWindow> window;

    public:
        ActivityManager(std::unique_ptr<sf::RenderWindow> _window);

        void run();
        void setActivity(std::unique_ptr<Activity> new_activity);

        sf::RenderWindow &getWindow();
    };

}

#endif
