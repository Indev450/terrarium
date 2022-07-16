#ifndef MODDING_INTERFACE_HPP
#define MODDING_INTERFACE_HPP

#include <memory>

#include "../game.hpp"
#include "../mapgen/mapgen_base.hpp"

namespace Terrarium {

    class ModdingInterface {
    public:
        std::shared_ptr<GameState> game;

        ModdingInterface(std::shared_ptr<GameState> _game):
            game(_game)
        {}

        virtual void update(float dtime) {};
        virtual void handleEvent(Event &event) {};
        virtual void initMapgen(MapgenBase &mapgen) {};
    };

} // namespace Terrarium

#endif
