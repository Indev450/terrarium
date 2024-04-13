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

#include <algorithm>
#include <numeric>

#include "inventory.hpp"
#include "../utils/binary_io.hpp"

namespace Terrarium {

    void Inventory::addItem(ItemStack &new_item) {
        if (new_item.empty()) {
            return;
        }

        // First, try to find non-empty item stacks to merge
        for (auto &item: items) {
            // Don't forget that empty item stacks are still "mergable"
            if (item->empty()) {
                continue;
            }

            item->merge(new_item);

            if (new_item.empty()) {
                return;
            }
        }

        // If new_item is still not empty, try to merge it with empty
        // item stacks
        auto it = std::find_if(items.begin(), items.end(), [] (auto &item) { return item->empty(); });

        if (it != items.end()) {
            (*it)->swap(new_item);
        }
    }

    unsigned int Inventory::takeItems(std::shared_ptr<ItemDef> type, unsigned int count) {
        for (auto &item: items) {
            if (item->getDef() != type) {
                continue;
            }

            count = -item->add(-count);

            if (count == 0) {
                return 0;
            }
        }

        return count;
    }

    unsigned int Inventory::countItems(std::shared_ptr<ItemDef> type) const {
        return std::accumulate(items.cbegin(), items.cend(), 0, [&] (size_t acc, auto &item) { return acc + (item->getDef() == type ? item->getCount() : 0); } );
    }

    std::shared_ptr<ItemStack> Inventory::find(std::shared_ptr<ItemDef> type) {
        auto it = std::find_if(items.begin(), items.end(), [&] (auto &item) { return item->getDef() == type && !item->empty(); });

        return it == items.cend() ? nullptr : *it;
    }

    std::shared_ptr<ItemStack> Inventory::find(const std::string &tag) {
        auto it = std::find_if(items.begin(), items.end(), [&] (auto &item) {
            if (item->empty()) return false;

            auto def = item->getDef();
            return def && def->tags.count(tag) != 0;
        });

        return it == items.cend() ? nullptr : *it;
    }

    void Inventory::load(std::istream &s, GameState &game) {
        items.clear();

        resize(read<uint16_t>(s));

        uint16_t entries = read<uint16_t>(s);

        for (unsigned int i = 0; i < entries; ++i) {
            uint16_t slot = read<uint16_t>(s);

            items[slot]->load(s, game);
        }
    }

    void Inventory::save(std::ostream &s) {
        write<uint16_t>(s, items.size());

        uint16_t entries = countUsedSlots();

        write<uint16_t>(s, entries);

        uint16_t slot = 0;

        for (auto &item: items) {
            if (!item->empty()) {
                write<uint16_t>(s, slot);
                item->save(s);
            }

            ++slot;
        }
    }

} // namespace Terrarium
