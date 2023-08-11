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

#include <memory>

#include "lua_interface.hpp"
#include "../common/lua_util.hpp"
#include "lua_hud_bar.hpp"
#include "../../../ui/bar.hpp"

namespace Terrarium {

    namespace LuaHudBarAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_hud_bar_create", hud_bar_create);
            lua_interface.registerFunction("_hud_bar_set_value", hud_bar_set_value);
            lua_interface.registerFunction("_hud_bar_set_max_value", hud_bar_set_max_value);
            lua_interface.registerFunction("_hud_bar_remove", hud_bar_remove);
        }

        int hud_bar_create(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            std::string name = luaL_checkstring(L, 1);

            if (!lua_istable(L, 2)) {
                return luaL_error(L, "expected table as bar def");
            }

            lua_getfield(L, 2, "geometry");
            sf::FloatRect geometry = LuaUtil::checkfloatrect(L, -1);
            lua_pop(L, 1);

            auto bar = std::make_unique<Bar>(
                sf::Vector2f(geometry.width, geometry.height),
                lua_interface->game->gfx
            );

            bar->setPosition(geometry.left, geometry.top);

            lua_getfield(L, 2, "text_style");

            static const char* styles[] = {
                "Division",
                "Value",
                "None",
                nullptr,
            };

            int text_style_i = luaL_checkoption(L, -1, "None", styles);

            BarTextStyle text_style;

            switch (text_style_i) {
                case 0: text_style = BarTextStyle::Division; break;
                case 1: text_style = BarTextStyle::Value; break;
                default: text_style = BarTextStyle::None; break;
            }

            lua_pop(L, 1);

            bar->setTextStyle(text_style);

            if (text_style != BarTextStyle::None) {
                lua_getfield(L, 2, "text");
                bar->setText(luaL_checkstring(L, -1));
                lua_pop(L, 1);

                lua_getfield(L, 2, "text_size");
                bar->setTextSize(LuaUtil::checkinteger_ranged<unsigned int>(L, -1));
                lua_pop(L, 1);

                lua_getfield(L, 2, "text_color");
                bar->setTextColor(LuaUtil::checkcolor(L, -1));
                lua_pop(L, 1);
            }

            lua_getfield(L, 2, "background_color");
            bar->setBackgroundColor(LuaUtil::checkcolor(L, -1));
            lua_pop(L, 1);

            lua_getfield(L, 2, "bar_color");
            bar->setBarColor(LuaUtil::checkcolor(L, -1));
            lua_pop(L, 1);

            lua_getfield(L, 2, "max_value");
            bar->setMaxValue(luaL_checknumber(L, -1));
            lua_pop(L, 1);

            lua_interface->game->hud.addBar(name, std::move(bar));

            return 0;
        }

        int hud_bar_set_value(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Bar *bar = lua_interface->game->hud.getBar(luaL_checkstring(L, 1));

            if (bar != nullptr) {
                bar->setValue(luaL_checknumber(L, 2));
            }

            return 0;
        }

        int hud_bar_set_max_value(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Bar *bar = lua_interface->game->hud.getBar(luaL_checkstring(L, 1));

            if (bar != nullptr) {
                bar->setMaxValue(luaL_checknumber(L, 2));
            }

            return 0;
        }

        int hud_bar_remove(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            lua_interface->game->hud.delBar(luaL_checkstring(L, 1));

            return 0;
        }

    }

}
