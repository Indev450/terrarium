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

#ifndef WORLD_RENDERING_HPP
#define WORLD_RENDERING_HPP

#include <stdexcept>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "../game.hpp"
#include "../tile/tile.hpp"

namespace Terrarium {

    class WorldRenderer {
        // Two textures needed to swap between them when updating
        sf::RenderTexture rtexture;
        sf::RenderTexture rtexture_back;
        bool swap = false;

        sf::Sprite sprite;

        sf::RectangleShape air_block;

        sf::Vector2i in_world_pos = { 0, 0 };
        sf::IntRect already_drawn = { -1, -1, 0, 0 };
        bool needs_update = true;
        int step;

        inline void renderTile(
                sf::RenderTexture &target,
                GameState &game,
                int x, int y, int world_x, int world_y,
                const Tile &tile);

        inline bool isAutotileNeighbour(GameState &game, BlockDef &tile, blockid neighbour_id);

    public:
        WorldRenderer(const sf::Vector2u &screen_size, int _step = 4):
            air_block(sf::Vector2f(Tile::SIZE, Tile::SIZE)), step(_step) {

            air_block.setFillColor(sf::Color(0, 0, 0, 0));

            setScreenSize(screen_size);
        }

        void setScreenSize(const sf::Vector2u &screen_size) {
            if (!rtexture.create(screen_size.x + Tile::SIZE*step, screen_size.y + Tile::SIZE*step) ||
                !rtexture_back.create(screen_size.x + Tile::SIZE*step, screen_size.y + Tile::SIZE*step)) {
                throw std::runtime_error("Terrarium::WorldRenderer::setScreenSize: failed to create render texture");
            }
        }

        void update(GameState &game);

        void updatePosition(const sf::FloatRect &camera);

        void render(sf::RenderTarget &target);

    };

} // namespace Terrarium

#endif
