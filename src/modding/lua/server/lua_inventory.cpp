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

#include "lua_interface.hpp"
#include "lua_inventory.hpp"
#include "lua_item.hpp"
#include "lua_util.hpp"
#include "../../../ui/inventory.hpp"

namespace Terrarium {

    namespace LuaInventoryAPI {

        LuaInventoryUD::LuaInventoryUD(std::weak_ptr<Inventory> _inventory_ref):
            inventory_ref(_inventory_ref)
        {}

        bool LuaInventoryUD::isValid() {
            return !inventory_ref.expired();
        }

        void LuaInventoryUD::resize(unsigned int size) {
            std::shared_ptr<Inventory> inventory = checkedLock();

            inventory->resize(size);
        }

        unsigned int LuaInventoryUD::size() {
            std::shared_ptr<Inventory> inventory = checkedLock();

            return inventory->size();
        }

        std::shared_ptr<ItemStack> LuaInventoryUD::get(unsigned int i) {
            std::shared_ptr<Inventory> inventory = checkedLock();

            return inventory->get(i);
        }

        void LuaInventoryUD::addItem(ItemStack &new_item) {
            std::shared_ptr<Inventory> inventory = checkedLock();

            inventory->addItem(new_item);
        }

        unsigned int LuaInventoryUD::takeItems(std::shared_ptr<ItemDef> type, unsigned int count) {
            std::shared_ptr<Inventory> inventory = checkedLock();

            return inventory->takeItems(type, count);
        }

        unsigned int LuaInventoryUD::countItems(std::shared_ptr<ItemDef> type) {
            std::shared_ptr<Inventory> inventory = checkedLock();

            return inventory->countItems(type);
        }

        std::shared_ptr<ItemStack> LuaInventoryUD::find(std::shared_ptr<ItemDef> type) {
            std::shared_ptr<Inventory> inventory = checkedLock();

            return inventory->find(type);
        }

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_open_inventory_ui", open_inventory_ui);

            lua_State *L = lua_interface.getLuaState();

            luaL_newmetatable(L, LUA_INVENTORY); // push metatable

            lua_newtable(L); // push __index table

            lua_pushcfunction(L, inventory_is_valid);
            lua_setfield(L, -2, "is_valid");

            lua_pushcfunction(L, inventory_resize);
            lua_setfield(L, -2, "resize");

            lua_pushcfunction(L, inventory_size);
            lua_setfield(L, -2, "size");

            lua_pushcfunction(L, inventory_get);
            lua_setfield(L, -2, "get");

            lua_pushcfunction(L, inventory_add_item);
            lua_setfield(L, -2, "add_item");

            lua_interface.pushClosure(inventory_take_items);
            lua_setfield(L, -2, "take_items");

            lua_interface.pushClosure(inventory_count_items);
            lua_setfield(L, -2, "count_items");

            lua_interface.pushClosure(inventory_find);
            lua_setfield(L, -2, "find");

            lua_setfield(L, -2, "__index"); // pops __index table

            // Add destructor
            lua_pushcfunction(L, LuaUtil::call_destructor<LuaInventoryUD>);
            lua_setfield(L, -2, "__gc");

            lua_pop(L, 1); // pop metatable
        }

        int open_inventory_ui(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            auto inventory_ui = std::make_unique<InventoryUI>(
                lua_interface->game->gfx,
                LuaUtil::checkinteger_ranged<unsigned int>(L, 2),
                LuaUtil::checkinteger_ranged<unsigned int>(L, 3)
            );

            try {
                inventory_ui->inventory = inventory_ref->checkedLock();
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            lua_interface->game->hud.replaceElement("opened_inventory", std::move(inventory_ui));

            lua_interface->game->hud.setVisible("craft", true);
            lua_interface->game->hud.setVisible("inventory", true);
            lua_interface->game->hud.setVisible("opened_inventory", true);

            return 0;
        }

        int inventory_is_valid(lua_State *L) {
            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            lua_pushboolean(L, inventory_ref->isValid());

            return 1;
        }

        int inventory_resize(lua_State *L) {
            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            // Allows zero sized inventories
            unsigned int size = LuaUtil::checkinteger_ranged<unsigned int>(L, 2);

            try {
                inventory_ref->resize(size);
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 0;
        }

        int inventory_size(lua_State *L) {
            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            try {
                lua_pushinteger(L, inventory_ref->size());
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        int inventory_get(lua_State *L) {
            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            unsigned int i = LuaUtil::checkinteger_ranged<unsigned int>(L, 2);

            try {
                std::shared_ptr<ItemStack> item_stack = inventory_ref->get(i);

                if (item_stack) {
                    LuaItemAPI::push_itemstack(L, item_stack);
                } else {
                    lua_pushnil(L);
                }
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        int inventory_add_item(lua_State *L) {
            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            LuaItemAPI::LuaItemStackUD *item_stack_ud = reinterpret_cast<LuaItemAPI::LuaItemStackUD*>(luaL_checkudata(L, 2, LUA_ITEMSTACK));

            try {
                inventory_ref->addItem(*item_stack_ud->istack);
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 0;
        }

        int inventory_take_items(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            std::shared_ptr<ItemDef> type = item_by_name(*lua_interface, 2);

            unsigned int count = LuaUtil::checkinteger_ranged<unsigned int>(L, 3);

            try {
                lua_pushinteger(L, inventory_ref->takeItems(type, count));
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        int inventory_count_items(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            std::shared_ptr<ItemDef> type = item_by_name(*lua_interface, 2);

            try {
                lua_pushinteger(L, inventory_ref->countItems(type));
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        int inventory_find(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            LuaInventoryUD *inventory_ref = reinterpret_cast<LuaInventoryUD*>(luaL_checkudata(L, 1, LUA_INVENTORY));

            std::shared_ptr<ItemDef> type = item_by_name(*lua_interface, 2);

            try {
                std::shared_ptr<ItemStack> item_stack = inventory_ref->find(type);

                if (item_stack) {
                    LuaItemAPI::push_itemstack(L, item_stack);
                } else {
                    lua_pushnil(L);
                }
            } catch (const std::invalid_argument &e) {
                return luaL_error(L, e.what());
            }

            return 1;
        }

        void push_inventory(lua_State *L, std::weak_ptr<Inventory> inventory_ref) {
            void *inventory_ref_mem = lua_newuserdatauv(L, sizeof(LuaInventoryUD), 0);

            new (inventory_ref_mem) LuaInventoryUD(inventory_ref);

            luaL_setmetatable(L, LUA_INVENTORY);
        }

        std::shared_ptr<ItemDef> item_by_name(LuaModdingInterface &lua_interface, int idx) {
            lua_State *L = lua_interface.getLuaState();

            const char *name = luaL_checkstring(L, idx);

            std::shared_ptr<ItemDef> item_def = lua_interface.game->item_defs.get(name);

            if (!item_def) {
                luaL_error(L, "unknown item name: %s", name);
            }

            return item_def;
        }

    } // namespace LuaInventoryAPI

} // namespace Terrarium
