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

#ifndef UI_CHAT_HPP
#define UI_CHAT_HPP

#include <list>
#include <memory>

#include <SFML/Graphics.hpp>

#include "element.hpp"
#include "text_input.hpp"

namespace Terrarium {

    class GameState;

    class ChatUI: public TextInput, public UIElement, public std::enable_shared_from_this<ChatUI> {
    public:
        // Need those to be public to calculate chat position properly

        // More like font height, for width we divide that value anyway
        static const unsigned FONT_SIZE = 32;

        // Max messages to be displayed at once in the chat log
        static const unsigned MAX_SHOW_MESSAGES = 8;

        // Small gap between messages
        static const unsigned GAP = 2;

    private:
        // Scroll progress in chat log
        unsigned current_scroll = MAX_SHOW_MESSAGES;

        // Few rectangles for visuals
        sf::RectangleShape background_shape;
        sf::RectangleShape input_shape;
        sf::RectangleShape cursor_shape;

        // sf::Text instance to render chat log text
        sf::Text text;

        // sf::Text instance to render text in input prompt
        sf::Text input_text;

        // Position in input buffer to display. Still not perfect though
        unsigned visual_pos = 0;

        // Timer for how long chat log stays visible. When less than 1 it starts
        // slowly disappear
        float visible_timer = 0;

        const sf::Color background_color = sf::Color(156, 156, 156);
        const sf::Color input_color = sf::Color(156, 156, 156);
        const sf::Color command_color = sf::Color(212, 212, 128);
        const uint8_t base_transparency = 128;

        // Used to check if there are new messages appeared
        unsigned last_total_messages = 0;

    private:
        bool preHandleEvent(GameState &game, const sf::Event &event) override;

        void onBufferUpdated(GameState &game) override;
        void onPosUpdated(GameState &game) override;

        void updateInputText(GameState &game);

    public:
        ChatUI(GameState &game);

        void focusGained(GameState &game) override;

        void focusLost(GameState &game) override;

        bool scroll(GameState &game, const sf::Vector2f &position, float delta) override;

        void update(GameState &game, float dtime) override;

        void render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) override;
    };

}

#endif
