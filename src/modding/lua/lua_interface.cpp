#include <stdexcept>

#include "lua_interface.hpp"
#include "lua_util.hpp"
#include "lua_entity.hpp"

namespace Terrarium {

    LuaModdingInterface::LuaModdingInterface(std::shared_ptr<GameState> game):
        ModdingInterface(game), L(luaL_newstate())
    {
        luaL_openlibs(L);

        lua_newtable(L);
        lua_setglobal(L, "core");

        LuaEntityAPI::init(*this);

        if (!LuaUtil::run_script(L, "assets/init.lua")) {
            throw std::runtime_error("could not execute assets/init.lua");
        }
    }

    void LuaModdingInterface::update(float dtime) {
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_update");

        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L, dtime);

            // Pops _update and dtime
            if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
                LuaUtil::printerr(L);
            }
        } else {
            lua_pop(L, 1); // pops _update
        }

        lua_pop(L, 1); // pop 'core'
    }

    void LuaModdingInterface::handleEvent(Event &event) {
        // TODO - send event into core._handle_event()
    }

    void LuaModdingInterface::registerFunction(const char *name, lua_CFunction fn) {
        lua_getglobal(L, "core");

        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, fn, 1);
        lua_setfield(L, -2, name);

        lua_pop(L, 1); // pop core
    }

} // namespace Terrarium
