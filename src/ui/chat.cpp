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

#include "chat.hpp"
#include "../game.hpp"

namespace Terrarium {

    ChatUI::ChatUI(GameState &game):
        // Why can't all fonts have same size in pixels goddamit. This is why i
        // need this "/2.65".
        background_shape(sf::Vector2f(game.chat.max_width*FONT_SIZE/2.65, MAX_SHOW_MESSAGES*FONT_SIZE + GAP*(MAX_SHOW_MESSAGES+1))),
        input_shape(sf::Vector2f(game.chat.max_width*FONT_SIZE/2.65, FONT_SIZE + GAP*2)),
        cursor_shape(sf::Vector2f(1.f, FONT_SIZE)) {

        background_shape.setPosition(0, 0);
        background_shape.setFillColor(background_color);
        input_shape.setFillColor(input_color);
        input_shape.setPosition(0, (MAX_SHOW_MESSAGES+1)*FONT_SIZE + GAP*(MAX_SHOW_MESSAGES+1));
        cursor_shape.setFillColor(sf::Color::White);

        input_text.setPosition(0, (MAX_SHOW_MESSAGES+1)*FONT_SIZE + GAP*(MAX_SHOW_MESSAGES+1) + GAP);

        text.setFont(game.gfx.font);
        text.setCharacterSize(FONT_SIZE);
        text.setOutlineThickness(1.f);
        text.setOutlineColor(sf::Color::Black);

        input_text.setFont(game.gfx.font);
        input_text.setCharacterSize(FONT_SIZE);
        input_text.setFillColor(sf::Color::White);
        input_text.setOutlineThickness(1.f);
        input_text.setOutlineColor(sf::Color::Black);
    }

    void ChatUI::focusGained(GameState &game) {
        game.text_input = shared_from_this();
    }

    void ChatUI::focusLost(GameState &game) {
        if (game.text_input && game.text_input.get() == this) { game.text_input = nullptr; }
        takeString();
        visual_pos = 0;
        updateInputText(game);
    }

    bool ChatUI::scroll(GameState &game, const sf::Vector2f &position, float delta) {
        // Only do scroll if we are typing a message
        if (!game.text_input || game.text_input.get() != this)
            return false;

        if (delta > 0) {
            if (current_scroll < game.chat.numMessages()) {
                ++current_scroll;
            }
        } else {
            if (current_scroll > MAX_SHOW_MESSAGES) {
                --current_scroll;
            }
        }

        return true;
    }

    bool ChatUI::preHandleEvent(GameState &game, const sf::Event &event) {
        // 13 - newline control symbol. Not '\n' unfortunately...
        if (event.type == sf::Event::TextEntered && event.text.unicode == 13) {
            if (buffer.getSize() != 0) {
                game.chat.print({ takeString(), sf::Color::White });
                visual_pos = 0;
                updateInputText(game);
            }

            return true;
        }

        return false;
    }

    void ChatUI::onBufferUpdated(GameState &game) {
        updateInputText(game);
    }

    void ChatUI::onPosUpdated(GameState &game) {
        if (pos < visual_pos) {
            if (visual_pos - pos > game.chat.max_width/2) {
                --visual_pos;
            }
        } else {
            visual_pos = pos;
        }

        updateInputText(game);
    }

    void ChatUI::updateInputText(GameState &game) {
        size_t substring_start = visual_pos > game.chat.max_width ? visual_pos - game.chat.max_width : 0;
        size_t substring_end = std::min(substring_start+game.chat.max_width, buffer.getSize());

        input_text.setString(buffer.substring(substring_start, substring_end-substring_start));
    }

    void ChatUI::update(GameState &game, float dtime) {
        if ((game.text_input && game.text_input.get() == this) || game.chat.numTotalMessages() > last_total_messages) {
            visible_timer = 4.f;
        } else {
            visible_timer -= dtime;

            if (visible_timer < 0) {
                visible_timer = 0;
            } else {
                visible = true;
            }
        }

        last_total_messages = game.chat.numTotalMessages();
    }

    void ChatUI::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        if (visible_timer == 0) return;

        sf::Transform combined_transform = getTransform() * parent_transform;

        uint8_t alpha = (uint8_t)(base_transparency * std::min(visible_timer, 1.f));
        uint8_t text_alpha = (uint8_t)(255 * std::min(visible_timer, 1.f));

        sf::Color bg_color = background_color;
        bg_color.a = alpha;

        background_shape.setFillColor(bg_color);
        target.draw(background_shape, combined_transform);

        unsigned min_msg_num = current_scroll - MAX_SHOW_MESSAGES;
        unsigned max_msg_num = current_scroll;
        for (unsigned msg_num = min_msg_num; msg_num < max_msg_num; ++msg_num) {
            text.setPosition(0, int((max_msg_num - msg_num-1)*FONT_SIZE + GAP*(max_msg_num - msg_num)));

            const Message &message = game.chat.getMessage(msg_num);

            sf::Color msg_color = message.color;
            msg_color.a = text_alpha;

            text.setString(message.text);
            text.setFillColor(msg_color);

            target.draw(text, combined_transform);
        }

        if (game.text_input && game.text_input.get() == this) {
            sf::Color i_color = input_color;
            i_color.a = alpha;

            input_shape.setFillColor(i_color);
            target.draw(input_shape, combined_transform);

            target.draw(input_text, combined_transform);

            unsigned cursor_pos = std::min(std::min(game.chat.max_width - visual_pos + (unsigned)pos, (unsigned)pos), game.chat.max_width);

            cursor_shape.setPosition(input_text.findCharacterPos(cursor_pos));
            target.draw(cursor_shape, combined_transform);
        }

    }
}
