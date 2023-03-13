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

#ifndef GRAPHICS_LIGHT_HPP
#define GRAPHICS_LIGHT_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <memory>

#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "../game.hpp"

#define LIGHT_RECURSION_LIMIT 16
#define LIGHT_MIN_VALUE 8
#define LIGHT_DROPOFF_BLOCK 0.75
#define LIGHT_DROPOFF_DEFAULT 0.85
#define LIGHT_DROPOFF_DIAGONAL 0.9

namespace Terrarium {

    struct LightInput {
        bool blocks_light = false;
        uint8_t light = 0;
    };

    template <class T>
    struct LightGrid {
        std::vector<T> grid;
        int width = 0;
        int height = 0;

        LightGrid(int _width, int _height) {
            resize(_width, _height);
        }

        void resize(int _width, int _height) {
            grid.resize(_width*_height);
            width = _width;
            height = _height;
        }

        bool isInRange(int x, int y) {
            return x >= 0 && x < width && y >= 0 && y < height;
        }

        inline T &at(int x, int y) {
            // Uhhh this is kinda wrong but works?
            if (!isInRange(x, y)) {
                std::ostringstream errormsg;
                errormsg<<"light grid index ("<<x<<", "<<y<<") is out of range "
                        <<"("<<width<<", "<<height<<")";
                throw std::range_error(errormsg.str());
            }

            return grid[y*width + x];
        }
    };

    class LightCalculator: public sf::Drawable, public sf::Transformable {
        LightGrid<sf::Color> calculated;
        sf::VertexBuffer vertices;

        sf::Vector2i in_world_pos = { 0, 0 };
        bool needs_update = true;

        // Helper function that hides transformation from rect to 2 triangles.
        static void addQuad(std::vector<sf::Vertex> &arr, const sf::FloatRect &rect, const sf::Color &color);

    public:
        LightGrid<LightInput> input;
        int step = LIGHT_RECURSION_LIMIT;

        LightCalculator(int width, int height);

        void resize(int width, int height);

        void update(bool force = false);

        bool updatePosition(const sf::FloatRect &camera);

        void updateLightInput(std::shared_ptr<GameState> game);

    private:
        void calculateLight();

        void lightSource(int x, int y, uint8_t intensity, int ox = 0, int oy = 0, bool recursion = false);

        void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    };

}

#endif
