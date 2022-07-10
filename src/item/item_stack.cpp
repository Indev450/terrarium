#include "item_stack.hpp"

namespace Terrarium {

    void ItemStack::set(std::shared_ptr<ItemDef> def, uint16_t count) {
        this->def = def;
        this->count = count;

        if (def) {
            this->count = std::min(count, def->max_count);
        }
    }

    int ItemStack::add(int change) {
        int left = 0;

        if (!def) {
            count += change;
            return left;
        }

        if (change < 0 && change < -static_cast<int>(count)) {
            left = change + count;

            count = 0;
        } else if (change + count > def->max_count) {
            left = change + count - def->max_count;

            count = def->max_count;
        } else {
            count += change;
        }

        return left;
    }

    void ItemStack::swap(ItemStack &other) {
        auto other_def = other.def;
        auto other_count = other.count;

        other.set(def, count);

        def = other_def;
        count = other_count;
    }

} // namespace Terrarium
