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
        std::string current_name;
        sf::Sprite sprite;

    public:
        inline void setTexture(const sf::Texture &texture) {
            sprite.setTexture(texture);
        }

        inline const std::string &get() {
            return current_name;
        }

        void add(const std::string &name, const Animation &anim) {
            anims[name] = anim;
        }

        void set(const std::string &name, bool restart = false) {
            auto pair = anims.find(name);

            current_name = name;

            if (pair != anims.end()) {
                current = &pair->second;

                if (restart) {
                    current->current_frame = 0;
                    current->timer = 0;
                }
            } else {
                current = nullptr;
            }
        }

        void update(float dtime) {
            if (current != nullptr) {
                current->update(dtime);

                if (!current->next.empty() && current->ended) {
                    set(current->next);

                    if (current == nullptr)
                        return;
                }

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
