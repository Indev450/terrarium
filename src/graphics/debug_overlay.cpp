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

#include <sstream>
#include <iomanip>

#include "debug_overlay.hpp"

namespace Terrarium {

    DebugOverlay::DebugOverlay(const sf::Font &font):
        text("", font, 24)
    {
        background.setFillColor(sf::Color(128, 128, 128));
        background.setOutlineColor(sf::Color(64, 64, 64));
        background.setOutlineThickness(2.f);
        background.setPosition(2, 2);

        text.setFillColor(sf::Color::White);
        text.setPosition(4, 4);
    }

    void DebugOverlay::update(const DebugInfo &info) {
        std::ostringstream oss;
        oss<<std::setw(2)<<std::setprecision(4);
        oss<<"Fps: "<<info.fps<<std::endl;
        oss<<"World redraw time: "<<info.world_redraw_time<<" ms"<<std::endl;
        oss<<"Light calculation time: "<<info.light_calc_time<<" ms"<<std::endl;
        oss<<"Time of day: "<<info.time_of_day<<std::endl;
        oss<<"Phase of day: "<<info.phase_of_day+1<<"/"<<info.total_phases<<std::endl;

        text.setString(oss.str());

        sf::FloatRect text_bounds = text.getLocalBounds();
        background.setSize(sf::Vector2f(std::max(text_bounds.width, 400.f), text_bounds.height));
    }

    void DebugOverlay::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(background, states);
        target.draw(text, states);
    }

}
