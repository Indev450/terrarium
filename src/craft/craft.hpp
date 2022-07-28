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

#ifndef CRAFT_HPP
#define CRAFT_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <utility>

#include "../item/inventory.hpp"

namespace Terrarium {

    struct Recipe {
        std::vector<ItemStack> requirements;

        ItemStack result;

        bool canCraft(const Inventory &inventory);

        std::vector<std::shared_ptr<ItemDef>> getMissingItems(const Inventory &inventory);

        // First element in pair is items that cannot be added to inventory.
        // Second element means was item crafted or not
        std::pair<std::shared_ptr<ItemStack>, bool> craft(Inventory &inventory);
    };

    class CraftManager {
        std::unordered_map<std::string, std::vector<std::unique_ptr<Recipe>>> recipes;

    public:
        void addCategory(const std::string &category);

        void addRecipe(const std::string &category, std::unique_ptr<Recipe> recipe);

        std::vector<std::unique_ptr<Recipe>> &getRecipes(const std::string &category);
    };

}

#endif
