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

#include <cmath>

#include <SFML/Graphics/RenderStates.hpp>

#include "world_renderer.hpp"
#include "../utils/math.hpp"

namespace Terrarium {

    void WorldRenderer::update(GameState &game) {
        if (!needs_update && !game.world.isUpdated()) {
            return;
        }

        // First is current render target. Second is what have been drawn previously
        sf::RenderTexture *first = &rtexture;
        sf::RenderTexture *second = &rtexture_back;

        if (swap) {
            first = &rtexture_back;
            second = &rtexture;
        }

        // Initially, clear texture and copy what is already been drawn
        first->clear(sf::Color(0, 0, 0, 0));

        sf::Sprite prev_drawn_sprite;
        prev_drawn_sprite.setTexture(second->getTexture());
        prev_drawn_sprite.setPosition((already_drawn.left - in_world_pos.x) * Tile::SIZE,
                                      (already_drawn.top - in_world_pos.y) * Tile::SIZE);

        first->draw(prev_drawn_sprite);

        sf::Vector2u screen_size = first->getSize();

        // Maybe i could iterate only over parts of texture that changed,
        // but i hope that doesn't impact on perfomance much
        int start_x = static_cast<int>(stepify(floor(game.camera.left), step));
        int end_x = start_x + static_cast<int>(ceil(screen_size.x / Tile::SIZE));

        int start_y = static_cast<int>(stepify(floor(game.camera.top), step));
        int end_y = start_y + static_cast<int>(ceil(screen_size.y / Tile::SIZE));

        for (int x = start_x; x < end_x; ++x) {
            for (int y = start_y; y < end_y; ++y) {
                // If we already drawn that block, skip it, it is already on the texture
                if (already_drawn.contains(x, y) && !game.world.isUpdated(sf::Vector2i(x, y))) {
                    continue;
                }

                const Tile *tile = game.world.getTile(x, y);

                if (tile != nullptr) {
                    renderTile(
                        *first,
                        game.block_defs,
                        x - start_x,
                        y - start_y,
                        *tile);
                }
            }
        }

        first->display();

        game.world.resetUpdated();

        sprite.setTexture(first->getTexture());

        already_drawn = { start_x, start_y, end_x - 1 - start_x, end_y - 1 - start_y };

        swap = !swap;

        needs_update = false;
    }

    inline void WorldRenderer::renderTile(sf::RenderTexture &target, BlockDefHolder &block_defs, int x, int y, const Tile &tile) {
        if (tile.bg) {
            BlockDef &def = block_defs.getOrUnknown(tile.bg);

            // Maybe i should use game.blocks_to_pixels? I don't wanna change function signature tho
            def.sprite.setPosition(x*Tile::SIZE, y*Tile::SIZE);
            def.sprite.setColor(sf::Color(127, 127, 127, 255)); // Make wall darker

            target.draw(def.sprite);
        }

        if (tile.fg) {
            BlockDef &def = block_defs.getOrUnknown(tile.fg);

            def.sprite.setPosition(x*Tile::SIZE, y*Tile::SIZE);
            def.sprite.setColor(sf::Color(255, 255, 255, 255));

            target.draw(def.sprite);
        }

        if (!tile.fg && !tile.bg) {
            air_block.setPosition(x*Tile::SIZE, y*Tile::SIZE);

            target.draw(air_block, sf::BlendNone);
        }
    }

    void WorldRenderer::updatePosition(const sf::FloatRect &camera) {
        int new_world_x = stepify(floor(camera.left), step);
        int new_world_y = stepify(floor(camera.top), step);

        if (new_world_x != in_world_pos.x || new_world_y != in_world_pos.y) {
            in_world_pos.x = new_world_x;
            in_world_pos.y = new_world_y;

            needs_update = true;
        }

        // Same about transform, as in renderTile
        sprite.setPosition(
            stepify(camera.left*Tile::SIZE, Tile::SIZE*step) - camera.left*Tile::SIZE,
            stepify(camera.top*Tile::SIZE, Tile::SIZE*step) - camera.top*Tile::SIZE);
    }

    void WorldRenderer::render(sf::RenderTarget &target) {
        target.draw(sprite);
    }

} // namespace Terrarium
