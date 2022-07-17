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

#include <algorithm>

#include "inventory.hpp"

namespace Terrarium {

    void Inventory::addItem(ItemStack &new_item) {
        // First, try to find non-empty item stacks to merge
        for (auto &item: items) {
            item->merge(new_item);

            if (new_item.empty()) {
                return;
            }
        }

        // If new_item is still not empty, try to merge it with empty
        // item stacks
        for (auto &item: items) {
            if (item->empty()) {
                item->swap(new_item);
            }
        }
    }

    unsigned int Inventory::takeItems(std::shared_ptr<ItemDef> type, unsigned int count) {
        for (auto &item: items) {
            count = -item->add(-count);

            if (count == 0) {
                return 0;
            }
        }

        return count;
    }

    unsigned int Inventory::countItems(std::shared_ptr<ItemDef> type) {
        unsigned int result = 0;

        for (auto item: items) {
            if (item->getDef() == type) {
                result += item->getCount();
            }
        }

        return result;
    }

    std::shared_ptr<ItemStack> Inventory::find(std::shared_ptr<ItemDef> type) {
        for (auto &item: items) {
            if (item->getDef() == type && !item->empty()) {
                return item;
            }
        }

        return nullptr;
    }

} // namespace Terrarium
