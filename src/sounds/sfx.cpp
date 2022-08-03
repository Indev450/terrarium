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

#include <list>

#include "sfx.hpp"
#include "../game.hpp"

namespace Terrarium {
    void Sfx::update(GameState &game) {
        std::list<sound_handle> to_delete;

        for (auto it = playing_sounds.begin(); it != playing_sounds.end(); ++it) {
            it->second->update(game);

            if (!it->second->isPlaying()) {
                to_delete.push_back(it->first);
            }
        }

        for (auto handle: to_delete) {
            playing_sounds.del(handle);
        }
    }

    sound_handle Sfx::playSound(
        const std::string &name,
        float volume,
        float pitch) {

        auto sound = std::make_unique<sf::Sound>(sounds.get(name));

        sound->setPitch(pitch);

        auto sound_ctl = std::make_shared<SoundController>(std::move(sound));
        sound_ctl->setVolume(volume);

        sound_handle handle = playing_sounds.add(sound_ctl);

        if (handle != 0) {
            sound_ctl->play();
        }

        return handle;
    }

    sound_handle Sfx::playSoundAt(
        const std::string &name,
        float volume,
        float pitch,
        const sf::Vector2f &position) {

        auto sound = std::make_unique<sf::Sound>(sounds.get(name));

        sound->setPitch(pitch);

        auto sound_ctl = std::make_shared<SoundAtSpot>(std::move(sound), position);
        sound_ctl->setVolume(volume);

        sound_handle handle = playing_sounds.add(sound_ctl);

        if (handle != 0) {
            sound_ctl->play();
        }

        return handle;
    }

    sound_handle Sfx::playSoundAt(
        const std::string &name,
        float volume,
        float pitch,
        std::weak_ptr<Entity> entity_ref) {

        auto sound = std::make_unique<sf::Sound>(sounds.get(name));

        sound->setPitch(pitch);

        auto sound_ctl = std::make_shared<SoundAtEntity>(std::move(sound), entity_ref);
        sound_ctl->setVolume(volume);

        sound_handle handle = playing_sounds.add(sound_ctl);

        if (handle != 0) {
            sound_ctl->play();
        }

        return handle;
    }

    void Sfx::stopSound(sound_handle handle) {
        auto sound_ctl = playing_sounds.get(handle);

        if (sound_ctl) {
            sound_ctl->stop();
        }
    }
}
