#include <cmath>

#include "inventory.hpp"

namespace Terrarium {

    InventoryUI::InventoryUI(unsigned int _width, unsigned int _height, const Gfx &gfx):
        BG_COLOR(196, 196, 196, 127), CELL_COLOR(127, 127, 127),
        width(_width), height(_height)
    {
        background.setSize(sf::Vector2f(
            (CELL_SIZE * width) + (width+1) * SPACE,
            (CELL_SIZE * height) + (height+1) * SPACE));
        background.setFillColor(BG_COLOR);

        cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setFillColor(sf::Color(0, 0, 0, 0));
        cell.setOutlineThickness(OUTLINE);
        cell.setOutlineColor(CELL_COLOR);

        count_text.setFont(gfx.font);
        count_text.setCharacterSize(16);
        count_text.setFillColor(sf::Color::White);
    }

    bool InventoryUI::click(GameState &game, const sf::Vector2f &position) {
        // Calculate relative position
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        sf::Vector2f size = background.getSize();

        // Click out of bounds
        if (rpos.x < 0 || rpos.x > size.x ||
            rpos.y < 0 || rpos.y > size.y) {

            return false;
        }

        if (!inventory) {
            return true;
        }

        unsigned int clicked_cell_x = static_cast<unsigned int>(floor(rpos.x / (CELL_SIZE + SPACE)));
        unsigned int clicked_cell_y = static_cast<unsigned int>(floor(rpos.y / (CELL_SIZE + SPACE)));

        unsigned int clicked_index = clicked_cell_y * width + clicked_cell_x;

        game.player->hold_item_stack->swap(*inventory->get(clicked_index));

        return true;
    }

    void InventoryUI::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        sf::Transform combined_transform = getTransform() * parent_transform;

        target.draw(background, combined_transform);

        if (!inventory) {
            return;
        }

        unsigned int x = 0;
        unsigned int y = 0;

        for (unsigned int i = 0; i < inventory->size(); ++i) {
            x = i % width;
            y = i / width;

            sf::Vector2f cell_position(
                (CELL_SIZE * x) + (x+1) * SPACE,
                (CELL_SIZE * y) + (y+1) * SPACE
            );

            cell.setPosition(cell_position);
            target.draw(cell, combined_transform);

            std::shared_ptr<ItemStack> item = inventory->get(i);

            if (item->empty()) {
                continue;
            }

            // If item stack in hotbar is not empty, then item def is definitely not null
            std::shared_ptr<ItemDef> def = item->getDef();

            def->inventory_image.setPosition(cell_position);

            target.draw(def->inventory_image, combined_transform);

            // If there are more than one item, draw count
            if (item->getCount() > 1) {
                count_text.setString(std::to_string(item->getCount()));
                count_text.setPosition(cell_position);

                target.draw(count_text, combined_transform);
            }
        }
    }

}
