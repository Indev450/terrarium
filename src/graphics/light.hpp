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

#ifndef GRAPHICS_LIGHT_HPP
#define GRAPHICS_LIGHT_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../game.hpp"

#define LIGHT_RECURSION_LIMIT 32
#define LIGHT_MIN_VALUE 10
#define LIGHT_DROPOFF_BLOCK 0.75f
#define LIGHT_DROPOFF_DEFAULT 0.9f
#define LIGHT_DROPOFF_DIAGONAL 0.9f

namespace Terrarium {

    inline void setMaxLight(sf::Vector3i &into, const sf::Vector3i &from) {
        into.x = std::max(into.x, from.x);
        into.y = std::max(into.y, from.y);
        into.z = std::max(into.z, from.z);
    }

    inline void setMaxLight(sf::Color &into, const sf::Vector3i &from) {
        into.r = std::max<uint8_t>(into.r, from.x);
        into.g = std::max<uint8_t>(into.g, from.y);
        into.b = std::max<uint8_t>(into.b, from.z);
    }

    inline bool isBrighter(const sf::Vector3i &first, const sf::Vector3i &second) {
        return first.x >= second.x && first.y >= second.y && first.z >= second.z;
    }

    inline bool isBrighter(const sf::Color &first, const sf::Vector3i &second) {
        return first.r >= second.x && first.g >= second.y && first.b >= second.z;
    }

    inline int getMaxLight(const sf::Vector3i &from) {
        return std::max(std::max(from.x, from.y), from.z);
    }

    inline int getMaxLight(const sf::Color &from) {
        return std::max(std::max(from.r, from.g), from.b);
    }

    inline bool hasLight(const sf::Vector3i &val) {
        return val.x || val.y || val.z;
    }

    struct LightInput {
        bool blocks_light = false;
        sf::Vector3i light;
    };

    struct DynamicLightInput {
        sf::Vector2i position;
        sf::Vector3i light;
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

        inline bool isInRange(int x, int y) {
            return x >= 0 && x < width && y >= 0 && y < height;
        }

        inline T &at(int x, int y) {
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
        LightGrid<sf::Color> calculated_static;
        sf::Texture shadow;
        sf::RectangleShape shadow_rect;

        bool dither_enabled = true;
        std::unique_ptr<sf::Shader> shadow_shader;

        sf::Vector2i in_world_pos = { 0, 0 };
        bool needs_update = true;

    public:
        LightGrid<LightInput> input;
        std::vector<DynamicLightInput> input_dynamic;
        int step = LIGHT_RECURSION_LIMIT;

        LightCalculator(int width, int height);

        void resize(int width, int height);

        void update(DebugInfo &debug_info, bool force = false);

        bool updatePosition(const sf::FloatRect &camera);

        void updateLightInput(std::shared_ptr<GameState> game);

        void addDynamicLight(const DynamicLightInput &inp);

        void setSmooth(bool smooth);

        void setDither(bool dither);

    private:
        void calculateLight();

        void lightSource(int x, int y, const sf::Vector3i &intensity, int ox = 0, int oy = 0, bool recursion = false);

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

}

#endif
