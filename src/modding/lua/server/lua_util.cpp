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

#include <cstdint>

#include "lua_util.hpp"

namespace Terrarium {

    namespace LuaUtil {

        bool checkboolean(lua_State *L, int idx) {
            if (!lua_isboolean(L, idx)) {
                std::string msg = "function argument #";
                msg += std::to_string(idx);
                msg += " expected to be boolean";

                lua_warning(L, msg.c_str(), false);
            }

            return lua_toboolean(L, idx);
        }

        lua_Number checknumber_ranged(lua_State *L, int idx, lua_Number min, lua_Number max) {
            lua_Number num = luaL_checknumber(L, idx);

            if (num < min || num > max) {
                luaL_error(L, "function argument #%d expected to be in range from %f to %f", idx, min, max);
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
                luaL_error(L, "function argument #%d expected to be %s subclass", idx, base.c_str());
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
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            sf::FloatRect rect;

            idx = lua_absindex(L, idx);

            lua_getfield(L, idx, "x");
            rect.left = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "y");
            rect.top = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "width");
            rect.width = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "height");
            rect.height = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            return rect;
        }

        // Code duplication :(
        // I don't know how to avoid it
        sf::IntRect checkintrect(lua_State *L, int idx) {
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            sf::IntRect rect;

            idx = lua_absindex(L, idx);

            lua_getfield(L, idx, "x");
            rect.left = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "y");
            rect.top = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "width");
            rect.width = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "height");
            rect.height = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            return rect;
        }

        sf::Color checkcolor(lua_State *L, int idx) {
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            sf::Color color;

            idx = lua_absindex(L, idx);

            lua_getfield(L, idx, "r");
            color.r = checkinteger_ranged<uint8_t>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "g");
            color.g = checkinteger_ranged<uint8_t>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "b");
            color.b = checkinteger_ranged<uint8_t>(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "a");

            if (!lua_isnil(L, -1)) {
                color.a = checkinteger_ranged<uint8_t>(L, -1);
            }

            lua_pop(L, 1);

            return color;
        }

        template <>
        sf::Vector2<float> checkvector<float>(lua_State *L, int idx) {
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            sf::Vector2<float> vec;

            lua_getfield(L, idx, "x");
            vec.x = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "y");
            vec.y = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            return vec;
        }

        template <>
        sf::Vector2<int> checkvector<int>(lua_State *L, int idx) {
            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            sf::Vector2<int> vec;

            lua_getfield(L, idx, "x");
            vec.x = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "y");
            vec.y = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            return vec;
        }

        Animation checkanimation(lua_State *L, int idx) {
            Animation anim;

            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            idx = lua_absindex(L, idx);

            lua_getfield(L, idx, "time_per_frame");
            anim.time_per_frame = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "next");

            if (!lua_isnil(L, -1)) {
                anim.next = luaL_checkstring(L, -1);
            }

            lua_pop(L, 1);

            lua_getfield(L, idx, "interpolation");

            static const char *interp_type_names[] = {
                "None",
                "Linear",
                nullptr,
            };

            static const Animation::Interpolation interp_type_values[] = {
                Animation::Interpolation::None,
                Animation::Interpolation::Linear,
            };

            anim.interpolation = interp_type_values[luaL_checkoption(L, -1, "None", interp_type_names)];

            lua_pop(L, 1);

            lua_getfield(L, idx, "frames");

            if (!lua_istable(L, -1)) {
                luaL_error(L, "<animation>.frames expected to be table");
            }

            int amount_frames = luaL_len(L, -1);
            for (int i = 1; i <= amount_frames; ++i) {
                AnimationFrame frame;

                lua_geti(L, -1, i); // push value

                lua_getfield(L, -1, "rect"); // push rect

                frame.rect = checkintrect(L, -1);

                lua_pop(L, 1); // pop rect

                lua_getfield(L, -1, "offset"); // push offset

                if (!lua_isnil(L, -1)) {
                    frame.offset = checkvector<float>(L, -1);
                }

                lua_pop(L, 1); // pop offset

                lua_getfield(L, -1, "rotation"); // push rotation

                if (!lua_isnil(L, -1)) {
                    frame.rotation = luaL_checknumber(L, -1);
                }

                lua_pop(L, 1); // pop rotation

                lua_getfield(L, -1, "rotation_center"); // push rotation_center

                if (!lua_isnil(L, -1)) {
                    frame.rotation_center = checkvector<float>(L, -1);
                }

                lua_pop(L, 1); // pop rotation center

                lua_getfield(L, -1, "scale"); // push scale

                if (!lua_isnil(L, -1)) {
                    frame.scale = checkvector<float>(L, -1);
                }

                lua_pop(L, 1); // pop scale

                lua_getfield(L, -1, "scale_center"); // push scale center

                if (!lua_isnil(L, -1)) {
                    frame.scale_center = checkvector<float>(L, -1);
                }

                lua_pop(L, 1); // pop scale center

                lua_pop(L, 1); // pop value

                anim.frames.push_back(frame);
            }

            lua_pop(L, 1); // pop frames table

            return anim;
        }

    } // namespace LuaUtil

} // namespace Terrarium
