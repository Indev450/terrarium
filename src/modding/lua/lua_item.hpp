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

#ifndef MODDING_LUA_ITEM_HPP
#define MODDING_LUA_ITEM_HPP

#define LUA_ITEMSTACK "ItemStack"

#include <lua.hpp>
#include <memory>

#include "../../item/item_stack.hpp"

namespace Terrarium {

    class LuaModdingInterface;

    namespace LuaItemAPI {

        // Note that even if shared_ptr is nullable, it never should be null,
        // so ItemStack's userdata methods always expect istack to be valid
        struct LuaItemStackUD {
            std::shared_ptr<ItemStack> istack;

            LuaItemStackUD();
            LuaItemStackUD(std::shared_ptr<ItemStack> _istack);
        };

        void init(LuaModdingInterface &lua_interface);

        // Item-related core module functions

        // void core._register_item(ItemDef item_def)
        int register_item(lua_State *L);

        // ItemStack core._new_item_stack()
        int new_item_stack(lua_State *L);

        // ItemStack userdata methods

        // void ItemStack:set(string item_name[, uint16_t count])
        int itemstack_set(lua_State *L);

        // int ItemStack:add(int change)
        int itemstack_add(lua_State *L);

        // void ItemStack:merge(ItemStack other)
        int itemstack_merge(lua_State *L);

        // string/nil ItemStack:get_item_name()
        int itemstack_get_item_name(lua_State *L);

        // int ItemStack:get_item_count()
        int itemstack_get_item_count(lua_State *L);

        // bool ItemStack:empty()
        int itemstack_empty(lua_State *L);

        // Functions to call from C++

        void push_itemstack(lua_State *L, std::shared_ptr<ItemStack> istack);

    } // namespace LuaItemAPI

} // namespace Terrarium

#endif
