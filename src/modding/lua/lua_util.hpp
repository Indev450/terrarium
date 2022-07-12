#ifndef MODDING_LUA_UTIL_HPP
#define MODDING_LUA_UTIL_HPP

#include <iostream>

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

        // Print error message occured in pcall and pop it
        void printerr(lua_State *L);

        bool run_script(lua_State *L, const char *path);

        void push_vector2f(lua_State *L, const sf::Vector2f &vec);

    }

}

#endif
