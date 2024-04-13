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

#include <cmath>

#include "sound.hpp"
#include "../game.hpp"


namespace Terrarium {

    SoundController::SoundController(std::unique_ptr<sf::Sound> _sound):
        sound(std::move(_sound))
    {}


    SoundAtSpot::SoundAtSpot(std::unique_ptr<sf::Sound> _sound, const sf::Vector2f &_position):
        SoundController(std::move(_sound)), position(_position)
    {
        // FIXME - this is needed because at the moment sound is created it doesn't have positioning
        // info yet so it may play at full volume for 1 frame. It is fixed on next update call but
        // it probably needs to do it the moment sound is created instead
        sound->setVolume(0);
    }

    void SoundAtSpot::update(GameState &game) {
        sf::Vector2f player_position = game.player->getPosition();

        float dx = position.x - player_position.x;
        float dy = position.y - player_position.y;

        float dist = sqrtf(dx*dx + dy*dy);

        // Very rare case
        if (dist != 0) {
            float dist_volume = std::min(1.f, fade_dist/dist);

            if (dist_volume < 0.1) {
                dist_volume = 0;
            }

            sound->setVolume(volume * dist_volume);
        } else {
            sound->setVolume(volume);
        }
    }

    SoundAtEntity::SoundAtEntity(std::unique_ptr<sf::Sound> _sound, std::weak_ptr<Entity> _entity_ref):
        SoundAtSpot(std::move(_sound), sf::Vector2f(0, 0)), entity_ref(_entity_ref) {

        updatePosition();
    }

    void SoundAtEntity::update(GameState &game) {
        updatePosition();

        SoundAtSpot::update(game);
    }

    void SoundAtEntity::updatePosition() {
        auto entity = entity_ref.lock();

        if (entity) {
            setPosition(sf::Vector2f(
                entity->hitbox.left + entity->hitbox.width/2,
                entity->hitbox.top + entity->hitbox.height/2));
        }
    }

}
