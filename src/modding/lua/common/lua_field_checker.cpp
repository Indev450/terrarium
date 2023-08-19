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

#include <cstring>
#include <stdexcept>
#include <functional>

#include "lua_field_checker.hpp"
#include "lua_util.hpp"

namespace Terrarium {

    namespace LuaUtil {

        FieldChecker::FieldChecker(lua_State *_L, const char *_structname, int _idx):
            L(_L), structname(_structname), idx(lua_absindex(L, _idx))
        {
            checkself();
        }

        void FieldChecker::checkself() {
            if (!lua_istable(L, idx)) {
                char errormsg[64];
                snprintf(errormsg, 64, "expected table for %s, got %s", structname, luaL_typename(L, idx));
                throw std::invalid_argument(errormsg);
            }
        }

        void FieldChecker::fielderror(const char *field, const char *extramsg) {
            // Uh i think thats enough in case we will get really long error
            // messages.
            char errormsg[1024];
            snprintf(errormsg, 512, "bad field '%s' in %s (%s)", field, structname, extramsg);

            throw std::invalid_argument(errormsg);
        }

        void FieldChecker::rfielderror(const char *field, const char *extramsg) {
            // Uh i think thats enough in case we will get really long error
            // messages.
            char errormsg[1024];
            snprintf(errormsg, 512, "%s\n\tIn field '%s' of %s", extramsg, field, structname);

            throw std::invalid_argument(errormsg);
        }

        void FieldChecker::fieldcheck(const char *field, int cond, const char *extramsg) {
            if (!cond) fielderror(field, extramsg);
        }

        void FieldChecker::typeerror(const char *field, const char *tname, const char *tname2) {
            char extramsg[128];
            snprintf(extramsg, 128, "%s expected, got %s", tname, tname2);

            fielderror(field, extramsg);
        }

        void FieldChecker::typeerror(const char *field, int expected, int got) {
            typeerror(field, lua_typename(L, expected), lua_typename(L, got));
        }

        void FieldChecker::typecheck(const char *field, int expected, int got) {
            if (expected != got) typeerror(field, expected, got);
        }

        int FieldChecker::getfieldtype(const char *field) {
            int got = lua_getfield(L, idx, field);

            lua_pop(L, 1);

            return got;
        }

        // If field matches type, push it, otherwise pop it and throw exception
        void FieldChecker::getfield(const char *field, int expected) {
            int got = lua_getfield(L, idx, field);

            if (expected != LUA_TNONE && got != expected) {
                lua_pop(L, 1);
                typeerror(field, expected, got);
            }
        }

        bool FieldChecker::havefield(const char *field) {
            bool have = lua_getfield(L, idx, field) != LUA_TNIL;

            lua_pop(L, 1);

            return have;
        }

        void FieldChecker::rfield(const char *field, std::function<void()> func, bool optional) {
            if (optional and !havefield(field)) return;

            getfield(field, LUA_TTABLE);

            try {
                func();
            } catch (const std::invalid_argument &e) {
                lua_pop(L, 1);
                rfielderror(field, e.what());
            }

            lua_pop(L, 1);
        }

        size_t FieldChecker::checkoption(const char *field, const char *def, const char *lst[]) {
            const char *str = def;

            if (havefield(field)) str = checkstring(field);

            if (str == nullptr) {
                typeerror(field, LUA_TSTRING, LUA_TNIL);
            }

            for (size_t i = 0; lst[i] != nullptr; ++i) {
                if (strcmp(str, lst[i]) == 0) {
                    return i;
                }
            }

            char extramsg[40];
            snprintf(extramsg, 40, "invalid option '%.22s'", str);

            fielderror(field, extramsg);

            // Shouldn't reach this
            return 0;
        }

        bool FieldChecker::checkboolean(const char *field) {
            getfield(field, LUA_TBOOLEAN);

            bool result = lua_toboolean(L, -1) != 0;

            lua_pop(L, 1);

            return result;
        }

        lua_Number FieldChecker::checknumber(const char *field) {
            getfield(field, LUA_TNUMBER);

            lua_Number num = lua_tonumber(L, -1);

            lua_pop(L, 1);

            return num;
        }

        lua_Number FieldChecker::checknumber_range(const char *field, lua_Number min, lua_Number max) {
            lua_Number num = checknumber(field);

            if (num < min || num > max) {
                char extramsg[64];
                snprintf(extramsg, 64, "should be in range from %.5lf to %.5lf", min, max);
                fielderror(field, extramsg);
            }

            return num;
        }

        lua_Integer FieldChecker::checkinteger(const char *field) {
            getfield(field, LUA_TNUMBER);

            if (!lua_isinteger(L, -1)) {
                lua_pop(L, 1);
                fielderror(field, "number has no integer representation");
            }

            lua_Integer num = lua_tointeger(L, -1);

            lua_pop(L, 1);

            return num;
        }

        lua_Unsigned FieldChecker::checkunsigned(const char *field) {
            lua_Integer num = checkinteger(field);

            if (num < 0) {
                fielderror(field, "expected unsigned number");
            }

            return static_cast<lua_Unsigned>(num);
        }

        const char *FieldChecker::checkstring(const char *field) {
            getfield(field, LUA_TSTRING);

            const char *str = lua_tostring(L, -1);

            lua_pop(L, 1);

            return str;
        }

        void *FieldChecker::checkudata(const char *field, const char *tname) {
            getfield(field, LUA_TUSERDATA);

            void *udata = luaL_testudata(L, -1, tname);

            if (udata == nullptr) {
                const char *gotname = luaL_typename(L, -1);
                lua_pop(L, 1);
                typeerror(field, tname, gotname);
            }

            return udata;
        }

        void *FieldChecker::checksubclass(const char *field, const std::string &base) {
            getfield(field, LUA_TUSERDATA);

            if (lua_getmetatable(L, -1)) { // If object has metatable, push it
                lua_getfield(L, -1, "__name"); // push __name
                std::string name = lua_tostring(L, -1);
                lua_pop(L, 2); // pop __name and metatable

                // If `__name` starts with `base`
                if (name.rfind(base, 0) == 0) {
                    lua_pop(L, 1); // Pop field
                    return lua_touserdata(L, -1);
                }
            }

            const char *got = luaL_typename(L, -1);

            lua_pop(L, 1); // Pop field (again)

            typeerror(field, (base + " subclass").c_str(), got);

            // Should never reach that
            return nullptr;
        }

        template <>
        sf::Vector2f FieldChecker::checkvector(const char *field) {
            TopKeeper top_keeper(L);

            getfield(field, LUA_TTABLE);

            FieldChecker checker(L, "Vector2f", -1);

            sf::Vector2f result;

            try {
                result.x = checker.checknumber("x");
                result.y = checker.checknumber("y");
            } catch (const std::invalid_argument &e) {
                rfielderror(field, e.what());
            }

            lua_pop(L, 1);

            return result;
        }

        template <>
        sf::Vector2i FieldChecker::checkvector(const char *field) {
            TopKeeper top_keeper(L);

            getfield(field, LUA_TTABLE);

            FieldChecker checker(L, "Vector2i", -1);

            sf::Vector2i result;

            try {
                result.x = checker.checkinteger("x");
                result.y = checker.checkinteger("y");
            } catch (const std::invalid_argument &e) {
                rfielderror(field, e.what());
            }

            lua_pop(L, 1);

            return result;
        }

        template <>
        sf::Vector2<uint8_t> FieldChecker::checkvector(const char *field) {
            TopKeeper top_keeper(L);

            getfield(field, LUA_TTABLE);

            FieldChecker checker(L, "Vector2<uint8_t>", -1);

            sf::Vector2<uint8_t> result;

            try {
                result.x = checker.checkinteger("x");
                result.y = checker.checkinteger("y");
            } catch (const std::invalid_argument &e) {
                rfielderror(field, e.what());
            }

            lua_pop(L, 1);

            return result;
        }

        template <>
        sf::FloatRect FieldChecker::checkrect(const char *field) {
            TopKeeper top_keeper(L);

            getfield(field, LUA_TTABLE);

            FieldChecker checker(L, "FloatRect", -1);

            sf::FloatRect result;

            try {
                result.left = checker.checknumber("x");
                result.top = checker.checknumber("y");
                result.width = checker.checknumber("width");
                result.height = checker.checknumber("height");
            } catch (const std::invalid_argument &e) {
                rfielderror(field, e.what());
            }

            lua_pop(L, 1);

            return result;
        }

        template <>
        sf::IntRect FieldChecker::checkrect(const char *field) {
            TopKeeper top_keeper(L);

            getfield(field, LUA_TTABLE);

            FieldChecker checker(L, "IntRect", -1);

            sf::IntRect result;

            try {
                result.left = checker.checkinteger("x");
                result.top = checker.checkinteger("y");
                result.width = checker.checkinteger("width");
                result.height = checker.checkinteger("height");
            } catch (const std::invalid_argument &e) {
                rfielderror(field, e.what());
            }

            lua_pop(L, 1);

            return result;
        }

        sf::Color FieldChecker::checkcolor(const char *field) {
            TopKeeper top_keeper(L);

            getfield(field, LUA_TTABLE);

            FieldChecker checker(L, "Color", -1);

            sf::Color result;

            try {
                result.r = checker.checkunsigned("r");
                result.g = checker.checkunsigned("g");
                result.b = checker.checkunsigned("b");

                if (checker.havefield("a")) {
                    result.a = checker.checkinteger("a");
                }
            } catch (const std::invalid_argument &e) {
                rfielderror(field, e.what());
            }

            lua_pop(L, 1);

            return result;
        }

        Animation FieldChecker::checkanimation(const char *field) {
            TopKeeper top_keeper(L);

            getfield(field, LUA_TTABLE);

            Animation anim;

            FieldChecker checker(L, "Animation", -1);

            try {
                anim.time_per_frame = checker.checknumber("time_per_frame");

                if (checker.havefield("next"))
                    anim.next = checker.checkstring("next");

                static const char *easing_type_names[] = {
                    "None",
                    "Linear",
                    "InQuad",
                    "OutQuad",
                    "InOutQuad",
                    "InCubic",
                    "OutCubic",
                    "InOutCubic",
                    "InExpo",
                    "OutExpo",
                    "InOutExpo",
                    nullptr,
                };

                static const Animation::Easing easing_type_values[] = {
                    Animation::Easing::None,
                    Animation::Easing::Linear,
                    Animation::Easing::InQuad,
                    Animation::Easing::OutQuad,
                    Animation::Easing::InOutQuad,
                    Animation::Easing::InCubic,
                    Animation::Easing::OutCubic,
                    Animation::Easing::InOutCubic,
                    Animation::Easing::InExpo,
                    Animation::Easing::OutExpo,
                    Animation::Easing::InOutExpo,
                };

                anim.easing = easing_type_values[checker.checkoption("easing", "None", easing_type_names)];

                checker.typecheck("frames", LUA_TTABLE, lua_getfield(L, checker.idx, "frames")); // Push frames table

                int amount_frames = luaL_len(L, -1);
                for (int i = 1; i <= amount_frames; ++i) {
                    AnimationFrame frame;

                    lua_geti(L, -1, i); // push value

                    FieldChecker frame_checker(L, "AnimationFrame", -1);

                    try {
                        frame.rect = frame_checker.checkrect<int>("rect");

                        if (frame_checker.havefield("offset")) {
                            frame.offset = frame_checker.checkvector<float>("offset");
                        }

                        if (frame_checker.havefield("rotation")) {
                            frame.rotation = frame_checker.checknumber("rotation") / M_PI * 180;
                        }

                        if (frame_checker.havefield("rotation_center")) {
                            frame.rotation_center = frame_checker.checkvector<float>("rotation_center");
                        }

                        if (frame_checker.havefield("scale")) {
                            frame.scale = frame_checker.checkvector<float>("scale");
                        }

                        if (frame_checker.havefield("scale_center")) {
                            frame.scale_center = frame_checker.checkvector<float>("scale_center");
                        }
                    } catch (const std::invalid_argument &e) {
                        char arrpos[20];
                        snprintf(arrpos, 20, "frames[%d]", i);
                        checker.rfielderror(arrpos, e.what());
                    }

                    lua_pop(L, 1); // pop value

                    anim.frames.push_back(frame);
                }

                lua_pop(L, 1); // pop frames table
            } catch (const std::invalid_argument &e) {
                rfielderror(field, e.what());
            }

            lua_pop(L, 1);

            return anim;
        }

    }

}
