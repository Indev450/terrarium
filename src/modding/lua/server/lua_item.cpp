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

#include <cassert>
#include <stdexcept>

#include "lua_item.hpp"
#include "lua_interface.hpp"
#include "../common/lua_util.hpp"
#include "../common/lua_field_checker.hpp"

namespace Terrarium {

    namespace LuaItemAPI {

        LuaItemStackUD::LuaItemStackUD():
            istack(std::make_shared<ItemStack>())
        {}

        LuaItemStackUD::LuaItemStackUD(std::shared_ptr<ItemStack> _istack):
            istack(_istack)
        {
            assert(_istack);
        }

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_register_item", register_item);
            lua_interface.registerFunction("_new_item_stack", new_item_stack);

            lua_State *L = lua_interface.getLuaState();

            luaL_newmetatable(L, LUA_ITEMSTACK); // push metatable

            lua_newtable(L); // push __index table

            // This one needs access to lua_interface->game->item_defs
            lua_interface.pushClosure(itemstack_set);
            lua_setfield(L, -2, "set");

            lua_pushcfunction(L, itemstack_add);
            lua_setfield(L, -2, "add");

            lua_pushcfunction(L, itemstack_merge);
            lua_setfield(L, -2, "merge");

            lua_pushcfunction(L, itemstack_get_item_name);
            lua_setfield(L, -2, "get_item_name");

            lua_pushcfunction(L, itemstack_get_item_count);
            lua_setfield(L, -2, "get_item_count");

            lua_pushcfunction(L, itemstack_has_tag);
            lua_setfield(L, -2, "has_tag");

            lua_pushcfunction(L, itemstack_empty);
            lua_setfield(L, -2, "empty");

            lua_setfield(L, -2, "__index"); // pop __index table

            // Add destructor
            lua_pushcfunction(L, LuaUtil::call_destructor<LuaItemStackUD>);
            lua_setfield(L, -2, "__gc");

            lua_pop(L, 1); // pop metatable
        }

        int register_item(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            std::shared_ptr<ItemDef> item_def = std::make_shared<ItemDef>();

            const char *name = luaL_checkstring(L, 1);
            item_def->name = name;

            luaL_checktype(L, 2, LUA_TTABLE);

            LuaUtil::FieldChecker checker(L, "ItemDef", 2);

            try {
                item_def->description = checker.checkstring("description");

                const char *inventory_image = checker.checkstring("inventory_image");
                item_def->inventory_image.setTexture(lua_interface->game->gfx.textures.get(inventory_image));

                uint16_t max_count = checker.checkunsigned("max_count");

                if (max_count == 0) {
                    checker.fielderror("max_count", "number should be natural");
                }

                checker.rfield("tags", [&] () {
                    int len = luaL_len(L, -1);

                    for (int i = 1; i <= len; ++i) {
                        // TODO - add another set of functions for integer indices instead of
                        // strings in field checker
                        lua_geti(L, -1, i);

                        if (lua_type(L, -1) == LUA_TSTRING) {
                            item_def->tags.insert(lua_tostring(L, -1));
                            lua_pop(L, 1);
                        } else {
                            lua_pop(L, 1);
                            throw std::invalid_argument("tags must only contain strings");
                        }
                    }
                }, true);

                item_def->max_count = max_count;

                lua_interface->game->item_defs.add(item_def);
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

            return 0;
        }

        int new_item_stack(lua_State *L) {
            void *item_stack_mem = lua_newuserdatauv(L, sizeof(LuaItemStackUD), 0);

            new (item_stack_mem) LuaItemStackUD();

            luaL_setmetatable(L, LUA_ITEMSTACK);

            return 1;
        }

        int itemstack_set(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            LuaItemStackUD *item_stack = reinterpret_cast<LuaItemStackUD*>(luaL_checkudata(L, 1, LUA_ITEMSTACK));

            const char *name = luaL_checkstring(L, 2);

            uint16_t count = 1;

            if (lua_gettop(L) != 2) {
                count = LuaUtil::checkinteger_ranged<uint16_t>(L, 3);
            }

            item_stack->istack->set(lua_interface->game->item_defs.get(name), count);

            return 0;
        }

        int itemstack_add(lua_State *L) {
            LuaItemStackUD *item_stack = reinterpret_cast<LuaItemStackUD*>(luaL_checkudata(L, 1, LUA_ITEMSTACK));

            int change = luaL_checkinteger(L, 2);

            lua_pushinteger(L, item_stack->istack->add(change));

            return 1;
        };

        int itemstack_merge(lua_State *L) {
            LuaItemStackUD *first = reinterpret_cast<LuaItemStackUD*>(luaL_checkudata(L, 1, LUA_ITEMSTACK));
            LuaItemStackUD *second = reinterpret_cast<LuaItemStackUD*>(luaL_checkudata(L, 2, LUA_ITEMSTACK));

            first->istack->merge(*second->istack);

            return 0;
        }

        int itemstack_get_item_name(lua_State *L) {
            LuaItemStackUD *item_stack = reinterpret_cast<LuaItemStackUD*>(luaL_checkudata(L, 1, LUA_ITEMSTACK));

            std::shared_ptr<ItemDef> def = item_stack->istack->getDef();

            if (!item_stack->istack->empty()) {
                lua_pushstring(L, def->name.c_str());
            } else {
                lua_pushnil(L);
            }

            return 1;
        }

        int itemstack_get_item_count(lua_State *L) {
            LuaItemStackUD *item_stack = reinterpret_cast<LuaItemStackUD*>(luaL_checkudata(L, 1, LUA_ITEMSTACK));

            // Item stack can be empty (item_def is nullptr), but item count is not 0.
            // In that case we just return 0
            if (item_stack->istack->empty()) {
                lua_pushinteger(L, 0);
            } else {
                lua_pushinteger(L, static_cast<int>(item_stack->istack->getCount()));
            }

            return 1;
        }

        int itemstack_has_tag(lua_State *L) {
            LuaItemStackUD *item_stack = reinterpret_cast<LuaItemStackUD*>(luaL_checkudata(L, 1, LUA_ITEMSTACK));

            const char *tag = luaL_checkstring(L, 2);

            lua_pushboolean(L, item_stack->istack->hasTag(tag));

            return 1;
        }

        int itemstack_empty(lua_State *L) {
            LuaItemStackUD *item_stack = reinterpret_cast<LuaItemStackUD*>(luaL_checkudata(L, 1, LUA_ITEMSTACK));

            lua_pushboolean(L, item_stack->istack->empty());

            return 1;
        }

        void push_itemstack(lua_State *L, std::shared_ptr<ItemStack> istack) {
            if (!istack) {
                lua_pushnil(L);
                return;
            }

            void *item_stack_mem = lua_newuserdatauv(L, sizeof(LuaItemStackUD), 0);

            new (item_stack_mem) LuaItemStackUD(istack);

            luaL_setmetatable(L, LUA_ITEMSTACK);
        }

    } // namespace LuaItemAPI

} // namespace Terrarium
