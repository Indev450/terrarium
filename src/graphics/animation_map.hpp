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

#ifndef ANIMATION_MAP_HPP
#define ANIMATION_MAP_HPP

#include <unordered_map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "animation.hpp"

namespace Terrarium {

    class AnimationMap: public sf::Drawable, public sf::Transformable {
        std::unordered_map<std::string, Animation> anims;
        Animation *current = nullptr;
        std::string current_name;
        sf::Sprite sprite;
        bool mirror = false;

    public:
        sf::Vector2f draw_size;

    public:
        inline void setTexture(const sf::Texture &texture) {
            sprite.setTexture(texture, true);
        }

        inline const std::string &get() {
            return current_name;
        }

        inline void setMirror(bool mirror) {
            this->mirror = mirror;
        }

        void add(const std::string &name, const Animation &anim) {
            anims[name] = anim;
        }

        void set(const std::string &name, bool restart = false) {
            auto pair = anims.find(name);

            restart = restart || name != current_name;

            current_name = name;

            if (pair != anims.end()) {
                current = &pair->second;

                if (restart) {
                    current->current_frame = 0;
                    current->timer = 0;
                    current->ended = false;
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

                sf::IntRect rect = current->getRect();

                if (mirror) {
                    rect.left += rect.width;
                    rect.width *= -1;
                }

                sprite.setTextureRect(rect);
            }
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
            sf::IntRect rect = sprite.getTextureRect();

            float scale_x = draw_size.x / rect.width;
            float scale_y = draw_size.y / rect.height;

            states.transform *= getTransform();

            states.transform.scale(scale_x, scale_y);

            target.draw(sprite, states);
        }
    };

}

#endif
