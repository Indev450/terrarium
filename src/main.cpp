/*
 * Copyright 2022 Indev
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
#include "ui/chat.hpp"
#include "utils/saves.hpp"
#include "utils/datafile.hpp"
#include "mapgen/mapgen_perlin.hpp"
#include "modding/lua/server/lua_interface.hpp"
#include "modding/lua/client/lua_client_interface.hpp"
#include "activity/activity_manager.hpp"
#include "activity/game_activity.hpp"
#include "activity/mapgen_activity.hpp"

// Those were in terrarium_config.hpp, but i anyway wasn't using any configuration
// in cmake other than version, so maybe it is not bad to set version here?
#define TERRARIUM_VERSION_MAJOR 0
#define TERRARIUM_VERSION_MINOR 9
#define TERRARIUM_VERSION_PATCH 0

#define SETTINGS_VERSION 3

void applyDefaultSettings(Terrarium::Datafile &settings) {
    auto &video = settings["video"];

    video["vsync"].setBool(true);
    video["max_fps"].setInt(60);

    video["resolution"].setInt(800, 0);
    video["resolution"].setInt(640, 1);

    video["smooth_lighting"].setBool(true);

    video["dither_lighting"].setBool(true);

    auto &audio = settings["audio"];
    audio["music_volume"].setReal(0.6);
    audio["sounds_volume"].setReal(0.6);

    auto &mapgen = settings["mapgen"];

    mapgen["world_size"].setInt(2000, 0);
    mapgen["world_size"].setInt(1000, 1);

    auto &perlin = mapgen["perlin"];

    perlin["ground_gen_scale"].setReal(0.02);
    perlin["ground_height_amp"].setInt(50);
    perlin["base_ground_height"].setReal(0.05);

    perlin["cave_gen_scale"].setReal(0.025, 0);
    perlin["cave_gen_scale"].setReal(0.03, 1);

    perlin["biome_gen_scale"].setReal(0.0035, 0);
    perlin["biome_gen_scale"].setReal(0.004, 1);

    perlin["min_block_density"].setReal(0.01);
    perlin["max_block_density"].setReal(0.31);

    perlin["min_density_factor"].setReal(0.25);
    perlin["max_density_factor"].setReal(0.8);

    perlin["min_cave_scale_factor"].setReal(0.6);
    perlin["max_cave_scale_factor"].setReal(0.8);
}

const char *TITLE = "TerrariumEngine Legacy";

int main(int argc, char **argv)
{
    std::cout<<"TerrariumEngine version "
             <<TERRARIUM_VERSION_MAJOR<<'.'
             <<TERRARIUM_VERSION_MINOR<<'.'
             <<TERRARIUM_VERSION_PATCH<<std::endl;

    Terrarium::Datafile settings;

    // Create "version" field so it is in the beginning of file
    settings["version"];

    applyDefaultSettings(settings);

    // Would create default settings file if it didn't exist or is outdated
    if (!settings.loadFromFile("settings.txt") || settings["version"].getInt() < SETTINGS_VERSION) {
        std::cout<<"Writing to settings file"<<std::endl;
        settings["version"].setInt(SETTINGS_VERSION);
        settings.saveToFile("settings.txt");
    }

    auto &video = settings["video"];

    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(video["resolution"].getInt(0), video["resolution"].getInt(1)), TITLE);

    if (video["vsync"].getBool()) {
        window->setVerticalSyncEnabled(true);
    } else {
        window->setFramerateLimit(video["max_fps"].getInt());
    }

    sf::Image icon;

    if (!icon.loadFromFile("assets/icons/terrarium.32.png")) {
        std::cerr<<"Icon not found"<<std::endl;
    } else {
        sf::Vector2u size = icon.getSize();
        window->setIcon(size.x, size.y, icon.getPixelsPtr());
    }

    Terrarium::ActivityManager am(std::move(window));

    // BEGIN TEST CODE

    std::string save_name = "world";

    if (argc > 1) {
        save_name = argv[1];
    }

    std::shared_ptr<Terrarium::GameState> game = Terrarium::GameState::create(save_name);

    game->settings = settings;

    game->day_length = 60*16;

    game->day_night_cycle = {
        { sf::Color(30, 20, 80),   60.0*1, 200 },
        { sf::Color(70, 100, 220), 60.0*8, 255 },
        { sf::Color(50, 80, 200),  60.0*2, 200 },
        { sf::Color(130, 80, 180), 60.0*1, 100 },
        { sf::Color(20, 10, 70),   60.0*4, 40  },
    };

    game->sfx.music_volume = settings["audio"]["music_volume"].getReal();
    game->sfx.sounds_volume = settings["audio"]["sounds_volume"].getReal();

    game->hud.setScreenSize(sf::Vector2f(video["resolution"].getInt(0), video["resolution"].getInt(1)));

    if (!game->gfx.font.loadFromFile("assets/monogram.ttf")) {
        std::cerr<<"Cannot load font"<<std::endl;
    }

    // Don't smooth muh pixel font!
    game->gfx.font.setSmooth(false);

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
        game->modding_interface->load();

        am.setActivity(std::make_unique<Terrarium::GameActivity>(am, game, save_name));
    } else {
        auto &mgsettings = settings["mapgen"];
        auto &perlin = mgsettings["perlin"];

        auto mapgen = std::make_unique<Terrarium::MapgenPerlin>(time(nullptr));

        mapgen->settings.ground_gen_scale = perlin["ground_gen_scale"].getReal();

        mapgen->settings.height_amp = perlin["ground_height_amp"].getInt();

        mapgen->settings.base_ground_height = perlin["base_ground_height"].getReal();

        mapgen->settings.cave_gen_scale_x = perlin["cave_gen_scale"].getReal(0);
        mapgen->settings.cave_gen_scale_y = perlin["cave_gen_scale"].getReal(1);

        mapgen->settings.biome_gen_scale_x = perlin["biome_gen_scale"].getReal(0);
        mapgen->settings.biome_gen_scale_y = perlin["biome_gen_scale"].getReal(1);

        mapgen->settings.min_block_density = perlin["min_block_density"].getReal();
        mapgen->settings.max_block_density = perlin["max_block_density"].getReal();

        mapgen->settings.min_density_factor = perlin["min_density_factor"].getReal();
        mapgen->settings.max_density_factor = perlin["max_density_factor"].getReal();

        mapgen->settings.min_cave_scale_factor = perlin["min_cave_scale_factor"].getReal();
        mapgen->settings.max_cave_scale_factor = perlin["max_cave_scale_factor"].getReal();

        // In theory, i could leave mapgen configuration to lua, but i'm planning
        // to add more different mapgens in future (just like in minetest),
        // and i currently haven't figured "protocol" for polymorphic mapgen configuration.
        game->modding_interface->initMapgen(*mapgen);

        am.setActivity(std::make_unique<Terrarium::MapgenActivity>(game, std::move(mapgen), mgsettings["world_size"].getInt(0), mgsettings["world_size"].getInt(1), save_name));
    }

    // HUD elements

    ////////////////////////////////////////////////////////////////////
    auto hotbar_renderer = std::make_shared<Terrarium::HotbarRenderer>(
        game->gfx, Terrarium::Player::HOTBAR_SIZE);

    hotbar_renderer->setPosition(32, 32);

    game->hud.addElement("hotbar", hotbar_renderer, false);

    ////////////////////////////////////////////////////////////////////
    auto inventory_ui = std::make_shared<Terrarium::InventoryUI>(
        game->gfx,
        Terrarium::Player::HOTBAR_SIZE,
        Terrarium::Player::INVENTORY_ROWS);

    inventory_ui->inventory = game->player->inventory;
    inventory_ui->setPosition(32, 74);
    inventory_ui->visible = false;

    game->hud.addElement("inventory", inventory_ui);

    ////////////////////////////////////////////////////////////////////
    // Dummy element for opened inventories.
    inventory_ui = std::make_shared<Terrarium::InventoryUI>(
        game->gfx, 0, 0);
    inventory_ui->setPosition(
        32 + Terrarium::ItemCellRenderer::realGridSize(Terrarium::Player::HOTBAR_SIZE, 0).x + 4,
        74);
    inventory_ui->visible = false;

    game->hud.addElement("opened_inventory", inventory_ui);

    ////////////////////////////////////////////////////////////////////
    auto pause_container = std::make_shared<Terrarium::UIContainer>(
        sf::Vector2f(100, 110));
    pause_container->setPosition(-50, -55);
    pause_container->setOriginType(ScreenTransformable::Origin::ScreenCenter);

    auto continue_button = std::make_shared<Terrarium::RectButton>(
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

    pause_container->addElement(continue_button);

    auto exit_button = std::make_shared<Terrarium::RectButton>(
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

    pause_container->addElement(exit_button);

    pause_container->visible = false;

    game->hud.addElement("pause", pause_container);

    ////////////////////////////////////////////////////////////////////
    auto craft_ui = std::make_shared<Terrarium::CraftUI>(game->gfx);
    craft_ui->setPosition(32, 512);
    craft_ui->visible = false;

    game->hud.addElement("craft", craft_ui);

    ////////////////////////////////////////////////////////////////////
    auto chat_ui = std::make_shared<Terrarium::ChatUI>(*game);
    int offset = -32 - Terrarium::ChatUI::FONT_SIZE*(Terrarium::ChatUI::MAX_SHOW_MESSAGES+2) - Terrarium::ChatUI::GAP*(Terrarium::ChatUI::MAX_SHOW_MESSAGES+3);

    chat_ui->setPosition(32, offset);
    chat_ui->visible = false;

    game->hud.addElement("chat", chat_ui);

    ////////////////////////////////////////////////////////////////////
    am.run();

    // END TEST CODE

    return 0;
}
