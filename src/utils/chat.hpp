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

#include <deque>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace Terrarium {

    struct Message {
        sf::String text;
        sf::Color color = sf::Color::White;
    };

    class Chat {
        std::deque<Message> log;
        Message invalid_message; // In case we requiested message out of range

        unsigned max_messages;

        // Total amount of messages, including discarded ones
        unsigned total_messages = 0;

    private:
        void pushMessage(const Message &msg);

    public:
        unsigned max_width;

    public:
        Chat(unsigned _max_messages, unsigned _max_width);

        void setMaxMessages(unsigned new_max_messages);

        void print(Message msg);

        const Message &getMessage(unsigned num) const;

        unsigned numMessages() const;

        unsigned numTotalMessages() const;
    };

}
