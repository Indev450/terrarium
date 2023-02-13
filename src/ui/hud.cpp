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

    void Hud::setScreenSize(const sf::Vector2f &size) {
        screen_size = size;

        for (auto it = bars.begin(); it != bars.end(); ++it) {
            it->second->setScreenSize(size);
        }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            it->second->setScreenSize(size);
        }
    }

    sf::Vector2f Hud::getScreenSize() {
        return screen_size;
    }

    void Hud::addElement(const std::string &name, std::unique_ptr<UIElement> element, bool focus) {
        element->setScreenSize(screen_size);
        elements[name] = std::move(element);

        if (elements[name]->visible && focus) {
            focused_elements.push_back(name);
        }
    }

    void Hud::replaceElement(const std::string &name, std::unique_ptr<UIElement> element, bool focus) {
        auto pair = elements.find(name);

        if (pair != elements.end()) {
            element->setPosition(pair->second->getPosition());
            element->visible = pair->second->visible;
        }

        addElement(name, std::move(element), focus);
    }

    void Hud::setVisible(const std::string &name, bool visible, bool focus) {
        auto pair = elements.find(name);

        if (pair != elements.end()) {
            pair->second->visible = visible;
        }

        if (visible && focus) {
            if (std::find(focused_elements.begin(), focused_elements.end(), name) == focused_elements.end()) {
                focused_elements.push_back(name);
            }
        } else if (!visible) {
            focused_elements.remove(name);
        }
    }

    bool Hud::isVisible(const std::string &name) {
        auto pair = elements.find(name);

        if (pair != elements.end()) {
            return pair->second->visible;
        }

        return false;
    }

    bool Hud::hideFocused() {
        if (focused_elements.empty()) {
            return false;
        }

        auto pair = elements.find(focused_elements.back());

        if (pair != elements.end()) {
            pair->second->visible = false;
        }

        focused_elements.pop_back();

        return true;
    }

    UIElement *Hud::getElement(const std::string &name) {
        auto pair = elements.find(name);

        if (pair != elements.end()) {
            return pair->second.get();
        }

        return nullptr;
    }

    void Hud::delElement(const std::string &name) {
        elements.erase(name);
    }

    void Hud::addBar(const std::string &name, std::unique_ptr<Bar> bar) {
        bar->setScreenSize(screen_size);
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

    void Hud::hover(GameState &game, const sf::Vector2f &position) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible) {
                continue;
            }

            auto hover_tip = it->second->hover(game, position);

            if (hover_tip) {
                tip = hover_tip;
                return;
            }
        }

        tip = std::nullopt;
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

        std::string *focused = nullptr;

        if (!focused_elements.empty()) {
            focused = &focused_elements.back();
        }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible || (focused && it->first == *focused)) {
                continue;
            }

            it->second->render(target, game, transform);
        }

        if (focused) {
            auto pair = elements.find(*focused);

            if (pair != elements.end() && pair->second->visible) {
                pair->second->render(target, game, transform);
            }
        }
    }

}
