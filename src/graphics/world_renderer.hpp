#ifndef WORLD_RENDERING_HPP
#define WORLD_RENDERING_HPP

#include <stdexcept>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "../game.hpp"

namespace Terrarium {

    class WorldRenderer {
        // Two textures needed to swap between them when updating
        sf::RenderTexture rtexture;
        sf::RenderTexture rtexture_back;
        bool swap = false;

        sf::Sprite sprite;

        sf::Vector2i in_world_pos = { 0, 0 };
        sf::IntRect already_drawn = { -1, -1, 0, 0 };
        bool needs_update = true;
        int step = 4;

        inline void renderTile(sf::RenderTexture &target, BlockDefHolder &block_defs, int x, int y, const Tile &tile);

    public:
        WorldRenderer(const sf::Vector2u &screen_size, int _step = 4):
            step(_step) {

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
