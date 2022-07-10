#ifndef UI_HOTBAR_HPP
#define UI_HOTBAR_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "../game.hpp"
#include "../item/item_stack.hpp"
#include "../graphics/gfx.hpp"

namespace Terrarium {

    class HotbarRenderer {
        static const unsigned int CELL_SIZE = 32;
        static const unsigned int SPACE = 4;

        static const int OUTLINE = 2;

        // I'd like to make this static const, but stupid compiler doesn't let me
        // to do that :blobcatreeeeeee:
        const sf::Color BG_COLOR;
        const sf::Color CELL_COLOR;
        const sf::Color SELECTED_CELL_COLOR;

        unsigned int items_count;

        sf::RectangleShape background;
        sf::RectangleShape cell;

        sf::Text count_text;
    public:
        HotbarRenderer(unsigned int _items_count, const Gfx &gfx);

        void render(sf::RenderTarget &target, GameState &game, const sf::Vector2f &position);
    };

}

#endif
