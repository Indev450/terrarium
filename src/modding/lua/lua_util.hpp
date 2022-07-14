#ifndef MODDING_LUA_UTIL_HPP
#define MODDING_LUA_UTIL_HPP

#include <iostream>
#include <string>

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
