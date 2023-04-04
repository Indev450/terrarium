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

#ifndef SFX_SOUND_HPP
#define SFX_SOUND_HPP

#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Audio.hpp>

#include "../entity/entity.hpp"

namespace Terrarium {

    class GameState;

    // Just a sound
    class SoundController {
    protected:
        std::unique_ptr<sf::Sound> sound;

    public:
        SoundController(std::unique_ptr<sf::Sound> _sound);

        virtual void update(GameState &game) {};

        virtual void setVolume(float new_volume) {
            sound->setVolume(new_volume);
        }

        void play() { sound->play(); }

        void stop() { sound->stop(); }

        bool isPlaying() {
            return sound->getStatus() == sf::SoundSource::Playing;
        }
    };

    // Sound playing at specified world position
    class SoundAtSpot: public SoundController {
        sf::Vector2f position;

        float fade_dist = 10;

        float volume = 1.0;

    public:
        SoundAtSpot(std::unique_ptr<sf::Sound> _sound, const sf::Vector2f &_position);

        void update(GameState &game) override;

        void setVolume(float new_volume) override {
            volume = new_volume;
        }

        void setPosition(const sf::Vector2f &new_position) {
            position = new_position;
        }

        void setFadeDistance(float new_fade_dist) {
            fade_dist = new_fade_dist;
        }
    };

    // Sound attached to entity
    class SoundAtEntity: public SoundAtSpot {
        std::weak_ptr<Entity> entity_ref;

        void updatePosition();

    public:
        SoundAtEntity(std::unique_ptr<sf::Sound> _sound, std::weak_ptr<Entity> _entity_ref);

        void update(GameState &game) override;
    };

}

#endif
