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

        game.player->hold_item_stack->swap(*inventory->get(clicked_index));

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
