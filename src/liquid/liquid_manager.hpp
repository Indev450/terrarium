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

#ifndef LIQUID_MANAGER_HPP
#define LIQUID_MANAGER_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "../utils/cells.hpp"

namespace Terrarium {

    struct LiquidCell {
        liquidid id = 0;
        float level = 0;
        bool updated = false;
    };

    class LiquidManager {
        Cells<LiquidCell> liquid;
        sf::VertexBuffer vertex_buff;

        void addQuad(std::vector<sf::Vertex> &vertices, const sf::IntRect &rect);

    public:
        LiquidManager& operator=(LiquidManager &&moved);

        LiquidManager(LiquidManager &&moved);

        LiquidManager() = default;

        LiquidManager(const LiquidManager &copy) = delete;
        LiquidManager& operator=(const LiquidManager &copy) = delete;

        void update(float dtime, const std::vector<sf::FloatRect> &areas);

        void render(sf::RenderTarget &target, const sf::FloatRect &camera);

        // Liquids save format:
        // u16                               width
        // u16                               height
        // LiquidCell                        liquid[width*height]
        void load(std::istream &file, GameState &game);

        void save(std::ostream &file) const;
    };

}

#endif
