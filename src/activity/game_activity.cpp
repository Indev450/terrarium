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

#include <iostream>

#include "game_activity.hpp"
#include "activity_manager.hpp"

namespace Terrarium {

    GameActivity::GameActivity(ActivityManager &am, std::shared_ptr<GameState> _game, const std::string &_save_name):
        game(_game),
        world_renderer(std::make_unique<WorldRenderer>(am.getWindow().getSize() + sf::Vector2u(Tile::SIZE, Tile::SIZE))),
        item_cell_renderer(game->gfx, sf::Color::White, sf::Color::Transparent, sf::Color::Transparent)
    {
        game->modding_interface->onPlayerJoin(game->player);

        auto win_size = game->pixels_to_blocks.transformPoint(sf::Vector2f(am.getWindow().getSize()));

        game->camera.width = win_size.x;
        game->camera.height = win_size.y;
    }

    void GameActivity::update(ActivityManager &am, float dtime) {
        auto &window = am.getWindow();

        game->entity_mgr.update(*game, dtime);

        sf::Vector2f camera_pos = game->player->getPosition() - sf::Vector2f(game->camera.width/2, game->camera.height/2);

        game->camera.left = std::max<float>(
            0,
            std::min<float>(
                camera_pos.x,
                game->world.getWidth() - game->camera.width));

        game->camera.top = std::max<float>(
            0,
            std::min<float>(
                camera_pos.y,
                game->world.getHeight() - game->camera.height));

        sf::Vector2i mouse_pos_pixels = sf::Mouse::getPosition(window);
        sf::Vector2f mouse_pos = game->pixels_to_blocks.transformPoint(mouse_pos_pixels.x, mouse_pos_pixels.y);

        game->player->controls.mouse_pos.x = game->camera.left + mouse_pos.x;
        game->player->controls.mouse_pos.y = game->camera.top + mouse_pos.y;

        item_cell_renderer.setPosition(sf::Vector2f(mouse_pos_pixels) - sf::Vector2f(16, 16));

        while (!game->events.empty()) {
            Event &event = game->events.front();

            if (event.type == Event::UISubmit) {
                // TODO - Maybe change those form and field names into integers and
                // do switch instead of if-else if-... statements?
                if (event.ui->form == "pause") {
                    for (auto &field: event.ui->fields) {
                        if (field.first == "continue" && !field.second.empty()) {
                            game->hud.setVisible("pause", false);
                        } else if (field.first == "exit" && !field.second.empty()) {
                            window.close();
                        }
                    }

                    game->events.pop();
                    continue;
                }
            }

            game->modding_interface->handleEvent(event);

            game->events.pop();
        }

        game->modding_interface->update(dtime);

        game->sfx.update(*game);

        world_renderer->updatePosition(game->camera);
        world_renderer->update(*game);
    }

    void GameActivity::render(sf::RenderTarget &target) {
        target.clear(sf::Color::Blue);

        world_renderer->render(target);

        game->entity_mgr.draw(*game, target);

        game->hud.render(target, *game);

        if (!game->player->hold_item_stack->empty()) {
            item_cell_renderer.render(
                target, *game->player->hold_item_stack, sf::Transform::Identity, ItemCellRendererSettings::Default);
        }
    };

    void GameActivity::onEvent(ActivityManager &am, sf::Event event) {
        auto &window = am.getWindow();

        switch (event.type) {
            case sf::Event::Closed:
                window.close();
            break;

            case sf::Event::Resized:
            {
                sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visible_area));

                sf::Vector2f new_size = game->pixels_to_blocks.transformPoint(event.size.width, event.size.height);

                game->camera.width = new_size.x;
                game->camera.height = new_size.y;

                // FIXME - recreating world renderer each time screen size changed
                // is temporary solution. I probably just need to fix
                // WorldRenderer::setScreenSize instead.
                world_renderer = std::make_unique<WorldRenderer>(sf::Vector2u{
                    event.size.width + Tile::SIZE,
                    event.size.height + Tile::SIZE });

                game->hud.setScreenSize(sf::Vector2f(event.size.width, event.size.height));
            }
            break;

            case sf::Event::KeyPressed:
            {
                switch (event.key.code) {
                    case sf::Keyboard::A:
                        game->player->controls.left = true;
                    break;

                    case sf::Keyboard::D:
                        game->player->controls.right = true;
                    break;

                    case sf::Keyboard::Space:
                        game->player->controls.jump = true;
                    break;

                    case sf::Keyboard::B:
                    {
                        sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));

                        if (!game->hud.scroll(*game, mouse_pos, -1)) {
                            game->player->hotbar_scroll = -1;
                        }
                    }
                    break;

                    case sf::Keyboard::N:
                    {
                        sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));

                        if (!game->hud.scroll(*game, mouse_pos, 1)) {
                            game->player->hotbar_scroll = 1;
                        }
                    }
                    break;

                    case sf::Keyboard::I:
                    {
                        if (game->hud.isVisible("craft") || !game->player->crafting_category) {
                            game->player->crafting_category = game->crafts.default_category;
                        }

                        game->hud.setVisible("inventory", !game->hud.isVisible("inventory"));
                        game->hud.setVisible("craft", !game->hud.isVisible("craft"));
                        game->hud.setVisible("opened_inventory", false);
                    }
                    break;

                    case sf::Keyboard::E:
                        game->world_interact.interact(*game);
                    break;

                    case sf::Keyboard::Escape:
                    {
                        if (game->hud.isVisible("inventory")) {
                            game->hud.setVisible("inventory", false);
                            game->hud.setVisible("craft", false);
                            game->hud.setVisible("opened_inventory", false);
                        } else {
                            game->hud.setVisible("pause", !game->hud.isVisible("pause"));
                        }
                    }
                    break;

                    default:
                    break;
                }
            }
            break;

            case sf::Event::KeyReleased:
            {
                switch (event.key.code) {
                    case sf::Keyboard::A:
                        game->player->controls.left = false;
                    break;

                    case sf::Keyboard::D:
                        game->player->controls.right = false;
                    break;

                    case sf::Keyboard::Space:
                        game->player->controls.jump = false;
                    break;

                    default:
                    break;
                }
            }
            break;

            case sf::Event::MouseButtonPressed:
            {
                switch (event.mouseButton.button) {
                    case sf::Mouse::Left:
                    {
                        sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));

                        if (!game->hud.click(*game, mouse_pos)) {
                            game->player->controls.lmb = true;
                        }
                    }
                    break;

                    case sf::Mouse::Right:
                        game->player->controls.rmb = true;
                    break;

                    default:
                    break;
                }
            }
            break;

            case sf::Event::MouseButtonReleased:
            {
                switch (event.mouseButton.button) {
                    case sf::Mouse::Left:
                        game->player->controls.lmb = false;
                    break;

                    case sf::Mouse::Right:
                        game->player->controls.rmb = false;
                    break;

                    default:
                    break;
                }
            }
            break;

            case sf::Event::MouseWheelScrolled:
            {
                sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));

                if (!game->hud.scroll(*game, mouse_pos, event.mouseWheelScroll.delta)) {
                    game->player->hotbar_scroll = -event.mouseWheelScroll.delta;
                }
            }
            break;

            default:
            break;
        }
    }

    GameActivity::~GameActivity() {
        game->save();
    }

}
