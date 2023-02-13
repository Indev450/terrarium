#include <cmath>

#include "inventory.hpp"

namespace Terrarium {

    InventoryUI::InventoryUI(const Gfx &gfx, unsigned int _width, unsigned int _height):
        BG_COLOR(196, 196, 196, 127),
        width(_width), height(_height),
        item_cell_renderer(gfx, sf::Color::White, sf::Color(0, 0, 0, 0), sf::Color(127, 127, 127))
    {
        background.setSize(item_cell_renderer.realGridSize(width, height));
        background.setFillColor(BG_COLOR);
    }

    std::optional<std::string> InventoryUI::hover(GameState &game, const sf::Vector2f &position) {
        // Calculate relative position
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        sf::Vector2f size = item_cell_renderer.realGridSize(width, height);

        // Click out of bounds
        if (rpos.x < 0 || rpos.x > size.x ||
            rpos.y < 0 || rpos.y > size.y) {

            return std::nullopt;
        }

        if (!inventory) {
            return std::nullopt;
        }

        sf::Vector2i hover_cell = item_cell_renderer.clickedCellPosition(rpos);

        unsigned int hover_index = hover_cell.y * width + hover_cell.x;

        auto hover_item = inventory->get(hover_index);

        if (!hover_item || hover_item->empty()) {
            return std::nullopt;
        }

        auto item_def = hover_item->getDef();

        return item_def->description;
    }

    bool InventoryUI::click(GameState &game, const sf::Vector2f &position) {
        // Calculate relative position
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        sf::Vector2f size = item_cell_renderer.realGridSize(width, height);

        // Click out of bounds
        if (rpos.x < 0 || rpos.x > size.x ||
            rpos.y < 0 || rpos.y > size.y) {

            return false;
        }

        if (!inventory) {
            return true;
        }

        sf::Vector2i clicked_cell = item_cell_renderer.clickedCellPosition(rpos);

        unsigned int clicked_index = clicked_cell.y * width + clicked_cell.x;

        auto clicked_item = inventory->get(clicked_index);

        if (!clicked_item) {
            return true;
        }

        if (game.player->hold_item_stack->canMerge(*clicked_item) &&
            !game.player->hold_item_stack->empty() &&
            !clicked_item->empty()) {

            clicked_item->merge(*game.player->hold_item_stack);
        } else {
            game.player->hold_item_stack->swap(*clicked_item);
        }

        return true;
    }

    void InventoryUI::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        sf::Transform combined_transform = getTransform() * parent_transform;

        target.draw(background, combined_transform);

        if (!inventory) {
            return;
        }

        ItemCellRendererSettings settings;

        for (unsigned int i = 0; i < inventory->size(); ++i) {
            settings.grid_x = i % width;
            settings.grid_y = i / width;

            item_cell_renderer.render(
                target, *inventory->get(i), combined_transform, settings);
        }
    }

}
