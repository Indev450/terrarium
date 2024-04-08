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

#ifndef SFX_HPP
#define SFX_HPP

#include <cstdint>

#include <SFML/Audio.hpp>

#include "sound.hpp"
#include "../utils/auto_id_map.hpp"
#include "../utils/resource_holder.hpp"

namespace Terrarium {

    typedef uint8_t sound_handle;

    class GameState;

    struct Sfx {
        AutoIDMap<sound_handle, SoundController> playing_sounds;
        ResourceHolder<sf::SoundBuffer> sounds;
        ResourceHolder<sf::Music> music;

        sf::Music *music_playing = nullptr;
        sf::Music *music_fading = nullptr;

        float fade_volume = 0.0;

        float music_volume = 1.0;
        float sounds_volume = 1.0;

        void update(GameState &game, float dtime);

        void playMusic(const std::string &name, bool loop = false);

        // True if any music file is playing right now
        bool isMusicPlaying();

        void stopMusic();

        sound_handle playSound(
            const std::string &name,
            float volume,
            float pitch);

        sound_handle playSoundAt(
            const std::string &name,
            float volume,
            float pitch,
            const sf::Vector2f &position);

        sound_handle playSoundAt(
            const std::string &name,
            float volume,
            float pitch,
            std::weak_ptr<Entity> entity_ref);

        void stopSound(sound_handle handle);

    };

}

#endif
