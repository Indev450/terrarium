/*
 * Copyright 2022 Inner Devil
 *
 * This file is part of TerrariumEngine
 *
 * TerrariumEngine is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

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
