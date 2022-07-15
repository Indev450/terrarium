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

        Animation checkanimation(lua_State *L, int idx) {
            Animation anim;

            if (!lua_istable(L, idx)) {
                luaL_error(L, "function argument #%d expected to be table", idx);
            }

            idx = lua_absindex(L, idx);

            lua_getfield(L, idx, "time_per_frame");
            anim.time_per_frame = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, idx, "frames");

            if (!lua_istable(L, -1)) {
                luaL_error(L, "<animation>.frames expected to be table");
            }

            lua_pushnil(L); // push first key
            while (lua_next(L, -2) != 0) { // pop key, push key, value

                anim.frames.push_back(checkintrect(L, -1));

                lua_pop(L, 1); // pop value
            }

            lua_pop(L, 1); // pop frames table

            return anim;
        }

    } // namespace LuaUtil

} // namespace Terrarium
