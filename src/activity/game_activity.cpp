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
#include <cmath>
#include <chrono>

#include "game_activity.hpp"
#include "activity_manager.hpp"

#include "../utils/math.hpp"

namespace Terrarium {

    GameActivity::GameActivity(ActivityManager &am, std::shared_ptr<GameState> _game, const std::string &_save_name):
        game(_game),
        world_renderer(std::make_unique<WorldRenderer>(am.getWindow().getSize() + sf::Vector2u(Tile::SIZE, Tile::SIZE))),
        light_calc(game->camera.width, game->camera.height),
        item_cell_renderer(game->gfx, sf::Color::White, sf::Color::Transparent, sf::Color::Transparent),
        debug_overlay(game->gfx.font),
        tip_text("", game->gfx.font, 16),
        def_view()
    {
        game->modding_interface->onPlayerJoin(game->player);

        def_view = am.getWindow().getDefaultView();

        auto win_size = game->pixels_to_blocks.transformPoint(sf::Vector2f(am.getWindow().getSize()));

        game->camera.width = win_size.x;
        game->camera.height = win_size.y;

        tip_background.setFillColor(sf::Color(64, 64, 64, 127));
        tip_background.setOutlineColor(sf::Color(127, 127, 127, 127));
        tip_background.setOutlineThickness(4);
    }

    void GameActivity::update(ActivityManager &am, float dtime) {
        auto &window = am.getWindow();

        game->time += dtime;

        game->entity_mgr.update(*game, dtime);

        sf::Vector2f target_camera_pos = game->player->getPosition() - sf::Vector2f(game->camera.width/2, game->camera.height/2);

        // Offset caused by zoom
        float camera_off_x = (game->camera.width - game->camera.width * zoom) / 2;
        float camera_off_y = (game->camera.height - game->camera.height * zoom) / 2;

        game->camera.left = std::max<float>(
            -camera_off_x,
            std::min<float>(
                (target_camera_pos.x + game->camera.left) / 2,
                game->world.getWidth() + camera_off_x - game->camera.width));

        game->camera.top = std::max<float>(
            -camera_off_y,
            std::min<float>(
                (target_camera_pos.y + game->camera.top) / 2,
                game->world.getHeight() + camera_off_y  - game->camera.height));

        sf::Vector2u win_size = window.getSize();

        float cam_off_x = (win_size.x - win_size.x * zoom) / 2;
        float cam_off_y = (win_size.y - win_size.y * zoom) / 2;

        sf::Vector2i mouse_pos_pixels = sf::Mouse::getPosition(window);
        sf::Vector2f mouse_pos = game->pixels_to_blocks.transformPoint(mouse_pos_pixels.x*zoom + cam_off_x, mouse_pos_pixels.y*zoom + cam_off_y);

        game->player->controls.mouse_pos.x = game->camera.left + mouse_pos.x;
        game->player->controls.mouse_pos.y = game->camera.top + mouse_pos.y;

        item_cell_renderer.setPosition(sf::Vector2f(mouse_pos_pixels) - sf::Vector2f(16, 16));

        if (game->hud.tip) {
            tip_text.setString(*(game->hud.tip));
            tip_text.setPosition(sf::Vector2f(mouse_pos_pixels)
                                 + sf::Vector2f(10, 10) // Offset from cursor
                                 + sf::Vector2f(10, 5)); // Offset from background
            tip_background.setPosition(sf::Vector2f(mouse_pos_pixels)
                                       + sf::Vector2f(10, 10)); // Offset from cursor

            sf::FloatRect text_bounds = tip_text.getGlobalBounds();

            tip_background.setSize(sf::Vector2f(text_bounds.width, text_bounds.height) + sf::Vector2f(20, 20));
        }

        while (!game->events.empty()) {
            Event &event = game->events.front();

            game->modding_interface->handleEvent(event);

            game->events.pop();
        }

        game->modding_interface->update(dtime);
        game->client_modding_interface->update(dtime);

        game->sfx.update(*game, dtime);

        bool force_light_update = game->world.isUpdated();

        if (abs(int(game->natural_light) - int(light_new)) > light_change_step) {
            force_light_update = true;
            game->natural_light = light_new;
        }

        world_renderer->updatePosition(game->camera);
        world_renderer->update(*game);

        if (light_calc.updatePosition(game->camera) || force_light_update) {
            light_calc.updateLightInput(game);
        }
        light_calc.update(game->debug_info, force_light_update);

        game->hud.hover(*game, sf::Vector2f(mouse_pos_pixels));

        game->debug_info.updateFps(dtime);
        game->debug_info.time_of_day = game->time;
    }

    void GameActivity::render(sf::RenderTarget &target) {
        auto view = def_view;
        view.zoom(zoom);
        target.setView(view);

        sf::Color sky_color = sf::Color(90, 120, 240);

        if (!game->day_night_cycle.empty()) {
            float daytime = game->time - std::floor(game->time / game->day_length) * game->day_length;
            unsigned phases = game->day_night_cycle.size();

            game->debug_info.total_phases = phases;

            float timestamp = 0;

            for (unsigned i = 0; i < phases; ++i) {
                timestamp += game->day_night_cycle[i].length;

                if (daytime < timestamp || i == phases - 1) {
                    game->debug_info.phase_of_day = i;

                    sf::Color prev = game->day_night_cycle[i].sky_color;
                    uint8_t light_prev = game->day_night_cycle[i].light;

                    float interpolation = 1 - (timestamp - daytime) / game->day_night_cycle[i].length;

                    i = (i + 1 == phases) ? 0 : (i + 1);

                    sf::Color next = game->day_night_cycle[i].sky_color;
                    uint8_t light_next = game->day_night_cycle[i].light;

                    sky_color.r = float(prev.r) + (float(next.r) - float(prev.r)) * interpolation;
                    sky_color.g = float(prev.g) + (float(next.g) - float(prev.g)) * interpolation;
                    sky_color.b = float(prev.b) + (float(next.b) - float(prev.b)) * interpolation;

                    light_new = float(light_prev) + (float(light_next) - float(light_prev)) * interpolation;

                    break;
                }
            }
        }

        target.clear(sky_color);

        world_renderer->render(target);

        game->world_interact.highlightInteractive(*game, target);

        game->entity_mgr.draw(*game, target);

        target.draw(light_calc);

        view = def_view;
        target.setView(view);

        game->hud.render(target, *game);

        if (!game->player->hold_item_stack->empty()) {
            item_cell_renderer.render(
                target, *game->player->hold_item_stack, sf::Transform::Identity, ItemCellRendererSettings::Default);
        }

        if (game->hud.tip) {
            target.draw(tip_background);
            target.draw(tip_text);
        }

        if (show_debug) {
            debug_overlay.update(game->debug_info);
            target.draw(debug_overlay);
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
                def_view = sf::View(visible_area);

                sf::Vector2f new_size = game->pixels_to_blocks.transformPoint(event.size.width, event.size.height);

                game->camera.width = new_size.x;
                game->camera.height = new_size.y;

                light_calc.resize(new_size.x, new_size.y);

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

                    case sf::Keyboard::Hyphen:
                        zoom = std::min(ZOOM_MAX, zoom * 2);
                    break;

                    case sf::Keyboard::Equal:
                        zoom = std::max(ZOOM_MIN, zoom / 2);
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
                        game->hud.setVisible("craft", !game->hud.isVisible("craft"), false);
                        game->hud.setVisible("opened_inventory", false, false);
                    }
                    break;

                    case sf::Keyboard::E:
                        game->world_interact.interact(*game);
                    break;

                    case sf::Keyboard::O:
                        show_debug = !show_debug;
                    break;

                    case sf::Keyboard::Escape:
                    {
                        // This is so stupid but i can't figure out another way
                        // to make opened_inventory work properly :(
                        bool inventory_was_shown = game->hud.isVisible("inventory");

                        if (game->hud.hideFocused()) {
                            game->hud.setVisible("craft", game->hud.isVisible("inventory"), false);

                            if (inventory_was_shown && !game->hud.isVisible("inventory")) {
                                game->hud.setVisible("opened_inventory", false, false);
                            }

                            game->player->crafting_category = game->crafts.default_category;
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
