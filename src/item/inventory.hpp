/*
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

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include <memory>

#include "item_stack.hpp"

namespace Terrarium {

    class Inventory {
        std::vector<std::shared_ptr<ItemStack>> items;

    public:
        Inventory(unsigned int size) {
            resize(size);

            for (unsigned int i = 0; i < size; ++i) {
                if (!items[i]) {
                    items[i] = std::make_shared<ItemStack>();
                }
            }
        }

        inline void resize(unsigned int size) {
            items.resize(size);
        }

        inline unsigned int size() {
            return items.size();
        }

        inline std::shared_ptr<ItemStack> get(unsigned int i) {
            if (i >= items.size()) {
                return nullptr;
            }

            return items[i];
        }

        // Add new_item into inventory. If possible, it will merge with existing
        // item stacks with same type. If some items couldn't fit, they are left in new_item
        void addItem(ItemStack &new_item);

        // Take at most 'count' items of given type and return how many
        // items in this "negative item stack left" (should be same as max(count - countItems(type), 0))
        unsigned int takeItems(std::shared_ptr<ItemDef> type, unsigned int count);

        // Count how many items of given type exist in inventory.
        // Can be used in crafts, as well as takeItems()
        unsigned int countItems(std::shared_ptr<ItemDef> type);

        // Find any ItemStack that have required type
        std::shared_ptr<ItemStack> find(std::shared_ptr<ItemDef> type);
    };

}

#endif
