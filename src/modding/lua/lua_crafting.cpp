#include "lua_crafting.hpp"
#include "lua_interface.hpp"
#include "lua_item.hpp"


namespace Terrarium {

    namespace LuaCraftingAPI {

        void init(LuaModdingInterface &lua_interface) {
            lua_interface.registerFunction("_add_crafting_category", add_crafting_category);
            lua_interface.registerFunction("_set_default_crafting_category", set_default_crafting_category);
            lua_interface.registerFunction("_register_recipe", register_recipe);
            lua_interface.registerFunction("_show_crafting_ui", show_crafting_ui);
        }

        int add_crafting_category(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            lua_interface->game->crafts.addCategory(luaL_checkstring(L, 1));

            return 0;
        }

        int set_default_crafting_category(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            lua_interface->game->crafts.default_category = luaL_checkstring(L, 1);

            return 0;
        }

        int register_recipe(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            const char *category = luaL_checkstring(L, 1);
            LuaItemAPI::LuaItemStackUD *istack_udata = nullptr;

            if (!lua_istable(L, 2)) {
                return luaL_error(L, "expected table as recipe");
            }

            std::unique_ptr<Recipe> recipe = std::make_unique<Recipe>();

            lua_getfield(L, 2, "result");
            istack_udata = reinterpret_cast<LuaItemAPI::LuaItemStackUD*>(luaL_checkudata(L, -1, LUA_ITEMSTACK));
            recipe->result = *(istack_udata->istack);
            lua_pop(L, 1);

            lua_getfield(L, 2, "requirements");

            if (!lua_istable(L, -1)) {
                return luaL_error(L, "expected table as recipe requirements");
            }

            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                istack_udata = reinterpret_cast<LuaItemAPI::LuaItemStackUD*>(luaL_checkudata(L, -1, LUA_ITEMSTACK));

                recipe->requirements.emplace_back();
                recipe->requirements.back() = *istack_udata->istack;

                lua_pop(L, 1);
            }

            lua_pop(L, 1);

            lua_interface->game->crafts.addRecipe(category, std::move(recipe));

            return 0;
        }

        int show_crafting_ui(lua_State *L) {
            LuaModdingInterface *lua_interface = reinterpret_cast<LuaModdingInterface*>(lua_touserdata(L, lua_upvalueindex(1)));

            lua_interface->game->player->crafting_category = luaL_checkstring(L, 1);

            lua_interface->game->hud.setVisible("inventory", true);
            lua_interface->game->hud.setVisible("craft", true);

            return 0;
        }

    }

}
