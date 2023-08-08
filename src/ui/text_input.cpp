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

#include "text_input.hpp"

namespace Terrarium {

    sf::String TextInput::takeString() {
        sf::String copy = buffer;

        buffer.clear();
        pos = 0;

        return copy;
    }

    bool TextInput::handleEvent(GameState &game, const sf::Event &event) {
        if (preHandleEvent(game, event)) { return true; }

        switch (event.type) {
            case sf::Event::TextEntered:
            {
                switch (event.text.unicode) {
                    case 0x08: // Backspace
                    {
                        if (pos) {
                            buffer.erase(pos-1);
                            --pos;
                            onPosUpdated(game);
                        }
                    }
                    break;

                    case 0x7f: // Delete
                    {
                        if (pos < buffer.getSize()) {
                            buffer.erase(pos);
                        }
                    }
                    break;

                    default:
                    {
                        // If not a control character
                        if (event.text.unicode > 31) {
                            buffer.insert(pos, event.text.unicode);
                            ++pos;
                            onPosUpdated(game);
                        }
                    }
                    break;
                }

                onBufferUpdated(game);
            }
            break;

            case sf::Event::KeyPressed:
            {
                switch (event.key.code) {
                    case sf::Keyboard::Left:
                    {
                        if (pos) { --pos; }
                    }
                    break;

                    case sf::Keyboard::Right:
                    {
                        if (pos < buffer.getSize()) { ++pos; }
                    }
                    break;

                    default:
                        return false;
                    break;
                }

                onPosUpdated(game);
            }
            break;

            default:
                return false;
            break;
        }

        return true;
    }

}
