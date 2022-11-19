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

#ifndef PLAYER_WORLD_INTERACT_HPP
#define PLAYER_WORLD_INTERACT_HPP

#include <optional>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace Terrarium {

    class GameState;

    // Helper object for handling interactive objects.
    struct WorldInteractHelper {
        float interact_distance = 4;

        sf::RectangleShape highlight_shape;

        WorldInteractHelper();

        void interact(GameState &game);
        void highlightInteractive(GameState &game, sf::RenderTarget &target);

        std::optional<sf::Vector2i> findNearestInteractiveBlock(GameState &game);
    };

}

#endif
