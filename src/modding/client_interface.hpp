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

#ifndef MODDING_CLIENT_INTERFACE_HPP
#define MODDING_CLIENT_INTERFACE_HPP

#include <filesystem>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../event/event.hpp"

namespace fs = std::filesystem;

namespace Terrarium {

    class GameState;

    class ClientModdingInterface {
    public:
        std::shared_ptr<GameState> game;

        ClientModdingInterface(std::shared_ptr<GameState> _game):
            game(_game)
        {}

        virtual void loadScript(const fs::path &path) {};

        virtual void update(float dtime) {};
        virtual bool ui_click(const std::string &id, const sf::Vector2f &position) { return false; };
        virtual bool ui_scroll(const std::string &id, const sf::Vector2f &position, float delta) { return false; };
        virtual void ui_render(const std::string &id) {};

        virtual void handleModCmd(ModCmdEvent &cmd_event) {};
    };

}

#endif
