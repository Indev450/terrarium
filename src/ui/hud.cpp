#include "hud.hpp"
#include "../game.hpp"

namespace Terrarium {

    void Hud::addElement(const std::string &name, std::unique_ptr<UIElement> element) {
        elements[name] = std::move(element);
    }

    void Hud::setVisible(const std::string &name, bool visible) {
        auto pair = elements.find(name);

        if (pair != elements.end()) {
            pair->second->visible = visible;
        }
    }

    bool Hud::isVisible(const std::string &name) {
        auto pair = elements.find(name);

        if (pair != elements.end()) {
            return pair->second->visible;
        }

        return false;
    }

    bool Hud::click(GameState &game, const sf::Vector2f &position) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible) {
                continue;
            }

            if (it->second->click(game, position)) {
                return true;
            }
        }

        return false;
    }

    void Hud::render(sf::RenderTarget &target, GameState &game) {
        sf::Transform transform;

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible) {
                continue;
            }

            it->second->render(target, game, transform);
        }
    }

}
