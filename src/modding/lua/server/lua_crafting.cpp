#include "lua_crafting.hpp"
#include "lua_interface.hpp"
#include "lua_item.hpp"
#include "../common/lua_field_checker.hpp"

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

            luaL_checktype(L, 2, LUA_TTABLE);

            LuaUtil::FieldChecker checker(L, "Recipe", 2);

            std::unique_ptr<Recipe> recipe = std::make_unique<Recipe>();

            try {
                istack_udata = reinterpret_cast<LuaItemAPI::LuaItemStackUD*>(checker.checkudata("result", LUA_ITEMSTACK));
                recipe->result = *(istack_udata->istack);

                checker.getfield("requirements", LUA_TTABLE);

                LuaUtil::FieldChecker reqs_checker(L, "RecipeReqsList", -1);

                try {
                    lua_pushnil(L);
                    while (lua_next(L, -2) != 0) {
                        istack_udata = reinterpret_cast<LuaItemAPI::LuaItemStackUD*>(luaL_testudata(L, -1, LUA_ITEMSTACK));

                        if (istack_udata == nullptr) {
                            reqs_checker.typeerror(lua_tostring(L, -2), LUA_ITEMSTACK, luaL_typename(L, -1));
                        }

                        recipe->requirements.emplace_back();
                        recipe->requirements.back() = *istack_udata->istack;

                        lua_pop(L, 1);
                    }

                    lua_pop(L, 1);
                } catch (const std::invalid_argument &e) {
                    checker.rfielderror("requirements", e.what());
                }
            } catch (const std::invalid_argument &e) {
                luaL_error(L, e.what());
            }

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
