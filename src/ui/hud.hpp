#ifndef HUD_HPP
#define HUD_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "element.hpp"

namespace Terrarium {

    class GameState;

    class Hud {
        std::unordered_map<std::string, std::unique_ptr<UIElement>> elements;

    public:
        void addElement(const std::string &name, std::unique_ptr<UIElement> element);

        void setVisible(const std::string &name, bool visible);

        bool isVisible(const std::string &name);

        bool click(GameState &game, const sf::Vector2f &position);

        void render(sf::RenderTarget &target, GameState &game);
    };

}

#endif
