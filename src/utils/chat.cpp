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

namespace Terrarium {

    Chat::Chat(unsigned _max_messages, unsigned _max_width):
        max_width(_max_width) {

        setMaxMessages(_max_messages);
    }

    void Chat::setMaxMessages(unsigned new_max_messages) {
        max_messages = new_max_messages;

        if (log.size() > max_messages) {
            log.resize(max_messages);
        }
    }

    void Chat::pushMessage(const Message &msg) {
        if (log.size() == max_messages) {
            log.pop_back();
        }

        log.push_front(msg);
    }

    void Chat::print(Message msg) {
        ++total_messages;

        Message partial_msg;
        partial_msg.color = msg.color;

        while (msg.text.getSize() > max_width) {
            unsigned split_pos = max_width;

            for (unsigned i = max_width; i > 0; --i) {
                if (msg.text[i] == ' ') {
                    split_pos = i;
                    break;
                }
            }

            partial_msg.text = msg.text.substring(0, split_pos);
            msg.text = msg.text.substring(split_pos+1, msg.text.getSize()-(split_pos+1));

            pushMessage(partial_msg);
        }

        pushMessage(msg);
    }

    const Message &Chat::getMessage(unsigned num) const {
        if (num >= log.size()) {
            return invalid_message;
        }

        return log[num];
    }

    unsigned Chat::numMessages() const {
        return log.size();
    }

    unsigned Chat::numTotalMessages() const {
        return total_messages;
    }
}
