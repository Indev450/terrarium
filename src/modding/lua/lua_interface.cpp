#include <stdexcept>

#include "lua_interface.hpp"
#include "lua_util.hpp"
#include "lua_entity.hpp"
#include "lua_item.hpp"
#include "lua_block.hpp"
#include "lua_player.hpp"

namespace Terrarium {

    LuaModdingInterface::LuaModdingInterface(std::shared_ptr<GameState> game):
        ModdingInterface(game), L(luaL_newstate())
    {
        luaL_openlibs(L);

        lua_newtable(L);
        lua_setglobal(L, "core");

        LuaEntityAPI::init(*this);
        LuaItemAPI::init(*this);
        LuaBlockAPI::init(*this);
        LuaPlayerAPI::init(*this);

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
        lua_getglobal(L, "core");

        lua_getfield(L, -1, "_on_event");

        if (lua_isfunction(L, -1)) {
            lua_newtable(L); // push event table

            lua_pushstring(L, event.getName());
            lua_setfield(L, -2, "name");

            switch (event.type) {
                case Event::ItemUseStart:
                case Event::ItemUseStop:
                case Event::ItemAltUseStart:
                case Event::ItemAltUseStop:
                case Event::ItemSelect:
                {
                    pushItemEvent(*event.item);
                    lua_setfield(L, -2, "item_event");
                }
                break;
            }

            // Pops _on_event and event table
            if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
                LuaUtil::printerr(L);
            }

        } else {
            lua_pop(L, 1); // pops _on_event
        }

        lua_pop(L, 1); // pops core
    }

    void LuaModdingInterface::pushClosure(lua_CFunction fn) {
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, fn, 1);
    }

    void LuaModdingInterface::registerFunction(const char *name, lua_CFunction fn) {
        lua_getglobal(L, "core");

        pushClosure(fn);
        lua_setfield(L, -2, name);

        lua_pop(L, 1); // pop core
    }

    void LuaModdingInterface::pushItemEvent(ItemEvent &item_event) {
        lua_newtable(L);

        // item_event.def always valid, so we able to skip null-checks
        LuaItemAPI::push_itemstack(L, item_event.item_stack);
        lua_setfield(L, -2, "item_stack");

        LuaEntityAPI::push_entity(L, item_event.user);
        lua_setfield(L, -2, "user");

        LuaUtil::push_vector2f(L, item_event.position);
        lua_setfield(L, -2, "position");
    }

} // namespace Terrarium
