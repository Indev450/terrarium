#ifndef MODDING_LUA_UTIL_HPP
#define MODDING_LUA_UTIL_HPP

#include <iostream>
#include <string>
#include <limits>

#include <SFML/System/Vector2.hpp>

#include <lua.hpp>

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

        // Print error message occured in pcall and pop it
        void printerr(lua_State *L);

        bool run_script(lua_State *L, const char *path);

        void push_vector2f(lua_State *L, const sf::Vector2f &vec);

    }

}

#endif
