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

#include <string>

#include <lua.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../../../graphics/animation.hpp"


namespace Terrarium {

    namespace LuaUtil {

        // Class that makes parsing lua tables easier.
        //
        // All check* functions throw std::invalid_argument exception if some of
        // values have incorrect type. Top level "parsing" code should handle it
        // and convert into lua error (via luaL_error(L, e.what()), for example)
        class FieldChecker {
            lua_State *L;
            const char *structname;
            int idx;

        public:
            // Constructs field checker. Stack value pointed by idx should be a
            // table. Automatically converts relative index to absolute if
            // necessary, so as long as table stays at same index, this object
            // can be treated as some kind of interface to that table.
            FieldChecker(lua_State *_L, const char *_structname, int _idx);

            // Checks if value at index 'idx' is actually a table
            void checkself();

            // Bunch of getters and setters, for convinience
            lua_State *getState() { return L; }

            const char *getStructName() { return structname; }

            int getIndex() { return idx; }

            int setIndex(int newidx) {
                idx = lua_absindex(L, newidx);
                checkself();
                return idx;
            }

            // Throws exception with formatted error message:
            // "bad field 'field' in structname (extramsg)"
            void fielderror(const char *field, const char *extramsg);

            // Throws exception with formatted error message (for recursive errors):
            // "extramsg"
            // "in field structname of field"
            void rfielderror(const char *field, const char *extramsg);

            // Throws exception if cond is false (0)
            void fieldcheck(const char *field, int cond, const char *extramsg);

            // Throws exception with formatted standard message:
            // "... expected, got ..."
            void typeerror(const char *field, const char *tname, const char *tname2);

            // Same as above, but the integers represent lua types (such as
            // LUA_TTABLE, LUA_TNIL, etc)
            void typeerror(const char *field, int expected, int got);

            // Checks types and throws exception if they don't match
            void typecheck(const char *field, int expected, int got);

            // Returns type for field
            int getfieldtype(const char *field);

            // Pushes field on top of stack. If expected type is specified (not
            // equals to LUA_TNONE), field type is checked and if it doesn't
            // match, exception thrown.
            void getfield(const char *field, int expected = LUA_TNONE);

            // Checks if field exists (i.e. its type isn't LUA_TNIL)
            bool havefield(const char *field);

            // For parsing field recursively. Pushes field on top of stack
            // (expecting it to be a table) and calls func.
            // If exception happens, calls rfielderror.
            // If optional is true and field is nil, func is not executed and
            // exception is not thrown.
            void rfield(const char *field, std::function<void()> func, bool optional = false);

            ////////////////////////////////////////////////////////////////////
            // Alternatives for luaL_check* functions
            ////////////////////////////////////////////////////////////////////

            size_t checkoption(const char *field, const char *def, const char *lst[]);

            bool checkboolean(const char *field);

            lua_Number checknumber(const char *field);

            // Also checks if number lies within specified range
            lua_Number checknumber_range(const char *field, lua_Number min, lua_Number max);

            lua_Integer checkinteger(const char *field);

            // Also checks if number is >= 0
            lua_Unsigned checkunsigned(const char *field);

            const char *checkstring(const char *field);

            void *checkudata(const char *field, const char *tname);

            ////////////////////////////////////////////////////////////////////
            // Engine-specific set of check* functions
            ////////////////////////////////////////////////////////////////////

            // Checks if userdata is a subclass of specified base class, i.e.
            // it's __name metatable field starts with substring base.
            // For example, if PlayerRef is subclass of EntityRef, it's name
            // would be EntityRef.PlayerRef
            void *checksubclass(const char *field, const std::string &base);

            template <class T>
            sf::Vector2<T> checkvector(const char *field);

            template <class T>
            sf::Rect<T> checkrect(const char *field);

            sf::Color checkcolor(const char *field);

            // TODO - maybe move this out into separate function, since it is a
            // bit too specific
            Animation checkanimation(const char *field);

        };

    }
}
