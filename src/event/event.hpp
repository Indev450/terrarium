#ifndef EVENT_HPP
#define EVENT_HPP

#include <cassert>
#include <memory>

#include "../entity/entity.hpp"
#include "../item/item_def.hpp"

namespace Terrarium {

    struct ItemEvent {
        std::shared_ptr<ItemDef> def;
        std::weak_ptr<Entity> user;

        sf::Vector2f position;
    };

    struct Event {
        const enum Type {
            ItemUseStart = 0,
            ItemUseStop,

            ItemAltUseStart,
            ItemAltUseStop,

            ItemSelect, // Start wield item. Can be used for torches
        } type;

        const union {
            ItemEvent *item = nullptr;
            // ...more events data
        };

        Event(Type _type, ItemEvent *_item):
            type(_type), item(_item) {

            assert(type <= ItemSelect);
        }

        // Delete copy because i don't know how union with smart pointers inside
        // would behave.
        Event(const Event &copy) = delete;
        Event &operator=(const Event &copy) = delete;

        ~Event() {
            switch (type) {
                case ItemUseStart:
                case ItemUseStop:
                case ItemAltUseStart:
                case ItemAltUseStop:
                case ItemSelect:
                {
                    delete item;
                }
                break;
            }
        }
    };

}

#endif
