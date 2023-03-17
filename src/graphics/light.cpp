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

#include <iostream>
#include <chrono>

#include "light.hpp"

#include "../tile/tile.hpp"
#include "../utils/math.hpp"

namespace Terrarium {

    void LightCalculator::addQuad(std::vector<sf::Vertex> &arr, const sf::FloatRect &rect, const sf::Color &color) {
        arr.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top), color));
        arr.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top+rect.height), color));
        arr.push_back(sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top), color));

        arr.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top+rect.height), color));
        arr.push_back(sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top+rect.height), color));
        arr.push_back(sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top), color));
    }

    LightCalculator::LightCalculator(int width, int height):
        calculated(width, height),
        vertices(sf::PrimitiveType::Triangles, sf::VertexBuffer::Dynamic),
        input(width, height)
    {
        vertices.create(width*height*3*2);
    }

    void LightCalculator::resize(int width, int height) {
        // Dumb hack so player can't see borders loading
        width += (step+1)*2; height += (step+1)*2;
        input.resize(width, height);
        calculated.resize(width, height);
        vertices.create(width*height*3*2);
    }

    void LightCalculator::update(DebugInfo &debug_info, bool force) {
        if (!(needs_update || force)) {
            return;
        }

        auto start = std::chrono::steady_clock::now();

        // First we calculate light
        calculateLight();

        auto end = std::chrono::steady_clock::now();

        debug_info.light_calc_time = std::chrono::duration<double, std::milli>(end-start).count();

        // Then we add vertices to vertex array
        std::vector<sf::Vertex> arr;
        for (int x = 0; x < input.width; ++x) {
            for (int y = 0; y < input.height; ++y) {
                addQuad(arr,
                {
                    float(x*Tile::SIZE),
                    float(y*Tile::SIZE),
                    1.f*Tile::SIZE,
                    1.f*Tile::SIZE,
                }, calculated.at(x, y));
            }
        }

        // And then update vertex buffer with that information
        vertices.update(arr.data(), arr.size(), 0);

        needs_update = false;
    }

    bool LightCalculator::updatePosition(const sf::FloatRect &camera) {
        int new_world_x = stepify(floor(camera.left) - step, step);
        int new_world_y = stepify(floor(camera.top) - step, step);

        if (new_world_x != in_world_pos.x || new_world_y != in_world_pos.y) {
            in_world_pos.x = new_world_x;
            in_world_pos.y = new_world_y;

            needs_update = true;
        }

        setPosition(
            stepify((camera.left-step)*Tile::SIZE, step*Tile::SIZE) - camera.left*Tile::SIZE + step/2*Tile::SIZE,
            stepify((camera.top-step)*Tile::SIZE, step*Tile::SIZE) - camera.top*Tile::SIZE + step/2*Tile::SIZE);

        return needs_update;
    }

    void LightCalculator::updateLightInput(std::shared_ptr<GameState> game) {
        int start_x = static_cast<int>(stepify(floor(game->camera.left-step), step)) + step/2;
        int end_x = start_x + input.width;

        int start_y = static_cast<int>(stepify(floor(game->camera.top-step), step)) + step/2;
        int end_y = start_y + input.height;

        auto &world = game->world;
        auto &block_defs = game->block_defs;

        int natural_light_depth = game->world.getHeight()/4;
        auto natural_light = game->natural_light;

        for (int x = start_x; x < end_x; ++x) {
            for (int y = start_y; y < end_y; ++y) {
                int light_calc_x = x - start_x;
                int light_calc_y = y - start_y;

                const Tile *tile = world.getTile(x, y);

                auto &inp = input.at(light_calc_x, light_calc_y);

                inp.blocks_light = true;
                inp.light = 0;

                // Out of world bounds, don't do anything
                if (tile == nullptr) {
                    continue;
                }

                // No blocks, use natural light
                if (tile->fg == 0 && tile->bg == 0) {
                    inp.blocks_light = false;
                    inp.light = (y < natural_light_depth ? natural_light : 0);
                    continue;
                }

                BlockDef &fg = block_defs.getOrUnknown(tile->fg);
                BlockDef &bg = block_defs.getOrUnknown(tile->bg);

                // Background doesn't block natural light, but we can't be sure
                // about foreground yet
                if (tile->bg == 0 || !bg.blocks_light) {
                    inp.light = std::max<uint8_t>((y < natural_light_depth ? natural_light : 0), bg.light);
                } else if (tile->bg != 0) {
                    // Background does block natural light but it might emit
                    // light itself
                    inp.light = bg.light;
                }

                // There is foreground block...
                if (tile->fg != 0) {
                    if (fg.blocks_light) {
                        // It blocks light, replace any light from background
                        inp.light = fg.light;
                        inp.blocks_light = true;

                    } else {
                        // It doesn't block light, so we need to select max
                        // light value
                        inp.blocks_light = false;
                        inp.light = std::max(inp.light, fg.light);
                    }
                } else {
                    // There is no block at foreground, don't block light and
                    // use light value from background/natural light
                    inp.blocks_light = false;
                }
            }
        }
    }

    void LightCalculator::calculateLight() {
        // First set all values at light sources. This would greatly help
        // improve perfomance for cases where a lot of light sources located
        // nearby, such as natural light
        for (int y = 0; y < input.height; ++y) {
            for (int x = 0; x < input.width; ++x) {
                calculated.at(x, y) = sf::Color(0, 0, 0, 255-input.at(x, y).light);
            }
        }

        // Now recursively calculate light at every light source...
        for (int y = 0; y < input.height; ++y) {
            for (int x = 0; x < input.width; ++x) {
                auto &inp = input.at(x, y);
                auto &calc = calculated.at(x, y);

                // ...But do that if previously calculated light value is lower
                // than light source from input. Again, this greatly improves
                // perfomance.
                if (inp.light && 255-calc.a <= inp.light) {
                    lightSource(x, y, inp.light);
                }
            }
        }
    }

    void LightCalculator::lightSource(int x, int y, uint8_t intensity, int ox, int oy, bool recursion) {
        // Too far from light source
        if (ox*ox+oy*oy > LIGHT_RECURSION_LIMIT*LIGHT_RECURSION_LIMIT || intensity <= LIGHT_MIN_VALUE) {
            return;
        }

        // Out of range for calculations
        if (!calculated.isInRange(x+ox, y+oy)) {
            return;
        }

        auto &calc = calculated.at(x+ox, y+oy);
        auto &inp = input.at(x+ox, y+oy);

        // Light at this spot already was calculated and it isn't less that
        // source. Extra check if that is recursive call is done because light
        // at light source will always be same as calculated, as it was set in
        // calculateLight()
        if (255-calc.a >= intensity && recursion) {
            return;
        }

        calc = sf::Color(0, 0, 0, 255-intensity);

        for (int nx = -1; nx <= 1; ++nx) {
            for (int ny = -1; ny <= 1; ++ny) {
                // Don't re-run algorithm for same block
                if (!(nx || ny)) {
                    continue;
                }

                // New intensity is lower. If light spreads through blocks that
                // block light or it spreads diagonally, it becomes even lower
                uint8_t new_intensity = uint8_t(float(intensity)
                            * (inp.blocks_light ? LIGHT_DROPOFF_BLOCK : 1.f) * LIGHT_DROPOFF_DEFAULT
                            * (((nx && ny) ? LIGHT_DROPOFF_DIAGONAL : 1.f)));

                lightSource(x, y, new_intensity, ox+nx, oy+ny, true);
            }
        }
    }

    void LightCalculator::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(vertices, states);
    }

}
