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

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <string>

#include <cassert>

#include <SFML/Graphics.hpp>

#include "../utils/math.hpp"

namespace Terrarium {

    struct AnimationFrame {
        sf::IntRect rect;
        sf::Vector2f offset;
        float rotation = 0;
        sf::Vector2f rotation_center;
        sf::Vector2f scale = { 1.f, 1.f };
        sf::Vector2f scale_center;
    };

    struct Animation {
        std::vector<AnimationFrame> frames;

        enum Interpolation {
            None,
            Linear,
        } interpolation = None;

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

        inline const AnimationFrame &getFrame(unsigned i) const {
            i = i % frames.size();

            return frames[i];
        }

        inline const sf::IntRect &getRect() const {
            assert(frames.size() != 0);

            return frames[current_frame].rect;
        }

        inline sf::Transform getTransform(bool mirror) const {
            assert(frames.size() != 0);

            sf::Transform result;

            Interpolation use_interpolation = interpolation;

            // Do not interpolate between last frame and first if there is next
            // animation we will switch to
            if (current_frame == frames.size() - 1 && !next.empty()) {
                use_interpolation = None;
            }

            switch (use_interpolation) {
                case None:
                {
                    const auto &frame = frames[current_frame];

                    result.scale(frame.scale, frame.scale_center);
                    result.rotate(mirror ? frame.rotation : -frame.rotation, frame.rotation_center);
                    result.translate(mirror ? frame.offset.x : -frame.offset.x, frame.offset.y);
                }
                break;

                case Linear:
                {
                    const auto &frame = frames[current_frame];
                    const auto &next_frame = getFrame(current_frame+1);

                    float frame_rotation = mirror ? frame.rotation : -frame.rotation;
                    float next_rotation = mirror ? next_frame.rotation : -next_frame.rotation;

                    sf::Vector2f frame_offset = frame.offset;

                    if (mirror) {
                        frame_offset.x *= -1;
                    }

                    sf::Vector2f next_offset = next_frame.offset;

                    if (mirror) {
                        next_offset.x *= -1;
                    }

                    float t = timer / time_per_frame;

                    sf::Vector2f offset = lerp(frame_offset, next_offset, t);
                    float rotation = lerp(frame_rotation, next_rotation, t);
                    sf::Vector2f rotation_center = lerp(frame.rotation_center, next_frame.rotation_center, t);
                    sf::Vector2f scale = lerp(frame.scale, next_frame.scale, t);
                    sf::Vector2f scale_center = lerp(frame.scale_center, next_frame.scale_center, t);

                    result.scale(scale, scale_center);
                    result.rotate(rotation, rotation_center);
                    result.translate(offset);
                }
                break;
            }

            return result;
        }
    };

}

#endif
