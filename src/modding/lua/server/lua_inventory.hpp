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

#ifndef MODDING_LUA_INVENTORY_HPP
#define MODDING_LUA_INVENTORY_HPP

#define LUA_INVENTORY "InventoryRef"

#include <stdexcept>

#include <lua.hpp>

#include "../../../item/inventory.hpp"

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaInventoryAPI {

        class LuaInventoryUD {
            std::weak_ptr<Inventory> inventory_ref;

        public:
            LuaInventoryUD(std::weak_ptr<Inventory> _inventory_ref);

            bool isValid();

            void resize(unsigned int size);

            unsigned int size();

            std::shared_ptr<ItemStack> get(unsigned int i);

            void addItem(ItemStack &new_item);

            unsigned int takeItems(std::shared_ptr<ItemDef> type, unsigned int count);

            unsigned int countItems(std::shared_ptr<ItemDef> type);

            std::shared_ptr<ItemStack> find(std::shared_ptr<ItemDef> type);

            inline std::shared_ptr<Inventory> checkedLock() {
                std::shared_ptr<Inventory> inventory = inventory_ref.lock();

                if (!inventory) {
                    throw std::invalid_argument("inventory ref is expired");
                }

                return inventory;
            }
        };

        void init(LuaModdingInterface &lua_interface);

        // Inventory-related core module functions

        // void core._open_inventory_ui(InventoryRef inventory, unsigned int width, unsigned int height)
        int open_inventory_ui(lua_State *L);

        // InventoryRef userdata methods

        // bool InventoryRef:isValid()
        int inventory_is_valid(lua_State *L);

        // void InventoryRef:resize(unsigned int size)
        int inventory_resize(lua_State *L);

        // unsigned int InventoryRef:size()
        int inventory_size(lua_State *L);

        // ItemStack/nil InventoryRef:get(unsigned int i)
        int inventory_get(lua_State *L);

        // void InventoryRef:add_item(ItemStack new_item)
        int inventory_add_item(lua_State *L);

        // unsigned int InventoryRef:take_items(string name, unsigned int count)
        int inventory_take_items(lua_State *L);

        // unsigned int InventoryRef:count_items(string name)
        int inventory_count_items(lua_State *L);

        // ItemStack/nil InventoryRef:find(string name)
        int inventory_find(lua_State *L);

        // Functions to call from C++

        void push_inventory(lua_State *L, std::weak_ptr<Inventory> inventory_ref);

        std::shared_ptr<ItemDef> item_by_name(LuaModdingInterface &lua_interface, int idx);

    } // namespace LuaInventoryAPI

} // namespace Terrarium

#endif
