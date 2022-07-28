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

#include <stdexcept>

#include "craft.hpp"

namespace Terrarium {

    bool Recipe::canCraft(const Inventory &inventory) {
        return getMissingItems(inventory).empty();
    }

    std::vector<std::shared_ptr<ItemDef>> Recipe::getMissingItems(const Inventory &inventory) {
        std::vector<std::shared_ptr<ItemDef>> result;

        for (auto &req: requirements) {
            if (inventory.countItems(req.getDef()) < req.getCount()) {
                result.push_back(req.getDef());
            }
        }

        return result;
    }

    std::pair<std::shared_ptr<ItemStack>, bool> Recipe::craft(Inventory &inventory) {
        std::shared_ptr<ItemStack> to_add = std::make_shared<ItemStack>();

        if (!canCraft(inventory)) {
            return std::make_pair(to_add, false);
        }

        for (auto &req: requirements) {
            inventory.takeItems(req.getDef(), req.getCount());
        }

        // Copy
        to_add = std::make_shared<ItemStack>(result);

        inventory.addItem(*to_add);

        return std::make_pair(to_add, true);
    }

    void CraftManager::addCategory(const std::string &category) {
        recipes[category] = std::vector<std::unique_ptr<Recipe>>();
    }

    void CraftManager::addRecipe(const std::string &category, std::unique_ptr<Recipe> recipe) {
        if (recipes.count(category) == 0) {
            std::string msg = "Terrarium::CraftManager::addRecipe: no such category: ";
            msg += category;

            throw std::invalid_argument(msg);
        }

        recipes[category].push_back(std::move(recipe));
    }

    std::vector<std::unique_ptr<Recipe>> &CraftManager::getRecipes(const std::string &category) {
        if (recipes.count(category) == 0) {
            std::string msg = "Terrarium::CraftManager::getRecipes: no such category: ";
            msg += category;

            throw std::invalid_argument(msg);
        }

        return recipes[category];
    }

}
