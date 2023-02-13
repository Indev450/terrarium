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

#include "lua_ui.hpp"
#include "lua_client_interface.hpp"
#include "../server/lua_util.hpp"
#include "../../../game.hpp"
#include "../../../ui/frame.hpp"

namespace Terrarium {

    namespace LuaClientUIAPI {

        void init(LuaClientModdingInterface &lua_interface) {
            lua_interface.registerFunction("_frame_new", frame_new);
            lua_interface.registerFunction("_set_hud_item_visible", set_hud_item_visible);
            lua_interface.registerFunction("_is_hud_item_visible", is_hud_item_visible);
            lua_interface.registerFunction("_set_hud_item_position", set_hud_item_position);
            lua_interface.registerFunction("_set_hud_item_origin", set_hud_item_origin);
        }

        int frame_new(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Hud &hud = lua_interface->game->hud;

            std::string id = luaL_checkstring(L, 1);

            unsigned width = LuaUtil::checkinteger_ranged<unsigned>(L, 2);
            unsigned height = LuaUtil::checkinteger_ranged<unsigned>(L, 3);

            std::unique_ptr<UIFrame> frame = std::make_unique<UIFrame>(width, height, id);

            lua_pushlightuserdata(L, &frame->rtexture);

            hud.addElement(id, std::move(frame), false);

            return 1;
        }

        int set_hud_item_visible(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Hud &hud = lua_interface->game->hud;

            bool focus = true;

            if (lua_gettop(L) > 2) {
                focus = LuaUtil::checkboolean(L, 3);
            }

            hud.setVisible(luaL_checkstring(L, 1), LuaUtil::checkboolean(L, 2), focus);

            return 0;
        }

        int is_hud_item_visible(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Hud &hud = lua_interface->game->hud;

            lua_pushboolean(L, hud.isVisible(luaL_checkstring(L, 1)));

            return 1;
        }

        int set_hud_item_position(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Hud &hud = lua_interface->game->hud;

            UIElement *item = hud.getElement(luaL_checkstring(L, 1));
            sf::Vector2f position = LuaUtil::checkvector<float>(L, 2);

            if (item) {
                item->setPosition(position);
            }

            return 0;
        }

        int set_hud_item_origin(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Hud &hud = lua_interface->game->hud;

            static const char* origin_names[] = {
                "TopLeft",
                "ScreenCenter",
                nullptr,
            };

            static const ScreenTransformable::Origin origin_types[] = {
                ScreenTransformable::TopLeft,
                ScreenTransformable::ScreenCenter,
            };

            UIElement *item = hud.getElement(luaL_checkstring(L, 1));
            ScreenTransformable::Origin origin = origin_types[luaL_checkoption(L, 2, nullptr, origin_names)];

            if (item) {
                item->setOriginType(origin);
            }

            return 0;
        }

    }

}
