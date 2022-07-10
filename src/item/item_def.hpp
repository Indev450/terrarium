#ifndef ITEM_DEF_HPP
#define ITEM_DEF_HPP

#include <string>
#include <cstdint>

namespace Terrarium {

    struct ItemDef {
        std::string name = "default:name";
        std::string description = "Default description";

        uint16_t max_count = 1;
    };

}

#endif
