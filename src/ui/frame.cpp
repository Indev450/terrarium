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

#include "frame.hpp"
#include "../game.hpp"

namespace Terrarium {

    UIFrame::UIFrame(unsigned width, unsigned height, const std::string &_id):
        id(_id) {
        rtexture.create(width, height);
    }

    bool UIFrame::click(GameState &game, const sf::Vector2f &position) {
        // Relative position
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);
        sf::Vector2f size(rtexture.getSize());

        if (rpos.x < 0 || rpos.x > size.x || rpos.y < 0 || rpos.y > size.y) {
            return false;
        }

        return game.client_modding_interface->ui_click(id, rpos);
    }

    bool UIFrame::scroll(GameState &game, const sf::Vector2f &position, float delta) {
        // Relative position
        sf::Vector2f rpos = getInverseTransform().transformPoint(position);
        sf::Vector2f size(rtexture.getSize());

        if (rpos.x < 0 || rpos.x > size.x || rpos.y < 0 || rpos.y > size.y) {
            return false;
        }

        return game.client_modding_interface->ui_scroll(id, rpos, delta);
    }

    void UIFrame::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        game.client_modding_interface->ui_render(id);

        rtexture.display();

        sprite.setTexture(rtexture.getTexture(), true);

        sf::Transform combined_transform = getTransform() * parent_transform;

        target.draw(sprite, combined_transform);
    }
}
