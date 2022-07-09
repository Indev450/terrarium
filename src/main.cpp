#include <iostream>
#include <sstream>
#include <memory>
#include <cstdlib> // for rand()
#include <ctime> // for time()

#include <terrarium_config.hpp>

#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "graphics/world_renderer.hpp"

const char *TITLE = "TerrariumEngine";

int main()
{
    std::cout<<"TerrariumEngine version "<<TERRARIUM_VERSION_MAJOR<<'.'<<TERRARIUM_VERSION_MINOR<<std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 640), TITLE);
    window.setVerticalSyncEnabled(true);

    // BEGIN TEST CODE

    Terrarium::GameState game(512, 512);

    sf::Texture dirt_texture;

    if (!dirt_texture.loadFromFile("assets/dirt.png")) {
        std::cerr<<"Cannot load dirt texture"<<std::endl;

        return -1;
    }

    sf::Texture grass_texture;

    if (!grass_texture.loadFromFile("assets/grass.png")) {
        std::cerr<<"Cannot load grass texture"<<std::endl;

        return -1;
    }

    sf::Texture player_texture;

    if (!player_texture.loadFromFile("assets/player.png")) {
        std::cerr<<"Cannot load player texture"<<std::endl;

        return -1;
    }

    std::unique_ptr<Terrarium::BlockDef> dirt_def(new Terrarium::BlockDef);

    dirt_def->sprite.setTexture(dirt_texture);
    dirt_def->slippery = 0.85;

    std::unique_ptr<Terrarium::BlockDef> grass_def(new Terrarium::BlockDef);

    grass_def->sprite.setTexture(grass_texture);
    grass_def->slippery = 0.7;

    Terrarium::blockid dirt_id = game.block_defs.add(std::move(dirt_def));
    Terrarium::blockid grass_id = game.block_defs.add(std::move(grass_def));

    std::unique_ptr<Terrarium::EntityPrefab> jumping_block_prefab = std::make_unique<Terrarium::EntityPrefab>();
    jumping_block_prefab->size = { 16, 16 };

    jumping_block_prefab->anims.setTexture(dirt_texture);

    std::unique_ptr<Terrarium::EntityPrefab> player_prefab = std::make_unique<Terrarium::EntityPrefab>();
    player_prefab->size = { 24, 48 };

    player_prefab->anims.setTexture(player_texture);

    Terrarium::entity_prefabid jumping_block_prefab_id = game.entity_mgr.addPrefab(std::move(jumping_block_prefab));
    Terrarium::entity_prefabid player_prefab_id = game.entity_mgr.addPrefab(std::move(player_prefab));

    Terrarium::entityid jumping_block_eid = game.entity_mgr.create(jumping_block_prefab_id);

    game.player.entity_id = game.entity_mgr.create(player_prefab_id);

    srand(time(NULL));

    for (unsigned int x = 0; x < game.world.getWidth(); ++x) {
        for (unsigned int y = game.world.getHeight()/10; y < game.world.getHeight(); ++y) {
            if (rand() % 2 == 0) {
                game.world.setBlock(x, y, dirt_id);
            }

            if (rand() % 4 == 0) {
                game.world.setBlock(x, y, grass_id);
            }

            if (rand() % 2 == 0) {
                game.world.setWall(x, y, dirt_id);
            }

            if (rand() % 4 == 0) {
                game.world.setWall(x, y, grass_id);
            }
        }
    }

    Terrarium::WorldRenderer world_renderer({ 1024, 800 }, 8);

    sf::Clock clock;

    float fps_show_timer = 1;

    sf::Font font;

    if (!font.loadFromFile("assets/dpcomic.ttf")) {
        std::cerr<<"Cannot load font"<<std::endl;
    }

    sf::Text fps_text;
    fps_text.setCharacterSize(18);
    fps_text.setPosition(12, 12);
    fps_text.setFillColor(sf::Color::Black);
    fps_text.setFont(font);

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
                    game.camera.width = event.size.width;
                    game.camera.height = event.size.height;

                    world_renderer.setScreenSize({ event.size.width + Terrarium::Tile::SIZE, event.size.height + Terrarium::Tile::SIZE });
                }
                break;

                case sf::Event::KeyPressed:
                {
                    switch (event.key.code) {
                        case sf::Keyboard::A:
                            game.player.left = true;
                        break;

                        case sf::Keyboard::D:
                            game.player.right = true;
                        break;

                        case sf::Keyboard::Space:
                            game.player.jump = true;
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
                            game.player.left = false;
                        break;

                        case sf::Keyboard::D:
                            game.player.right = false;
                        break;

                        case sf::Keyboard::Space:
                            game.player.jump = false;
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
                            game.player.lmb = true;
                        break;

                        case sf::Mouse::Right:
                            game.player.rmb = true;
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
                            game.player.lmb = false;
                        break;

                        case sf::Mouse::Right:
                            game.player.rmb = false;
                        break;

                        default:
                        break;
                    }
                }
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

        Terrarium::Entity *jumping_block = game.entity_mgr.get(jumping_block_eid);

        if (jumping_block != nullptr) {
            if (jumping_block->collision_info.blockd) {
                jumping_block->speed.x = Terrarium::Tile::SIZE*3;
                jumping_block->speed.y = -Terrarium::Tile::SIZE*8;
            }
        }

        game.player.update(game, dtime);

        game.entity_mgr.update(game, dtime);

        sf::Vector2f camera_pos = game.player.getPosition(game) - sf::Vector2f(game.camera.width/2, game.camera.height/2);
        game.camera.left = camera_pos.x;
        game.camera.top = camera_pos.y;

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        game.player.mouse_pos.x = game.camera.left + mouse_pos.x;
        game.player.mouse_pos.y = game.camera.top + mouse_pos.y;

        // Draw
        window.clear(sf::Color::Blue);

        world_renderer.updatePosition(game.camera);
        world_renderer.update(game);
        world_renderer.render(window);

        game.entity_mgr.draw(game, window);

        window.draw(fps_text);

        window.display();
    }

    // END TEST CODE

    return 0;
}
