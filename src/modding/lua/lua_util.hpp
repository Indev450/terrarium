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

#ifndef MODDING_LUA_UTIL_HPP
#define MODDING_LUA_UTIL_HPP

#include <iostream>
#include <string>
#include <limits>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <lua.hpp>

#include "../../graphics/animation.hpp"

namespace Terrarium {

    namespace LuaUtil {

        // Calls userdata class destructor. Use as __gc metatable function
        template <class T>
        int call_destructor(lua_State *L) {
            void *obj_mem = lua_touserdata(L, 1);

            T *obj = reinterpret_cast<T*>(obj_mem);

            obj->~T();

            return 0;
        }

        // Converts value at given index into boolean. Warns if it was not boolean,
        // but not raises error, unlike other check* functions
        bool checkboolean(lua_State *L, int idx);

        // Like luaL_checkinteger, but also checks if number is within numeric_limits
        // bounds. For example, if template argument is uint8_t, function will check
        // number to be in range from 0 to 255.
        template <class T>
        T checkinteger_ranged(lua_State *L, int idx) {
            lua_Integer num = luaL_checkinteger(L, idx);

            if (!std::numeric_limits<T>::is_signed && num < 0) {
                return luaL_error(L, "function argument #%d expected to be unsigned", idx);
            }

            if (num < std::numeric_limits<T>::min() || num > std::numeric_limits<T>::max()) {
                return luaL_error(L, "function argument #%d expected to be in range from %d to %d",
                    idx, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
            }

            return static_cast<T>(num);
        }

        // After getting a number, this function also checks that it is within
        // specified range (note that it uses open range, [min, max], not (min, max))
        lua_Number checknumber_ranged(lua_State *L, int idx, lua_Number min, lua_Number max);

        // Like luaL_checkudata, but instead of strict checking <metatable>.__name equals name,
        // this function checks <metatable>.__name starts with name. With that, inheritance
        // can be organized like this:
        // "Base" - base class
        // "Base.Derived" - derived class
        //
        // Found this solution at http://lua-users.org/lists/lua-l/2005-06/msg00000.html
        // (from stackoverflow answer)
        void *checksubclass(lua_State *L, int idx, const std::string &base);

        inline std::string subclass(const std::string &base, const std::string &derived) {
            return base + "." + derived;
        }

        // Alternative to lua_pcall that automatically adds traceback to error message
        int pcall(lua_State *L, int nargs, int nret);

        // Adds traceback to error message
        int add_traceback(lua_State *L);

        // Print error message occured in pcall and pop it
        void printerr(lua_State *L);

        bool run_script(lua_State *L, const char *path);

        void push_vector2f(lua_State *L, const sf::Vector2f &vec);

        void push_floatrect(lua_State *L, const sf::FloatRect &rect);

        sf::FloatRect checkfloatrect(lua_State *L, int idx);

        sf::IntRect checkintrect(lua_State *L, int idx);

        Animation checkanimation(lua_State *L, int idx);

    }

}

#endif
