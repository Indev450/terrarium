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
#include <sstream>
#include <memory>
#include <ctime> // for time()

#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "graphics/gfx.hpp"
#include "graphics/world_renderer.hpp"
#include "ui/hud.hpp"
#include "ui/hotbar.hpp"
#include "ui/inventory.hpp"
#include "mapgen/mapgen_perlin.hpp"
#include "modding/lua/lua_interface.hpp"

// Those were in terrarium_config.hpp, but i anyway wasn't using any configuration
// in cmake other than version, so maybe it is not bad to set version here?
#define TERRARIUM_VERSION_MAJOR 0
#define TERRARIUM_VERSION_MINOR 1

const char *TITLE = "TerrariumEngine";

int main()
{
    std::cout<<"TerrariumEngine version "<<TERRARIUM_VERSION_MAJOR<<'.'<<TERRARIUM_VERSION_MINOR<<std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 640), TITLE);
    window.setVerticalSyncEnabled(true);

    // BEGIN TEST CODE

    std::shared_ptr<Terrarium::GameState> game = std::make_shared<Terrarium::GameState>(2000, 1000);

    if (!game->gfx.font.loadFromFile("assets/dpcomic.ttf")) {
        std::cerr<<"Cannot load font"<<std::endl;
    }

    // Add player prefab, with id 1.
    std::shared_ptr<Terrarium::EntityPrefab> player_prefab = std::make_shared<Terrarium::EntityPrefab>();
    Terrarium::entity_prefabid player_prefab_id = game->entity_mgr.addPrefab(player_prefab);

    // Lua interface runs mods, that can override player prefab
    Terrarium::LuaModdingInterface lua_interface(game);

    // Now player gets spawned. Hopefully with a normal prefab...
    Terrarium::entityid player_id = game->entity_mgr.create<Terrarium::Player>(player_prefab_id);
    game->player = std::dynamic_pointer_cast<Terrarium::Player>(game->entity_mgr.get(player_id));

    // Create mapgen. I think mapgen can be configured when game will have
    // menu and settings, so i will leave hardcoded settings for now
    Terrarium::MapgenPerlin mapgen(time(nullptr));

    mapgen.settings.ground_gen_scale = 1./20;

    mapgen.settings.cave_gen_scale_x = 1./20;
    mapgen.settings.cave_gen_scale_y = 1./20;

    mapgen.settings.biome_gen_scale_x = 1./300;
    mapgen.settings.biome_gen_scale_y = 1./200;

    mapgen.settings.min_block_density = 0.4;
    mapgen.settings.min_wall_density = 0.2;

    // In theory, i could leave mapgen configuration to lua, but i'm planning
    // to add more different mapgens in future (just like in minetest),
    // and i currently haven't figured "protocol" for polymorphic mapgen configuration.
    lua_interface.initMapgen(mapgen);

    // Mapgen currently is kinda slow, maybe its good idea to run it in
    // different thread and draw some kind of "Generating map, please wait..."
    // in GUI
    mapgen.run(game->world);

    lua_interface.onPlayerJoin(game->player);

    // Maybe world renderer step needs to be configured too
    Terrarium::WorldRenderer world_renderer({ 800, 640 }, 8);

    auto hotbar_renderer = std::make_unique<Terrarium::HotbarRenderer>(
        Terrarium::Player::HOTBAR_SIZE, game->gfx);

    hotbar_renderer->setPosition(32, 32);

    auto inventory_ui = std::make_unique<Terrarium::InventoryUI>(
        Terrarium::Player::HOTBAR_SIZE,
        Terrarium::Player::INVENTORY_ROWS,
        game->gfx);

    inventory_ui->inventory = game->player->inventory;
    inventory_ui->setPosition(32, 74);
    inventory_ui->visible = false;

    game->hud.addElement("hotbar", std::move(hotbar_renderer));
    game->hud.addElement("inventory", std::move(inventory_ui));

    sf::Clock clock;

    // For testing, currently. Maybe this needs to be added in HUD too
    const float fps_update_time = 0.2;
    float fps_show_timer = fps_update_time;

    sf::Text fps_text;
    fps_text.setCharacterSize(18);
    fps_text.setPosition(12, 12);
    fps_text.setFillColor(sf::Color::Black);
    fps_text.setFont(game->gfx.font);

    while (window.isOpen()) {
        // Update input
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                break;

                case sf::Event::Resized:
                {
                    sf::Vector2f new_size = game->pixels_to_blocks.transformPoint(event.size.width, event.size.height);

                    game->camera.width = new_size.x;
                    game->camera.height = new_size.y;

                    world_renderer.setScreenSize({ event.size.width + Terrarium::Tile::SIZE, event.size.height + Terrarium::Tile::SIZE });
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
                            game->player->hotbar_scroll = -1;
                        break;

                        case sf::Keyboard::N:
                            game->player->hotbar_scroll = 1;
                        break;

                        case sf::Keyboard::I:
                            game->hud.setVisible("inventory", !game->hud.isVisible("inventory"));
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
                    game->player->hotbar_scroll = -event.mouseWheelScroll.delta;
                break;

                default:
                break;
            }
        }

        // Update logic
        float dtime = clock.restart().asSeconds();

        fps_show_timer -= dtime;

        if (fps_show_timer < 0) {
            std::string text = "fps: ";
            text += std::to_string(1./dtime);

            fps_text.setString(text);

            fps_show_timer = fps_update_time;
        }

        game->entity_mgr.update(*game, dtime);

        sf::Vector2f camera_pos = game->player->getPosition(*game) - sf::Vector2f(game->camera.width/2, game->camera.height/2);
        game->camera.left = camera_pos.x;
        game->camera.top = camera_pos.y;

        sf::Vector2i mouse_pos_pixels = sf::Mouse::getPosition(window);
        sf::Vector2f mouse_pos = game->pixels_to_blocks.transformPoint(mouse_pos_pixels.x, mouse_pos_pixels.y);

        game->player->controls.mouse_pos.x = game->camera.left + mouse_pos.x;
        game->player->controls.mouse_pos.y = game->camera.top + mouse_pos.y;

        while (!game->events.empty()) {
            Terrarium::Event &event = game->events.front();

            lua_interface.handleEvent(event);

            game->events.pop();
        }

        lua_interface.update(dtime);

        // Draw
        window.clear(sf::Color::Blue);

        world_renderer.updatePosition(game->camera);
        world_renderer.update(*game);
        world_renderer.render(window);

        game->entity_mgr.draw(*game, window);

        window.draw(fps_text);

        game->hud.render(window, *game);

        window.display();
    }

    // END TEST CODE

    return 0;
}
