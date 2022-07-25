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

    void Hud::delElement(const std::string &name) {
        elements.erase(name);
    }

    void Hud::addBar(const std::string &name, std::unique_ptr<Bar> bar) {
        bars[name] = std::move(bar);
    }

    Bar *Hud::getBar(const std::string &name) {
        auto pair = bars.find(name);

        if (pair != bars.end()) {
            return pair->second.get();
        }

        return nullptr;
    }

    void Hud::delBar(const std::string &name) {
        bars.erase(name);
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

    bool Hud::scroll(GameState &game, const sf::Vector2f &position, float delta) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible) {
                continue;
            }

            if (it->second->scroll(game, position, delta)) {
                return true;
            }
        }

        return false;
    }

    void Hud::render(sf::RenderTarget &target, GameState &game) {
        sf::Transform transform;

        for (auto it = bars.begin(); it != bars.end(); ++it) {
            it->second->render(target);
        }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible) {
                continue;
            }

            it->second->render(target, game, transform);
        }
    }

}
