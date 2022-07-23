#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <SFML/Graphics.hpp>

namespace Terrarium {

    class GameState;

    class UIElement: public sf::Transformable {
    public:
        bool visible = true;

        // This function should return, was the click handled or not
        virtual bool click(GameState &game, const sf::Vector2f &position) = 0;

        virtual bool scroll(GameState &game, const sf::Vector2f &position, float delta) { return false; }

        virtual void render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) = 0;
    };

}

#endif
