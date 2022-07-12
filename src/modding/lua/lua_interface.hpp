#ifndef MODDING_LUA_INTERFACE_HPP
#define MODDING_LUA_INTERFACE_HPP

#include <lua.hpp>

#include "../interface.hpp"

namespace Terrarium {

    class LuaModdingInterface: public ModdingInterface {
        lua_State *L = nullptr;

        void pushItemEvent(ItemEvent &item_event);

    public:
        LuaModdingInterface(std::shared_ptr<GameState> game);

        void update(float dtime) override;
        void handleEvent(Event &event) override;

        // Makes closure with 1 upvalue - `this` pointer and leaves it on stack top
        void pushClosure(lua_CFunction fn);

        // Adds a function into core module in lua. Function is pushed as C closure,
        // with 1 upvalue - light userdata, which is `this` pointer
        void registerFunction(const char *name, lua_CFunction fn);

        inline lua_State *getLuaState() { return L; }
    };

} // namespace Terrarium

#endif
