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

#ifndef MODDING_LUA_PARTICLES_HPP
#define MODDING_LUA_PARTICLES_HPP

#include <lua.hpp>

#include "../../../graphics/particles.hpp"
#include "../common/lua_field_checker.hpp"

namespace Terrarium {

    class LuaModdingInterface;
    class GameState;

    namespace LuaParticleAPI {

        void init(LuaModdingInterface &lua_interface);

        // int core._add_particle_spawner(ParticleSpawnerDef particle_spawner_def, Vector2f/Entity position)
        int add_particle_spawner(lua_State *L);

        // void core._stop_particle_spawner(int id[, bool noerror])
        int stop_particle_spawner(lua_State *L);

        // Functions called from c++
        ParticleSpawnerParams checkparticlespawnerdef(lua_State *L, GameState &game, LuaUtil::FieldChecker &checker);

    }

}

#endif
