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

#include "item_stack.hpp"

namespace Terrarium {

    void ItemStack::set(std::shared_ptr<ItemDef> def, uint16_t count) {
        this->def = def;
        this->count = count;

        if (def) {
            this->count = std::min(count, def->max_count);
        }
    }

    int ItemStack::add(int change) {
        int left = 0;

        if (!def) {
            count += change;
            return left;
        }

        if (change < 0 && change < -static_cast<int>(count)) {
            left = change + count;

            count = 0;
        } else if (change + count > def->max_count) {
            left = change + count - def->max_count;

            count = def->max_count;
        } else {
            count += change;
        }

        return left;
    }

    void ItemStack::swap(ItemStack &other) {
        auto other_def = other.def;
        auto other_count = other.count;

        other.set(def, count);

        def = other_def;
        count = other_count;
    }

} // namespace Terrarium
