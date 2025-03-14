/*
 * Copyright 2022 Indev
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

    void Hud::focusPush(const std::string &name) {
        std::shared_ptr<GameState> game_ptr = game.lock();

        if (!game_ptr) { return; }

        if (!focused_elements.empty()) {
            auto pair = elements.find(focused_elements.back());

            if (pair != elements.end()) {
                pair->second->focusLost(*game_ptr);
            }
        }

        focused_elements.push_back(name);

        auto pair = elements.find(name);

        if (pair != elements.end()) {
            pair->second->focusGained(*game_ptr);
        }
    }

    void Hud::focusPop() {
        std::shared_ptr<GameState> game_ptr = game.lock();

        if (!game_ptr) { return; }

        if (!focused_elements.empty()) {
            auto pair = elements.find(focused_elements.back());

            if (pair != elements.end()) {
                pair->second->focusLost(*game_ptr);
            }

            focused_elements.pop_back();
        }
    }

    void Hud::addElement(const std::string &name, std::shared_ptr<UIElement> element, bool focus) {
        element->setScreenSize(screen_size);
        elements[name] = element;

        if (elements[name]->visible && focus) {
            focusPush(name);
        }
    }

    void Hud::replaceElement(const std::string &name, std::shared_ptr<UIElement> element, bool focus) {
        auto pair = elements.find(name);

        if (pair != elements.end()) {
            element->setPosition(pair->second->getPosition());
            element->visible = pair->second->visible;
        }

        addElement(name, element, focus);
    }

    void Hud::setVisible(const std::string &name, bool visible, bool focus) {
        auto pair = elements.find(name);

        if (pair != elements.end()) {
            pair->second->visible = visible;
        }

        if (visible && focus) {
            if (std::find(focused_elements.begin(), focused_elements.end(), name) == focused_elements.end()) {
                focusPush(name);
            }
        } else if (!visible) {
            if (focused_elements.back() == name) {
                focusPop();
            } else {
                focused_elements.remove(name);
            }
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

        focusPop();

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

    void Hud::addBar(const std::string &name, std::shared_ptr<Bar> bar) {
        bar->setScreenSize(screen_size);
        bars[name] = bar;
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

    void Hud::hover(const sf::Vector2f &position) {
        std::shared_ptr<GameState> game_ptr = game.lock();

        if (!game_ptr) { return; }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible) {
                continue;
            }

            auto hover_tip = it->second->hover(*game_ptr, position);

            if (hover_tip) {
                tip = hover_tip;
                return;
            }
        }

        tip = std::nullopt;
    }

    bool Hud::click(const sf::Vector2f &position) {
        std::shared_ptr<GameState> game_ptr = game.lock();

        if (!game_ptr) { return false; }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible) {
                continue;
            }

            if (it->second->click(*game_ptr, position)) {
                return true;
            }
        }

        return false;
    }

    bool Hud::scroll(const sf::Vector2f &position, float delta) {
        std::shared_ptr<GameState> game_ptr = game.lock();

        if (!game_ptr) { return false; }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (!it->second->visible) {
                continue;
            }

            if (it->second->scroll(*game_ptr, position, delta)) {
                return true;
            }
        }

        return false;
    }

    void Hud::update(float dtime) {
        std::shared_ptr<GameState> game_ptr = game.lock();

        if (!game_ptr) { return; }

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            it->second->update(*game_ptr, dtime);
        }
    }

    void Hud::render(sf::RenderTarget &target) {
        std::shared_ptr<GameState> game_ptr = game.lock();

        if (!game_ptr) { return; }

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

            it->second->render(target, *game_ptr, transform);
        }

        if (focused) {
            auto pair = elements.find(*focused);

            if (pair != elements.end() && pair->second->visible) {
                pair->second->render(target, *game_ptr, transform);
            }
        }
    }

}
