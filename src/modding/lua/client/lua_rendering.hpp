/*
 * Copyright 2022 Inner Devil
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

#ifndef MODDING_LUA_CLIENT_RENDERING_HPP
#define MODDING_LUA_CLIENT_RENDERING_HPP

#include <string>

#include <lua.hpp>

namespace Terrarium {

    class LuaClientModdingInterface;

    namespace LuaClientRenderingAPI {

        static const std::string LUA_SPRITE = "Sprite";
        static const std::string LUA_TEXT = "Text";

        void init(LuaClientModdingInterface &lua_interface);

        // void core._draw(RenderTarget *target, Drawable drawable)
        int draw(lua_State *L);

        // void core._clear(RenderTarget *target, Color color)
        int clear(lua_State *L);

        // Sprite core._sprite_new([string texture[, IntRect rect]])
        int sprite_new(lua_State *L);

        // void Sprite:set_texture(string texture[, bool reset_rect = false])
        int sprite_set_texture(lua_State *L);

        // void Sprite:set_texture_rect(IntRect rect)
        int sprite_set_texture_rect(lua_State *L);

        // void Sprite:set_color(Color color)
        int sprite_set_color(lua_State *L);

        // void Sprite:set_position(Vector2f position)
        int sprite_set_position(lua_State *L);

        // void Sprite:set_scale(Vector2f scale)
        int sprite_set_scale(lua_State *L);

        // Text core._text_new(string str, unsigned size[, Color color])
        int text_new(lua_State *L);

        // void Text:set_string(string str)
        int text_set_string(lua_State *L);

        // void Text:set_size(unsigned size)
        int text_set_size(lua_State *L);

        // void Text:set_color(Color color)
        int text_set_color(lua_State *L);

        // void Text:set_position(Vector2f position)
        int text_set_position(lua_State *L);

    }

}

#endif
