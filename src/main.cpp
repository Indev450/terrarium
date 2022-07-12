#include <iostream>
#include <sstream>
#include <memory>
#include <cstdlib> // for rand()
#include <ctime> // for time()

#include <terrarium_config.hpp>

#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "graphics/gfx.hpp"
#include "graphics/world_renderer.hpp"
#include "ui/hotbar.hpp"
#include "modding/lua/lua_interface.hpp"

const char *TITLE = "TerrariumEngine";

int main()
{
    std::cout<<"TerrariumEngine version "<<TERRARIUM_VERSION_MAJOR<<'.'<<TERRARIUM_VERSION_MINOR<<std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 640), TITLE);
    window.setVerticalSyncEnabled(true);

    // BEGIN TEST CODE

    std::shared_ptr<Terrarium::GameState> game = std::make_shared<Terrarium::GameState>(512, 512);

    std::string dirt_texture = "assets/dirt.png";
    std::string grass_texture = "assets/grass.png";
    std::string player_texture = "assets/player.png";

    game->gfx.loadTexture(dirt_texture);
    game->gfx.loadTexture(grass_texture);
    game->gfx.loadTexture(player_texture);

    if (!game->gfx.font.loadFromFile("assets/dpcomic.ttf")) {
        std::cerr<<"Cannot load font"<<std::endl;
    }

    Terrarium::LuaModdingInterface lua_interface(game);

    std::shared_ptr<Terrarium::ItemDef> dirt_item_def(new Terrarium::ItemDef);

    dirt_item_def->name = "default:dirt";
    dirt_item_def->description = "Dirt block";

    dirt_item_def->inventory_image.setTexture(game->gfx.getTexture(dirt_texture));

    dirt_item_def->max_count = 999;

    game->item_defs.add(dirt_item_def);

    Terrarium::ItemStack dirt_item;
    dirt_item.set(dirt_item_def, 64);

    game->player.inventory.addItem(dirt_item);

    std::shared_ptr<Terrarium::BlockDef> dirt_def(new Terrarium::BlockDef);

    dirt_def->sprite.setTexture(game->gfx.getTexture(dirt_texture));
    dirt_def->slippery = 0.85;

    std::shared_ptr<Terrarium::BlockDef> grass_def(new Terrarium::BlockDef);

    grass_def->sprite.setTexture(game->gfx.getTexture(grass_texture));
    grass_def->slippery = 0.7;

    Terrarium::blockid dirt_id = game->block_defs.add(dirt_def);
    Terrarium::blockid grass_id = game->block_defs.add(grass_def);

    std::shared_ptr<Terrarium::EntityPrefab> jumping_block_prefab = std::make_shared<Terrarium::EntityPrefab>();
    jumping_block_prefab->size = { 1, 1 };

    jumping_block_prefab->anims.setTexture(game->gfx.getTexture(dirt_texture));

    std::shared_ptr<Terrarium::EntityPrefab> player_prefab = std::make_shared<Terrarium::EntityPrefab>();
    player_prefab->size = { 1.5, 3 };

    player_prefab->anims.setTexture(game->gfx.getTexture(player_texture));

    Terrarium::entity_prefabid jumping_block_prefab_id = game->entity_mgr.addPrefab(jumping_block_prefab);
    Terrarium::entity_prefabid player_prefab_id = game->entity_mgr.addPrefab(player_prefab);

    Terrarium::entityid jumping_block_eid = game->entity_mgr.create(jumping_block_prefab_id);

    game->player.entity_id = game->entity_mgr.create(player_prefab_id);

    srand(time(NULL));

    for (unsigned int x = 0; x < game->world.getWidth(); ++x) {
        for (unsigned int y = game->world.getHeight()/10; y < game->world.getHeight(); ++y) {
            if (rand() % 2 == 0) {
                game->world.setBlock(x, y, dirt_id);
            }

            if (rand() % 4 == 0) {
                game->world.setBlock(x, y, grass_id);
            }

            if (rand() % 2 == 0) {
                game->world.setWall(x, y, dirt_id);
            }

            if (rand() % 4 == 0) {
                game->world.setWall(x, y, grass_id);
            }
        }
    }

    Terrarium::WorldRenderer world_renderer({ 1024, 800 }, 8);

    Terrarium::HotbarRenderer hotbar_renderer(Terrarium::Player::HOTBAR_SIZE, game->gfx);

    sf::Clock clock;

    float fps_show_timer = 1;

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
                            game->player.left = true;
                        break;

                        case sf::Keyboard::D:
                            game->player.right = true;
                        break;

                        case sf::Keyboard::Space:
                            game->player.jump = true;
                        break;

                        case sf::Keyboard::B:
                            game->player.hotbar_scroll = -1;
                        break;

                        case sf::Keyboard::N:
                            game->player.hotbar_scroll = 1;
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
                            game->player.left = false;
                        break;

                        case sf::Keyboard::D:
                            game->player.right = false;
                        break;

                        case sf::Keyboard::Space:
                            game->player.jump = false;
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
                            game->player.lmb = true;
                        break;

                        case sf::Mouse::Right:
                            game->player.rmb = true;
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
                            game->player.lmb = false;
                        break;

                        case sf::Mouse::Right:
                            game->player.rmb = false;
                        break;

                        default:
                        break;
                    }
                }
                break;

                case sf::Event::MouseWheelScrolled:
                    game->player.hotbar_scroll = -event.mouseWheelScroll.delta;
                break;

                default:
                break;
            }
        }

        // Update logic
        float dtime = clock.restart().asSeconds();

        fps_show_timer -= dtime;

        if (fps_show_timer < 0) {
            std::ostringstream oss;
            oss<<TITLE<<" fps: "<<1./dtime;

            fps_text.setString(oss.str());

            fps_show_timer = 1;
        }

        std::shared_ptr<Terrarium::Entity> jumping_block = game->entity_mgr.get(jumping_block_eid);

        if (jumping_block) {
            if (jumping_block->collision_info.blockd) {
                jumping_block->speed.x = 3;
                jumping_block->speed.y = -8;
            }
        }

        game->player.update(*game, dtime);

        game->entity_mgr.update(*game, dtime);

        sf::Vector2f camera_pos = game->player.getPosition(*game) - sf::Vector2f(game->camera.width/2, game->camera.height/2);
        game->camera.left = camera_pos.x;
        game->camera.top = camera_pos.y;

        sf::Vector2i mouse_pos_pixels = sf::Mouse::getPosition(window);
        sf::Vector2f mouse_pos = game->pixels_to_blocks.transformPoint(mouse_pos_pixels.x, mouse_pos_pixels.y);

        game->player.mouse_pos.x = game->camera.left + mouse_pos.x;
        game->player.mouse_pos.y = game->camera.top + mouse_pos.y;

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

        hotbar_renderer.render(window, *game, sf::Vector2f(32, 32));

        window.display();
    }

    // END TEST CODE

    return 0;
}
