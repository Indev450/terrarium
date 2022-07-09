#ifndef ANIMATION_MAP_HPP
#define ANIMATION_MAP_HPP

#include <unordered_map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "animation.hpp"

namespace Terrarium {

    class AnimationMap {
        std::unordered_map<std::string, Animation> anims;
        Animation *current = nullptr;
        sf::Sprite sprite;

    public:
        inline void setTexture(const sf::Texture &texture) {
            sprite.setTexture(texture);
        }

        void add(const std::string &name, const Animation &anim) {
            anims[name] = anim;
        }

        void set(const std::string &name) {
            auto pair = anims.find(name);

            if (pair != anims.end()) {
                current = &pair->second;
            }

            current = nullptr;
        }

        void update(float dtime) {
            if (current != nullptr) {
                current->update(dtime);

                sprite.setTextureRect(current->getRect());
            }
        }

        void draw(sf::RenderTarget &target, const sf::Vector2f &position) {
            sprite.setPosition(position);

            target.draw(sprite);
        }
    };

}

#endif
