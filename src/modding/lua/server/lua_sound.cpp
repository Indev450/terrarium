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

#include "lua_sound.hpp"
#include "lua_interface.hpp"
#include "lua_util.hpp"
#include "lua_entity.hpp"

namespace Terrarium {

    namespace LuaSoundAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_play_music", play_music);
            lua_interface.registerFunction("_stop_music", stop_music);
            lua_interface.registerFunction("_play_sound", play_sound);
            lua_interface.registerFunction("_stop_sound", stop_sound);
        }

        int play_music(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            const char *name = luaL_checkstring(L, 1);

            // Second argument, player name, is currently ignored
            lua_interface->game->sfx.playMusic(name);

            return 0;
        }

        int stop_music(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            lua_interface->game->sfx.stopMusic();

            return 0;
        }

        int play_sound(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            if (!lua_istable(L, 1)) {
                return luaL_error(L, "expected table as sound spec");
            }

            lua_getfield(L, 1, "name");
            const char *name = luaL_checkstring(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, 1, "volume");
            float volume = LuaUtil::checknumber_ranged(L, -1, 0, 1) * 100;
            lua_pop(L, 1);

            // TODO - maybe pitch also should be "ranged"?
            lua_getfield(L, 1, "pitch");
            float pitch = luaL_checknumber(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, 1, "source");

            sound_handle handle;

            if (lua_isnil(L, -1)) {
                handle = lua_interface->game->sfx.playSound(name, volume, pitch);

            } else if (lua_istable(L, -1)) {
                lua_getfield(L, -1, "x");
                float x = luaL_checknumber(L, -1);
                lua_pop(L, 1);

                lua_getfield(L, -1, "y");
                float y = luaL_checknumber(L, -1);
                lua_pop(L, 1);

                handle = lua_interface->game->sfx.playSoundAt(name, volume, pitch, sf::Vector2f(x, y));

            } else {
                LuaEntityAPI::LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityAPI::LuaEntityUD*>(
                    LuaUtil::checksubclass(L, 1, LuaEntityAPI::LUA_ENTITYREF));

                handle = lua_interface->game->sfx.playSoundAt(name, volume, pitch, entity_ref->getPtr());
            }

            lua_pop(L, 1);

            lua_pushinteger(L, handle);

            return 1;
        }

        int stop_sound(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            sound_handle handle = LuaUtil::checkinteger_ranged<sound_handle>(L, 1);

            lua_interface->game->sfx.stopSound(handle);

            return 0;
        }

    }

}
