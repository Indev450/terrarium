#ifndef ITEM_DEF_MANAGER_HPP
#define ITEM_DEF_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

#include "item_def.hpp"

namespace Terrarium {

    class ItemDefManager {
        std::unordered_map<std::string, std::shared_ptr<ItemDef>> defs;

    public:
        void add(std::shared_ptr<ItemDef> def) {
            if (defs.find(def->name) != defs.end()) {
                std::cerr<<"Terrarium::ItemDefManager::add: ";
                std::cerr<<"attempt to redefine item '";
                std::cerr<<def->name;
                std::cerr<<"'"<<std::endl;

                return;
            }

            defs[def->name] = def;
        }

        std::shared_ptr<ItemDef> get(const std::string &name) {
            if (defs.find(name) != defs.end()) {
                return defs[name];
            }

            return nullptr;
        }
    };

}

#endif
