#include "lua_util.hpp"

namespace Terrarium {

    namespace LuaUtil {

        lua_Number checknumber_ranged(lua_State *L, int idx, lua_Number min, lua_Number max) {
            lua_Number num = luaL_checknumber(L, idx);

            if (num < min || num > max) {
                luaL_error(L, "function argument #%d expected to be in range from %f to %f", idx, min, max);
            }

            return num;
        }

        void *checksubclass(lua_State *L, int idx, const std::string &base) {
            if (lua_getmetatable(L, idx)) { // If object has metatable, push it
                lua_getfield(L, -1, "__name"); // push __name
                std::string name = luaL_checkstring(L, -1);
                lua_pop(L, 2); // pop __name and metatable

                // If `__name` starts with `base`
                if (name.rfind(base, 0) == 0) {
                    return lua_touserdata(L, idx);
                } else {
                    luaL_error(L, "function argument #%d expected to be %s subclass", idx, base.c_str());
                }

            } else {
                luaL_error(L, "function argument #%d doesn't have metatable", idx);
            }

            // should never reach that
            return nullptr;
        }

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

    } // namespace LuaUtil

} // namespace Terrarium
