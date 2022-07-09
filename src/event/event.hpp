#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include "../entity/entity.hpp"

namespace Terrarium {

    struct Event {
        enum Type {
            ItemUseStart,
            ItemUseStop,

            ItemAltUseStart,
            ItemAltUseStop,

            ItemSelect, // Start wield item. Can be used for torches
        } type;

        union {
            struct {
                std::string item_id;
                Entity *user = nullptr;
            } item;
        };
    };

}

#endif
