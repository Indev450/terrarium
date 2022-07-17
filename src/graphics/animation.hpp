/*
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

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <string>

#include <cassert>

#include <SFML/Graphics/Rect.hpp>

namespace Terrarium {

    struct Animation {
        std::vector<sf::IntRect> frames;
        unsigned int current_frame = 0;
        float time_per_frame = 0;
        float timer = 0;

        std::string next;
        bool ended = false;

        void update(float dtime) {
            timer += dtime;

            if (timer > time_per_frame) {
                timer = 0;

                ++current_frame;

                if (current_frame == frames.size()) {
                    current_frame = 0;
                    ended = true;
                }
            }
        }

        inline sf::IntRect &getRect() {
            assert(frames.size() != 0);

            return frames[current_frame];
        }
    };

}

#endif
