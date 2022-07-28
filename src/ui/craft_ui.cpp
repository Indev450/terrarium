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

#include "craft_ui.hpp"
#include "../game.hpp"
#include "../craft/craft.hpp"

namespace Terrarium {

    CraftUI::CraftUI(const Gfx &gfx, const std::string &_category):
        category(_category),
        item_cell_renderer(gfx, sf::Color::White, sf::Color(196, 196, 196, 127), sf::Color(127, 127, 127))
    {}

    bool CraftUI::click(GameState &game, const sf::Vector2f &position) {
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        sf::Vector2f size = item_cell_renderer.realGridSize(1, SHOW_RECIPES);

        if (rpos.x < 0 || rpos.x > size.x ||
            rpos.y < 0 || rpos.y > size.y) {

            return false;
        }

        std::vector<std::unique_ptr<Recipe>> &recipes = game.crafts.getRecipes(category);

        unsigned int recipe_index = item_cell_renderer.clickedCellPosition(rpos).y;

        recipe_index += current_scroll;

        if (recipe_index >= recipes.size()) {
            return false;
        }

        // TODO - do something with return value. Maybe send some kind of
        // "Craft" event?
        recipes[recipe_index]->craft(*game.player->inventory);

        return true;
    }

    bool CraftUI::scroll(GameState &game, const sf::Vector2f &position, float delta) {
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        sf::Vector2f size = item_cell_renderer.realGridSize(1, SHOW_RECIPES);

        if (rpos.x < 0 || rpos.x > size.x ||
            rpos.y < 0 || rpos.y > size.y) {

            return false;
        }

        if (game.crafts.getRecipes(category).size() < SHOW_RECIPES) {
            return true;
        }

        if (delta > 0) {
            current_scroll = std::min<unsigned int>(current_scroll+1, game.crafts.getRecipes(category).size() - SHOW_RECIPES);
        } else {
            if (current_scroll != 0) {
                --current_scroll;
            }
        }

        return true;
    }

    void CraftUI::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        sf::Transform combined_transform = getTransform() * parent_transform;

        std::vector<std::unique_ptr<Recipe>> &recipes = game.crafts.getRecipes(category);

        ItemCellRendererSettings settings;

        for (unsigned int i = 0; i < SHOW_RECIPES; ++i) {
            unsigned int recipe_index = current_scroll + i;

            if (recipe_index >= recipes.size()) {
                break;
            }

            Recipe &recipe = *recipes[recipe_index];

            std::vector<std::shared_ptr<ItemDef>> missing = recipe.getMissingItems(*game.player->inventory);

            settings.reset();

            settings.grid_y = i;
            settings.grid_x = 0;

            settings.text_color = sf::Color::Yellow;
            settings.cell_color = sf::Color(196, 169, 127, 127);

            item_cell_renderer.render(target, recipe.result, combined_transform, settings);

            // Add small gap
            combined_transform.translate(RECIPE_GAP, 0);

            for (auto &req: recipe.requirements) {
                settings.reset();

                settings.grid_x++;

                if (std::count(missing.begin(), missing.end(), req.getDef()) != 0) {
                    settings.text_color = sf::Color::Red;
                    settings.cell_color = sf::Color(196, 127, 127, 127);
                }

                item_cell_renderer.render(target, req, combined_transform, settings);
            }

            combined_transform.translate(-RECIPE_GAP, 0);
        }
    }

}

