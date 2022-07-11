#ifndef GFX_HPP
#define GFX_HPP

#include <unordered_map>
#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>

namespace Terrarium {

    // Structure with data required to draw stuff
    struct Gfx {
        sf::Font font;

        bool loadTexture(const std::string &path) {
            if (textures.find(path) != textures.end()) {
                return true;
            }

            sf::Texture texture;

            if (!texture.loadFromFile(path)) {
                return false;
            }

            textures[path] = texture;

            return true;
        }

        const sf::Texture &getTexture(const std::string &path) {
            if (textures.find(path) != textures.end()) {
                return textures[path];
            }

            std::cerr<<"Terrarium::Gfx::getTexture: texture '"<<path<<"' not found"<<std::endl;
            return texture_not_found;
        }

    private:
        std::unordered_map<std::string, sf::Texture> textures;
        sf::Texture texture_not_found;
    };

}

#endif
