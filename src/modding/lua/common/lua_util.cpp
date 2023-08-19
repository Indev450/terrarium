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

#include <cstdint>
#include <cstring>

#include "../common/lua_util.hpp"
#include "../common/lua_field_checker.hpp"

namespace Terrarium {

    namespace LuaUtil {

        bool checkboolean(lua_State *L, int idx) {
            luaL_checktype(L, idx, LUA_TBOOLEAN);

            return lua_toboolean(L, idx);
        }

        lua_Number checknumber_ranged(lua_State *L, int idx, lua_Number min, lua_Number max) {
            lua_Number num = luaL_checknumber(L, idx);

            if (num < min || num > max) {
                char errormsg[128];
                snprintf(errormsg, 128, "should be in range from %.5lf to %.5lf", min, max);
                luaL_argerror(L, idx, errormsg);
            }

            return num;
        }

        void *trysubclass(lua_State *L, int idx, const std::string &base) {
            if (lua_getmetatable(L, idx)) { // If object has metatable, push it
                lua_getfield(L, -1, "__name"); // push __name
                std::string name = luaL_checkstring(L, -1);
                lua_pop(L, 2); // pop __name and metatable

                // If `__name` starts with `base`
                if (name.rfind(base, 0) == 0) {
                    return lua_touserdata(L, idx);
                } else {
                    return nullptr;
                }
            }

            return nullptr;
        }

        void *checksubclass(lua_State *L, int idx, const std::string &base) {
            void *result = trysubclass(L, idx, base);

            if (!result) {
                char errormsg[100];
                snprintf(errormsg, 100, "expected %s subclass", base.c_str());
                luaL_argerror(L, idx, errormsg);
            }

            return result;
        }

        int pcall(lua_State *L, int nargs, int nret) {
            int hpos = lua_gettop(L) - nargs;

            int ret = 0;

            lua_pushcfunction(L, add_traceback);

            lua_insert(L, hpos);

            ret = lua_pcall(L, nargs, nret, hpos);

            lua_remove(L, hpos);

            return ret;
        }

        int add_traceback(lua_State *L) {
            luaL_traceback(L, L, lua_tostring(L, 1), 1);

            return 1;
        }

        void printerr(lua_State *L) {
            const char *msg = lua_tostring(L, -1);
            std::cerr<<msg<<std::endl;
            lua_pop(L, 1);
        }

        void error(lua_State *L, const char *msg) {
            printerr(L);

            throw std::runtime_error(msg);
        }

        bool run_script(lua_State *L, const char *path) {
            if (luaL_loadfile(L, path) != LUA_OK) {
                printerr(L);
                return false;
            }

            if (pcall(L, 0, LUA_MULTRET) != LUA_OK) {
                printerr(L);
                return false;
            }

            return true;
        }

        void push_vector2f(lua_State *L, const sf::Vector2f &vec) {
            lua_getfield(L, LUA_REGISTRYINDEX, "vec2.new");

            lua_pushnumber(L, vec.x);
            lua_pushnumber(L, vec.y);

            lua_call(L, 2, 1);
        }

        void push_vector2i(lua_State *L, const sf::Vector2i &vec) {
            lua_getfield(L, LUA_REGISTRYINDEX, "vec2.new");

            lua_pushinteger(L, vec.x);

            lua_pushinteger(L, vec.y);

            lua_call(L, 2, 1);
        }

        void push_floatrect(lua_State *L, const sf::FloatRect &rect) {
            lua_newtable(L);

            lua_pushnumber(L, rect.left);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, rect.top);
            lua_setfield(L, -2, "y");

            lua_pushnumber(L, rect.width);
            lua_setfield(L, -2, "width");

            lua_pushnumber(L, rect.height);
            lua_setfield(L, -2, "height");
        }

        sf::FloatRect checkfloatrect(lua_State *L, int idx) {
            luaL_checktype(L, idx, LUA_TTABLE);

            FieldChecker checker(L, "FloatRect", idx);

            sf::FloatRect rect;

            try {
                rect.left = checker.checknumber("x");

                rect.top = checker.checknumber("y");

                rect.width = checker.checknumber("width");

                rect.height = checker.checknumber("height");
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

            return rect;
        }

        // Code duplication :(
        // I don't know how to avoid it
        sf::IntRect checkintrect(lua_State *L, int idx) {
            luaL_checktype(L, idx, LUA_TTABLE);

            FieldChecker checker(L, "IntRect", idx);

            sf::IntRect rect;

            try {
                rect.left = checker.checkinteger("x");

                rect.top = checker.checkinteger("y");

                rect.width = checker.checkinteger("width");

                rect.height = checker.checkinteger("height");
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

            return rect;
        }

        sf::Color checkcolor(lua_State *L, int idx) {
            luaL_checktype(L, idx, LUA_TTABLE);

            FieldChecker checker(L, "Color", idx);

            sf::Color color;

            try {
                color.r = checker.checkunsigned("r");

                color.g = checker.checkunsigned("g");

                color.b = checker.checkunsigned("b");

                if (checker.havefield("a")) {
                    color.a = checker.checkunsigned("a");
                }
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

            return color;
        }

        template <>
        sf::Vector2<float> checkvector<float>(lua_State *L, int idx) {
            luaL_checktype(L, idx, LUA_TTABLE);

            FieldChecker checker(L, "Vector2f", idx);

            sf::Vector2<float> vec;

            try {
                vec.x = checker.checknumber("x");
                vec.y = checker.checknumber("y");
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

            return vec;
        }

        template <>
        sf::Vector2<int> checkvector<int>(lua_State *L, int idx) {
            luaL_checktype(L, idx, LUA_TTABLE);

            FieldChecker checker(L, "Vector2i", idx);

            sf::Vector2<int> vec;

            try {
                vec.x = checker.checkinteger("x");
                vec.y = checker.checkinteger("y");
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

            return vec;
        }

        template <>
        sf::Vector2<uint8_t> checkvector<uint8_t>(lua_State *L, int idx) {
            luaL_checktype(L, idx, LUA_TTABLE);

            FieldChecker checker(L, "Vector2<uint8_t>", idx);

            sf::Vector2<uint8_t> vec;

            try {
                vec.x = checker.checkunsigned("x");
                vec.y = checker.checkunsigned("y");
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

            return vec;
        }

    } // namespace LuaUtil

} // namespace Terrarium
