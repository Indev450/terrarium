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

#include <SFML/Graphics.hpp>

#include "lua_rendering.hpp"
#include "lua_client_interface.hpp"
#include "../server/lua_util.hpp"
#include "../../../game.hpp"

namespace Terrarium {

    namespace LuaClientRenderingAPI {

        void init(LuaClientModdingInterface &lua_interface) {
            lua_interface.registerFunction("_draw", draw);
            lua_interface.registerFunction("_clear", clear);
            lua_interface.registerFunction("_sprite_new", sprite_new);
            lua_interface.registerFunction("_text_new", text_new);

            lua_State *L = lua_interface.getLuaState();

            luaL_newmetatable(L, LUA_SPRITE.c_str()); // push metatable

            lua_newtable(L); // push __index table

            lua_interface.pushClosure(sprite_set_texture);
            lua_setfield(L, -2, "set_texture");

            lua_pushcfunction(L, sprite_set_texture_rect);
            lua_setfield(L, -2, "set_texture_rect");

            lua_pushcfunction(L, sprite_set_color);
            lua_setfield(L, -2, "set_color");

            lua_pushcfunction(L, sprite_set_position);
            lua_setfield(L, -2, "set_position");

            lua_pushcfunction(L, sprite_set_scale);
            lua_setfield(L, -2, "set_scale");

            lua_setfield(L, -2, "__index"); // pop __index

            // Add destructor
            lua_pushcfunction(L, LuaUtil::call_destructor<sf::Sprite>);
            lua_setfield(L, -2, "__gc");

            lua_pop(L, 1); // pop metatable


            luaL_newmetatable(L, LUA_TEXT.c_str()); // push metatable

            lua_newtable(L); // push __index table

            lua_interface.pushClosure(text_set_string);
            lua_setfield(L, -2, "set_string");

            lua_interface.pushClosure(text_set_size);
            lua_setfield(L, -2, "set_size");

            lua_interface.pushClosure(text_set_color);
            lua_setfield(L, -2, "set_color");

            lua_pushcfunction(L, text_set_position);
            lua_setfield(L, -2, "set_position");

            lua_setfield(L, -2, "__index"); // pop __index

            // Add destructor
            lua_pushcfunction(L, LuaUtil::call_destructor<sf::Text>);
            lua_setfield(L, -2, "__gc");

            lua_pop(L, 1); // pop metatable
        }

        int draw(lua_State *L) {
            if (!lua_islightuserdata(L, 1)) {
                return luaL_error(L, "argument #1 expected to be render target");
            }

            sf::RenderTarget *target = reinterpret_cast<sf::RenderTarget*>(lua_touserdata(L, 1));

            sf::Sprite *sprite = reinterpret_cast<sf::Sprite*>(LuaUtil::trysubclass(L, 2, LUA_SPRITE));
            sf::Text *text = reinterpret_cast<sf::Text*>(LuaUtil::trysubclass(L, 2, LUA_TEXT));

            if (sprite) {
                target->draw(*sprite);
            } else if (text) {
                target->draw(*text);
            } else {
                return luaL_error(L, "Unknown drawable");
            }

            // Maybe more drawable things later

            return 0;
        }

        int clear(lua_State *L) {
            if (!lua_islightuserdata(L, 1)) {
                return luaL_error(L, "argument #1 expected to be render target");
            }

            sf::RenderTarget *target = reinterpret_cast<sf::RenderTarget*>(lua_touserdata(L, 1));

            sf::Color color = LuaUtil::checkcolor(L, 2);

            target->clear(color);

            return 0;
        }

        int sprite_new(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Gfx &gfx = lua_interface->game->gfx;

            void *sprite_mem = lua_newuserdatauv(L, sizeof(sf::Sprite), 0);

            // There is a texture argument!
            if (lua_gettop(L) > 0 && !lua_isnil(L, 1)) {
                const char *texture = luaL_checkstring(L, 1);

                // And texture_rect argument!
                if (lua_gettop(L) > 1 && !lua_isnil(L, 2)) {
                    sf::IntRect texture_rect = LuaUtil::checkintrect(L, 2);

                    // Init with texture and rect
                    new (sprite_mem) sf::Sprite(gfx.textures.get(texture), texture_rect);
                } else {
                    // Init with texture
                    new (sprite_mem) sf::Sprite(gfx.textures.get(texture));
                }
            } else {
                // Init without anything
                new (sprite_mem) sf::Sprite();
            }

            luaL_setmetatable(L, LUA_SPRITE.c_str());

            return 1;
        }

        int sprite_set_texture(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Gfx &gfx = lua_interface->game->gfx;

            sf::Sprite *sprite = reinterpret_cast<sf::Sprite*>(LuaUtil::checksubclass(L, 1, LUA_SPRITE));

            sprite->setTexture(gfx.textures.get(luaL_checkstring(L, 2)), lua_toboolean(L, 3));

            return 0;
        }

        int sprite_set_texture_rect(lua_State *L) {
            sf::Sprite *sprite = reinterpret_cast<sf::Sprite*>(LuaUtil::checksubclass(L, 1, LUA_SPRITE));

            sf::IntRect rect = LuaUtil::checkintrect(L, 2);

            sprite->setTextureRect(rect);

            return 0;
        }

        int sprite_set_color(lua_State *L) {
            sf::Sprite *sprite = reinterpret_cast<sf::Sprite*>(LuaUtil::checksubclass(L, 1, LUA_SPRITE));

            sf::Color color = LuaUtil::checkcolor(L, 2);

            sprite->setColor(color);

            return 0;
        }

        int sprite_set_position(lua_State *L) {
            sf::Sprite *sprite = reinterpret_cast<sf::Sprite*>(LuaUtil::checksubclass(L, 1, LUA_SPRITE));

            sf::Vector2f position = LuaUtil::checkvector<float>(L, 2);

            sprite->setPosition(position);

            return 0;
        }

        int sprite_set_scale(lua_State *L) {
            sf::Sprite *sprite = reinterpret_cast<sf::Sprite*>(LuaUtil::checksubclass(L, 1, LUA_SPRITE));

            sf::Vector2f scale = LuaUtil::checkvector<float>(L, 2);

            sprite->setScale(scale);

            return 0;
        }

        int text_new(lua_State *L) {
            LuaClientModdingInterface *lua_interface = reinterpret_cast<LuaClientModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            Gfx &gfx = lua_interface->game->gfx;

            void *text_mem = lua_newuserdatauv(L, sizeof(sf::Text), 0);

            new (text_mem) sf::Text(luaL_checkstring(L, 1), gfx.font, LuaUtil::checkinteger_ranged<unsigned>(L, 2));

            if (lua_gettop(L) > 2 && !lua_isnil(L, 3)) {
                sf::Text *text = reinterpret_cast<sf::Text*>(text_mem);

                text->setFillColor(LuaUtil::checkcolor(L, 3));
            }

            luaL_setmetatable(L, LUA_TEXT.c_str());

            return 1;
        }

        int text_set_string(lua_State *L) {
            sf::Text *text = reinterpret_cast<sf::Text*>(LuaUtil::checksubclass(L, 1, LUA_TEXT));

            text->setString(luaL_checkstring(L, 2));

            return 0;
        }

        int text_set_size(lua_State *L) {
            sf::Text *text = reinterpret_cast<sf::Text*>(LuaUtil::checksubclass(L, 1, LUA_TEXT));

            text->setCharacterSize(LuaUtil::checkinteger_ranged<unsigned>(L, 2));

            return 0;
        }

        int text_set_color(lua_State *L) {
            sf::Text *text = reinterpret_cast<sf::Text*>(LuaUtil::checksubclass(L, 1, LUA_TEXT));

            text->setFillColor(LuaUtil::checkcolor(L, 2));

            return 0;
        }

        int text_set_position(lua_State *L) {
            sf::Text *text = reinterpret_cast<sf::Text*>(LuaUtil::checksubclass(L, 1, LUA_TEXT));

            text->setPosition(LuaUtil::checkvector<float>(L, 2));

            return 0;
        }

    }

}
