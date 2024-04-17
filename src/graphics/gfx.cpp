#include "gfx.hpp"

namespace Terrarium {

    sf::Texture &TextureHolder::get(const std::string &desc) {
        // Created already
        if (!exists(desc)) {
            size_t lastplus = 0, plus = desc.find("+");

            std::string sub;

            sub = desc.substr(0, plus);

            sf::Texture &start_texture = ResourceHolder<sf::Texture>::get(sub);

            sf::Vector2u size = start_texture.getSize();

            combine_temp.create(size.x, size.y);

            combine_temp.clear(sf::Color::Transparent);

            combine_temp_sprite.setTexture(start_texture, true);

            combine_temp.draw(combine_temp_sprite);

            while (plus != desc.npos) {
                lastplus = plus+1;
                plus = desc.find("+", lastplus);

                sub = desc.substr(lastplus, plus - lastplus);

                combine_temp_sprite.setTexture(ResourceHolder<sf::Texture>::get(sub), true);

                combine_temp.draw(combine_temp_sprite);
            }

            combine_temp.display();

            insert(desc, std::make_shared<sf::Texture>(combine_temp.getTexture()));
        }

        return ResourceHolder<sf::Texture>::get(desc);
    }

}
