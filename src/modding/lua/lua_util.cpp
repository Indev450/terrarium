#include "lua_util.hpp"

namespace Terrarium {

    namespace LuaUtil {

        void printerr(lua_State *L) {
            const char *msg = lua_tostring(L, -1);
            std::cerr<<msg<<std::endl;
            lua_pop(L, 1);
        }

        bool run_script(lua_State *L, const char *path) {
            if (luaL_dofile(L, path) != LUA_OK) {
                printerr(L);
                return false;
            }

            return true;
        }

        void push_vector2f(lua_State *L, const sf::Vector2f &vec) {
            lua_newtable(L);

            lua_pushnumber(L, vec.x);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, vec.y);
            lua_setfield(L, -2, "y");
        }

    } // namespace LuaUtil

} // namespace Terrarium
