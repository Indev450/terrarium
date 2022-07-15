#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>

#include <cassert>

#include <SFML/Graphics/Rect.hpp>

namespace Terrarium {

    struct Animation {
        std::vector<sf::IntRect> frames;
        unsigned int current_frame = 0;
        float time_per_frame = 0;
        float timer = 0;

        void update(float dtime) {
            timer += dtime;

            if (timer > time_per_frame) {
                timer = 0;

                ++current_frame;

                if (current_frame == frames.size()) {
                    current_frame = 0;
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
