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

#include "lua_particles.hpp"
#include "lua_entity.hpp"
#include "lua_interface.hpp"
#include "../common/lua_util.hpp"

namespace Terrarium {

    namespace LuaParticleAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_add_particle_spawner", add_particle_spawner);
            lua_interface.registerFunction("_stop_particle_spawner", add_particle_spawner);
        }

        int add_particle_spawner(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            LuaUtil::FieldChecker checker(L, "ParticleSpawnerDef", 1);

            ParticleSpawnerParams params;

            try {
                params = checkparticlespawnerdef(L, *lua_interface->game, checker);
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

            sf::Vector2f position;
            std::weak_ptr<Entity> source;

            if (lua_istable(L, 2)) {
                position = LuaUtil::checkvector<float>(L, 2);
            } else {
                auto entity_ref = reinterpret_cast<LuaEntityAPI::LuaEntityUD*>(LuaUtil::checksubclass(L, 2, LuaEntityAPI::LUA_ENTITYREF));
                source = entity_ref->getPtr();
            }

            auto spawner = std::shared_ptr<ParticleSpawner>(new ParticleSpawner(params, position, source));

            lua_pushinteger(L, lua_interface->game->particles.add(spawner));

            return 1;
        }

        int stop_particle_spawner(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            particlespawnerid id = luaL_checkinteger(L, 1);

            auto spawner = lua_interface->game->particles.get(id);

            if (!spawner) {
                if (lua_toboolean(L, 2)) {
                    return 0;
                } else {
                    return luaL_error(L, "Invalid particle spawner id");
                }
            }

            spawner->stop();

            return 0;
        }

        ParticleSpawnerParams checkparticlespawnerdef(lua_State *L, GameState &game, LuaUtil::FieldChecker &checker) {
            ParticleSpawnerParams params;

            params.time = checker.checknumber("time");
            params.amount = checker.checkunsigned("amount");

            checker.rfield("physics", [&] () {
                LuaUtil::FieldChecker physics_checker(L, "PhysicsParams", -1);
                params.physics = LuaEntityAPI::checkphysicsparams(physics_checker);
            });

            if (checker.havefield("texture")) {
                params.texture = &game.gfx.textures.get(checker.checkstring("texture"));
            }

            if (checker.havefield("texture_rect")) {
                params.texture_rect = checker.checkrect<float>("texture_rect");
            }

            if (checker.havefield("color")) {
                params.color = checker.checkcolor("color");
            }

            if (checker.havefield("minoffset")) {
                params.minoffset = checker.checkvector<float>("minoffset");
            }

            if (checker.havefield("maxoffset")) {
                params.maxoffset = checker.checkvector<float>("maxoffset");
            }

            if (checker.havefield("minspeed")) {
                params.minspeed = checker.checkvector<float>("minspeed");
            }

            if (checker.havefield("maxspeed")) {
                params.maxspeed = checker.checkvector<float>("maxspeed");
            }

            if (checker.havefield("minlifetime")) {
                params.minlifetime = checker.checknumber("minlifetime");
            }

            if (checker.havefield("maxlifetime")) {
                params.maxlifetime = checker.checknumber("maxlifetime");
            }

            if (checker.getfieldtype("accel") == LUA_TTABLE) {
                params.accel = checker.checkvector<float>("accel");
            } else if (checker.getfieldtype("accel") == LUA_TNUMBER) {
                params.accel = float(checker.checknumber("accel"));
            }

            if (checker.havefield("speed_mult")) {
                params.speed_mult = checker.checknumber("speed_mult");
            }

            return params;
        }

    }
}
