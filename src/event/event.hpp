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

#ifndef EVENT_HPP
#define EVENT_HPP

#include <cassert>
#include <memory>

#include "../entity/entity.hpp"
#include "../item/item_stack.hpp"

namespace Terrarium {

    struct ItemEvent {
        std::shared_ptr<ItemStack> item_stack;
        std::weak_ptr<Player> user;

        sf::Vector2f position;
    };

    struct Event {
        const enum Type {
            ItemUseStart = 0,
            ItemUseStop,

            ItemAltUseStart,
            ItemAltUseStop,

            ItemSelect, // Start wield item. Can be used for torches
        } type;

        const union {
            ItemEvent *item = nullptr;
            // ...more events data
        };

        Event(Type _type, ItemEvent *_item):
            type(_type), item(_item) {

            assert(type <= ItemSelect);
        }

        // Delete copy because i don't know how union with smart pointers inside
        // would behave.
        Event(const Event &copy) = delete;
        Event &operator=(const Event &copy) = delete;

        // Sadly, there is no easy way to convert enum value to string, so i have to use this
        const char *getName() const {
            #define ENUM_TOSTRING(value) case value: return #value

            // Switch to warn myself if i add new event types and forget to stringify them
            switch (type) {
                ENUM_TOSTRING(ItemUseStart);
                ENUM_TOSTRING(ItemUseStop);

                ENUM_TOSTRING(ItemAltUseStart);
                ENUM_TOSTRING(ItemAltUseStop);

                ENUM_TOSTRING(ItemSelect);
            }

            // Execution flow should never reach this, but compiler still prints warning,
            // so i add this return;
            return nullptr;
        }

        ~Event() {
            switch (type) {
                case ItemUseStart:
                case ItemUseStop:
                case ItemAltUseStart:
                case ItemAltUseStop:
                case ItemSelect:
                {
                    delete item;
                }
                break;
            }
        }
    };

}

#endif
