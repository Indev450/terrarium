#ifndef MODDING_LUA_INTERFACE_HPP
#define MODDING_LUA_INTERFACE_HPP

#include <lua.hpp>

#include "../interface.hpp"

namespace Terrarium {

    class LuaModdingInterface: public ModdingInterface {
        lua_State *L = nullptr;

    public:
        LuaModdingInterface(std::shared_ptr<GameState> game);

        void update(float dtime) override;
        void handleEvent(Event &event) override;

        // Adds a function into core module in lua. Function is pushed as C closure,
        // with 1 upvalue - light userdata, which is `this` pointer
        void registerFunction(const char *name, lua_CFunction fn);

        inline lua_State *getLuaState() { return L; }
    };

} // namespace Terrarium

#endif
