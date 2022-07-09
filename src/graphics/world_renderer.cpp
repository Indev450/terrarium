#include <cmath>

#include <SFML/Graphics/RenderStates.hpp>

#include "world_renderer.hpp"
#include "../tile/tile.hpp"
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
        int start_x = static_cast<int>(stepify(floor(game.camera.left / Tile::SIZE), step));
        int end_x = start_x + static_cast<int>(ceil(screen_size.x / Tile::SIZE));

        int start_y = static_cast<int>(stepify(floor(game.camera.top / Tile::SIZE), step));
        int end_y = start_y + static_cast<int>(ceil(screen_size.y / Tile::SIZE));

        for (int x = start_x; x < end_x; ++x) {
            for (int y = start_y; y < end_y; ++y) {
                // If we already drawn that block, skip it, it is already on the texture
                if (already_drawn.contains(x, y)) {
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

        sprite.setTexture(first->getTexture());

        already_drawn = { start_x, start_y, end_x - 1 - start_x, end_y - 1 - start_y };

        swap = !swap;

        needs_update = false;
    }

    inline void WorldRenderer::renderTile(sf::RenderTexture &target, BlockDefHolder &block_defs, int x, int y, const Tile &tile) {
        if (tile.bg) {
            BlockDef &def = block_defs.getOrUnknown(tile.bg);

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
    }

    void WorldRenderer::updatePosition(const sf::FloatRect &camera) {
        int new_world_x = stepify(floor(camera.left / Tile::SIZE), step);
        int new_world_y = stepify(floor(camera.top / Tile::SIZE), step);

        if (new_world_x != in_world_pos.x || new_world_y != in_world_pos.y) {
            in_world_pos.x = new_world_x;
            in_world_pos.y = new_world_y;

            needs_update = true;
        }

        sprite.setPosition(
            stepify(camera.left, Tile::SIZE*step) - camera.left,
            stepify(camera.top, Tile::SIZE*step) - camera.top);
    }

    void WorldRenderer::render(sf::RenderTarget &target) {
        target.draw(sprite);
    }

} // namespace Terrarium
