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

#ifndef DEBUG_OVERLAY_HPP
#define DEBUG_OVERLAY_HPP

#include <SFML/Graphics.hpp>

namespace Terrarium {

    struct DebugInfo {
        float fps = 0;
        float world_redraw_time = 0;
        float light_calc_time = 0;
        float time_of_day = 0;
        int phase_of_day = 0;
        int total_phases = 0;

        void updateFps(float dtime) {
            fps_update_timer -= dtime;

            if (fps_update_timer < 0) {
                fps_update_timer = 1;
                fps = 1./dtime;
            }
        }

    private:
        float fps_update_timer = 1;
    };

    class DebugOverlay: public sf::Drawable, public sf::Transformable {
        sf::Text text;
        sf::RectangleShape background;

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    public:
        DebugOverlay(const sf::Font &font);

        void update(const DebugInfo &info);
    };

}

#endif
