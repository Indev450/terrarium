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
#include <fstream>
#include <memory>
#include <filesystem>
#include <ctime> // for time()

#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "graphics/gfx.hpp"
#include "graphics/world_renderer.hpp"
#include "ui/hud.hpp"
#include "ui/hotbar.hpp"
#include "ui/inventory.hpp"
#include "ui/rect_button.hpp"
#include "ui/container.hpp"
#include "ui/bar.hpp"
#include "ui/craft_ui.hpp"
#include "utils/saves.hpp"
#include "mapgen/mapgen_perlin.hpp"
#include "modding/lua/server/lua_interface.hpp"
#include "modding/lua/client/lua_client_interface.hpp"
#include "activity/activity_manager.hpp"
#include "activity/game_activity.hpp"
#include "activity/mapgen_activity.hpp"

// Those were in terrarium_config.hpp, but i anyway wasn't using any configuration
// in cmake other than version, so maybe it is not bad to set version here?
#define TERRARIUM_VERSION_MAJOR 0
#define TERRARIUM_VERSION_MINOR 3
#define TERRARIUM_VERSION_PATCH 6

const char *TITLE = "TerrariumEngine";

int main(int argc, char **argv)
{
    std::cout<<"TerrariumEngine version "
             <<TERRARIUM_VERSION_MAJOR<<'.'
             <<TERRARIUM_VERSION_MINOR<<'.'
             <<TERRARIUM_VERSION_PATCH<<std::endl;

    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 640), TITLE);
    window->setVerticalSyncEnabled(true);

    Terrarium::ActivityManager am(std::move(window));

    // BEGIN TEST CODE

    std::string save_name = "world";

    if (argc > 1) {
        save_name = argv[1];
    }

    std::shared_ptr<Terrarium::GameState> game = std::make_shared<Terrarium::GameState>(save_name);

    game->day_length = 60*16;

    game->day_night_cycle = {
        { sf::Color(70, 100, 220), 60.0*8 },
        { sf::Color(50, 80, 200), 60.0*2 },
        { sf::Color(130, 80, 180), 60.0 },
        { sf::Color(20, 10, 70), 60.0*4 },
        { sf::Color(30, 20, 80), 60.0 },
    };

    game->hud.setScreenSize(sf::Vector2f(800, 640));

    if (!game->gfx.font.loadFromFile("assets/dpcomic.ttf")) {
        std::cerr<<"Cannot load font"<<std::endl;
    }

    game->gfx.textures.addSearchPath("assets");
    game->gfx.textures.load("null.png");

    Terrarium::SavesManager saves_mgr("saves");

    bool save_exists = saves_mgr.saveExists(save_name);

    if (save_exists) {
        saves_mgr.loadWorldData(*game, save_name);
    }

    // Lua interface runs mods, that can override player prefab
    game->client_modding_interface = std::make_unique<Terrarium::LuaClientModdingInterface>(game);
    game->modding_interface = std::make_unique<Terrarium::LuaModdingInterface>(game);

    // Now player gets spawned. Hopefully with a normal prefab...
    Terrarium::entityid player_id = game->entity_mgr.create<Terrarium::Player>(1);
    game->player = std::dynamic_pointer_cast<Terrarium::Player>(game->entity_mgr.get(player_id));

    if (save_exists) {
        saves_mgr.loadPlayerData(*game, save_name);
        saves_mgr.loadInventoriesData(*game, save_name);

        am.setActivity(std::make_unique<Terrarium::GameActivity>(am, game, save_name));
    } else {
        // Create mapgen. I think mapgen can be configured when game will have
        // menu and settings, so i will leave hardcoded settings for now
        auto mapgen = std::make_unique<Terrarium::MapgenPerlin>(time(nullptr));

        mapgen->settings.ground_gen_scale = 1./20;

        mapgen->settings.cave_gen_scale_x = 1./20;
        mapgen->settings.cave_gen_scale_y = 1./20;

        mapgen->settings.biome_gen_scale_x = 1./300;
        mapgen->settings.biome_gen_scale_y = 1./200;

        mapgen->settings.min_block_density = 0.4;
        mapgen->settings.min_wall_density = 0.2;

        // In theory, i could leave mapgen configuration to lua, but i'm planning
        // to add more different mapgens in future (just like in minetest),
        // and i currently haven't figured "protocol" for polymorphic mapgen configuration.
        game->modding_interface->initMapgen(*mapgen);

        am.setActivity(std::make_unique<Terrarium::MapgenActivity>(game, std::move(mapgen), 2000, 1000, save_name));
    }

    // HUD elements

    ////////////////////////////////////////////////////////////////////
    auto hotbar_renderer = std::make_unique<Terrarium::HotbarRenderer>(
        game->gfx, Terrarium::Player::HOTBAR_SIZE);

    hotbar_renderer->setPosition(32, 32);

    game->hud.addElement("hotbar", std::move(hotbar_renderer));

    ////////////////////////////////////////////////////////////////////
    auto inventory_ui = std::make_unique<Terrarium::InventoryUI>(
        game->gfx,
        Terrarium::Player::HOTBAR_SIZE,
        Terrarium::Player::INVENTORY_ROWS);

    inventory_ui->inventory = game->player->inventory;
    inventory_ui->setPosition(32, 74);
    inventory_ui->visible = false;

    game->hud.addElement("inventory", std::move(inventory_ui));

    ////////////////////////////////////////////////////////////////////
    // Dummy element for opened inventories.
    inventory_ui = std::make_unique<Terrarium::InventoryUI>(
        game->gfx, 0, 0);
    inventory_ui->setPosition(
        32 + Terrarium::ItemCellRenderer::realGridSize(Terrarium::Player::HOTBAR_SIZE, 0).x + 4,
        74);
    inventory_ui->visible = false;

    game->hud.addElement("opened_inventory", std::move(inventory_ui));

    ////////////////////////////////////////////////////////////////////
    auto pause_container = std::make_unique<Terrarium::UIContainer>(
        sf::Vector2f(100, 110));
    pause_container->setPosition(-50, -55);
    pause_container->setOriginType(ScreenTransformable::Origin::ScreenCenter);

    auto continue_button = std::make_unique<Terrarium::RectButton>(
        game->gfx, sf::Vector2f(80, 40),
        [&] (Terrarium::GameState &game) {
            game.hud.setVisible("pause", false);
        }, "continue", 20);
    continue_button->setTextColor(sf::Color::White);
    continue_button->setBackgroundColor(sf::Color(127, 127, 127, 127));
    continue_button->setOutlineColor(sf::Color(127, 127, 127, 255));
    continue_button->setOutlineThickness(4);
    continue_button->setPosition(10, 10);

    // Set size of container as screen size. I honestly don't want to fix
    // this properly right now. Maybe later. UI is too hard for me
    continue_button->setScreenSize(sf::Vector2f(100, 110));

    pause_container->addElement(std::move(continue_button));

    auto exit_button = std::make_unique<Terrarium::RectButton>(
        game->gfx, sf::Vector2f(80, 40),
        [&] (Terrarium::GameState &game) {
            am.getWindow().close();
        }, "exit", 20);
    exit_button->setTextColor(sf::Color::White);
    exit_button->setBackgroundColor(sf::Color(127, 127, 127, 127));
    exit_button->setOutlineColor(sf::Color(127, 127, 127, 255));
    exit_button->setOutlineThickness(4);
    exit_button->setPosition(10, 60);

    // Same as above
    exit_button->setScreenSize(sf::Vector2f(100, 110));

    pause_container->addElement(std::move(exit_button));

    pause_container->visible = false;

    game->hud.addElement("pause", std::move(pause_container));

    ////////////////////////////////////////////////////////////////////
    auto craft_ui = std::make_unique<Terrarium::CraftUI>(game->gfx);
    craft_ui->setPosition(32, 512);
    craft_ui->visible = false;

    game->hud.addElement("craft", std::move(craft_ui));

    ////////////////////////////////////////////////////////////////////
    am.run();

    // END TEST CODE

    return 0;
}
