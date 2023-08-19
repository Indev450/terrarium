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
#include "../common/lua_util.hpp"
#include "../common/lua_field_checker.hpp"
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

            luaL_checktype(L, 1, LUA_TTABLE);

            LuaUtil::FieldChecker checker(L, "SoundSpec", 1);

            try {
                const char *name = checker.checkstring("name");

                float volume = checker.checknumber_range("volume", 0, 1) * 100;

                // TODO - maybe pitch also should be "ranged"?
                float pitch = checker.checknumber("pitch");

                int got = checker.getfieldtype("source");

                sound_handle handle;

                switch (got) {
                    case LUA_TNIL:
                        handle = lua_interface->game->sfx.playSound(name, volume, pitch);
                    break;

                    case LUA_TTABLE:
                        handle = lua_interface->game->sfx.playSoundAt(name, volume, pitch, checker.checkvector<float>("source"));
                    break;

                    case LUA_TUSERDATA:
                    {
                        LuaEntityAPI::LuaEntityUD *entity_ref = reinterpret_cast<LuaEntityAPI::LuaEntityUD*>(
                            checker.checksubclass("source", LuaEntityAPI::LUA_ENTITYREF));

                        handle = lua_interface->game->sfx.playSoundAt(name, volume, pitch, entity_ref->getPtr());
                    }
                    break;

                    default:
                        checker.typeerror("source", "nil, Vector2f or EntityRef", lua_typename(L, got));

                        // Should never reach that, putting it just so compiler
                        // doesn't complain about handle being uninitialized
                        return 0;
                    break;
                }

                lua_pushinteger(L, handle);
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

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
