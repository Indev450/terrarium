#include <string>

#include "hotbar.hpp"

namespace Terrarium {

    HotbarRenderer::HotbarRenderer(unsigned int _items_count, const Gfx &gfx):
        BG_COLOR(196, 196, 196, 127), CELL_COLOR(127, 127, 127),
        SELECTED_CELL_COLOR(64, 196, 196), items_count(_items_count)
    {
        background.setSize(sf::Vector2f(
            (CELL_SIZE * items_count) + (items_count+1) * SPACE,
            CELL_SIZE + SPACE*2));
        background.setFillColor(BG_COLOR);

        cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setFillColor(sf::Color(0, 0, 0, 0));
        cell.setOutlineThickness(OUTLINE);
        // Outline color set when drawing

        count_text.setFont(gfx.font);
        count_text.setCharacterSize(16);
        count_text.setFillColor(sf::Color::White);
    }

    void HotbarRenderer::render(sf::RenderTarget &target, GameState &game, const sf::Vector2f &position) {
        background.setPosition(position);

        target.draw(background);

        std::shared_ptr<ItemStack> *hotbar = game.player->getHotbar();

        for (unsigned int i = 0; i < game.player->HOTBAR_SIZE; ++i) {
            sf::Vector2f cell_position = position + sf::Vector2f((CELL_SIZE * i) + (i+1) * SPACE, SPACE);

            if (i == game.player->getHotbarSelected()) {
                cell.setOutlineColor(SELECTED_CELL_COLOR);
            } else {
                cell.setOutlineColor(CELL_COLOR);
            }

            cell.setPosition(cell_position);
            target.draw(cell);

            if (hotbar[i]->empty()) {
                continue;
            }

            // If item stack in hotbar is not empty, then item def is definitely not null
            std::shared_ptr<ItemDef> def = hotbar[i]->getDef();

            def->inventory_image.setPosition(cell_position);

            target.draw(def->inventory_image);

            // If there are more than one item, draw count
            if (hotbar[i]->getCount() > 1) {
                count_text.setString(std::to_string(hotbar[i]->getCount()));
                count_text.setPosition(cell_position);

                target.draw(count_text);
            }
        }
    }

}
