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

#ifndef ITEM_STACK_HPP
#define ITEM_STACK_HPP

#include <memory>
#include <iostream>

#include "item_def.hpp"

namespace Terrarium {

    class GameState;

    class ItemStack {
        std::shared_ptr<ItemDef> def;
        uint16_t count = 0;

    public:
        ItemStack() = default;

        ItemStack(const ItemStack &copy) {
            set(copy.def, copy.count);
        }

        void set(std::shared_ptr<ItemDef> def, uint16_t count = 1);

        int add(int change);

        void swap(ItemStack &other);

        inline void merge(ItemStack &other) {
            if (!canMerge(other)) { return; }

            other.count = add(other.count);

            def = other.def;
        }

        inline bool canMerge(const ItemStack &other) {
            if (empty() || other.empty()) {
                return true;
            }

            if (def == other.def) {
                return count < def->max_count && other.count < other.def->max_count;
            }

            return false;
        }

        inline std::shared_ptr<ItemDef> getDef() const {
            if (empty()) { return nullptr; }
            return def;
        }

        bool hasTag(const std::string &tag);

        inline uint16_t getCount() const {
            return count;
        }

        inline bool empty() const {
            return !def || count == 0;
        }

        // ItemStack save format:
        // string item_name
        // u16 count

        // Needs game state to get item def
        void load(std::istream &s, GameState &game);

        void save(std::ostream &s);
    };

}

#endif
