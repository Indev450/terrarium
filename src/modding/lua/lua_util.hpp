#ifndef MODDING_LUA_UTIL_HPP
#define MODDING_LUA_UTIL_HPP

#include <iostream>

#include <lua.hpp>

namespace Terrarium {

    namespace LuaUtil {

        // Calls userdata class destructor. Use as __gc metatable function
        template <class T>
        static int call_destructor(lua_State *L) {
            void *obj_mem = lua_touserdata(L, 1);

            T *obj = reinterpret_cast<T*>(obj_mem);

            obj->~T();

            return 0;
        }

        // Print error message occured in pcall and pop it
        static inline void printerr(lua_State *L) {
            const char *msg = lua_tostring(L, -1);
            std::cerr<<msg<<std::endl;
            lua_pop(L, 1);
        }

        static bool run_script(lua_State *L, const char *path) {
            if (luaL_dofile(L, path) != LUA_OK) {
                printerr(L);
                return false;
            }

            return true;
        }

    }

}

#endif
