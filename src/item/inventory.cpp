#include <algorithm>

#include "inventory.hpp"

namespace Terrarium {

    void Inventory::addItem(ItemStack &new_item) {
        // First, try to find non-empty item stacks to merge
        for (auto &item: items) {
            item->merge(new_item);

            if (new_item.empty()) {
                return;
            }
        }

        // If new_item is still not empty, try to merge it with empty
        // item stacks
        for (auto &item: items) {
            if (item->empty()) {
                item->swap(new_item);
            }
        }
    }

    unsigned int Inventory::takeItems(std::shared_ptr<ItemDef> type, unsigned int count) {
        for (auto &item: items) {
            count = -item->add(-count);

            if (count == 0) {
                return 0;
            }
        }

        return count;
    }

    unsigned int Inventory::countItems(std::shared_ptr<ItemDef> type) {
        unsigned int result = 0;

        for (auto item: items) {
            if (item->getDef() == type) {
                result += item->getCount();
            }
        }

        return result;
    }

    std::shared_ptr<ItemStack> Inventory::find(std::shared_ptr<ItemDef> type) {
        for (auto &item: items) {
            if (item->getDef() == type && !item->empty()) {
                return item;
            }
        }

        return nullptr;
    }

} // namespace Terrarium
