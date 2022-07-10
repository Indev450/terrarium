#ifndef ITEM_STACK_HPP
#define ITEM_STACK_HPP

#include <memory>

#include "item_def.hpp"

namespace Terrarium {

    class ItemStack {
        std::shared_ptr<ItemDef> def;
        uint16_t count = 0;

    public:
        void set(std::shared_ptr<ItemDef> def, uint16_t count = 1);

        int add(int change);

        void swap(ItemStack &other);

        inline void merge(ItemStack &other) {
            if (!canMerge(other)) { return; }

            other.count = add(other.count);

            if (!def) {
                other.def = def;
            }
        }

        inline bool canMerge(const ItemStack &other) {
            return def == other.def || empty() || other.empty();
        }

        inline std::shared_ptr<ItemDef> getDef() const {
            return def;
        }

        inline uint16_t getCount() const {
            return count;
        }

        inline bool empty() const {
            return !def || count == 0;
        }
    };

}

#endif
