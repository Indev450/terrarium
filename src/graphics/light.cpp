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

#include <iostream>
#include <chrono>

#include "light.hpp"

#include "../tile/tile.hpp"
#include "../utils/math.hpp"

namespace Terrarium {

    LightCalculator::LightCalculator(int width, int height):
        calculated(width, height),
        calculated_static(width, height),
        input(width, height)
    {
        shadow.create(width, height);

        shadow_rect.setTexture(&shadow);
        shadow_rect.setSize({ float(width), float(height) });

        shadow_shader = std::make_unique<sf::Shader>();

        if (!shadow_shader->loadFromFile("assets/dither.frag", sf::Shader::Fragment))
            shadow_shader.reset();
        else
            std::cout<<"Dither shader loaded successfully"<<std::endl;
    }

    void LightCalculator::resize(int width, int height) {
        // Dumb hack so player can't see borders loading
        width += (step+1)*2; height += (step+1)*2;
        input.resize(width, height);
        calculated.resize(width, height);
        calculated_static.resize(width, height);
        shadow.create(width, height);
        shadow_rect.setTexture(&shadow, true);
        shadow_rect.setSize({ float(width), float(height) });
    }

    void LightCalculator::update(DebugInfo &debug_info, bool force) {
        if (needs_update || force) {
            auto start = std::chrono::steady_clock::now();

            // First we calculate light
            calculateLight();

            auto end = std::chrono::steady_clock::now();

            debug_info.light_calc_time = std::chrono::duration<double, std::milli>(end-start).count();

            calculated_static = calculated;

            needs_update = false;
        }

        calculated = calculated_static;

        for (auto & inp : input_dynamic) {
            lightSource(inp.position.x - in_world_pos.x - step/2, inp.position.y - in_world_pos.y - step/2, inp.light);
        }

        input_dynamic.clear();

        shadow.update((uint8_t*)calculated.grid.data());
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
        auto natural_light_base = sf::Vector3i(game->natural_light,game->natural_light,game->natural_light);

        for (int x = start_x; x < end_x; ++x) {
            for (int y = start_y; y < end_y; ++y) {
                int light_calc_x = x - start_x;
                int light_calc_y = y - start_y;

                const Tile *tile = world.getTile(x, y);

                auto &inp = input.at(light_calc_x, light_calc_y);

                inp.blocks_light = true;
                inp.light = { 0, 0, 0 };

                // Out of world bounds, don't do anything
                if (tile == nullptr) {
                    continue;
                }

                sf::Vector3i natural_light;

                if (y < natural_light_depth) {
                    natural_light = natural_light_base;
                }

                // No blocks, use natural light
                if (tile->fg.id == 0 && tile->bg.id == 0) {
                    inp.blocks_light = false;
                    inp.light = natural_light;
                    continue;
                }

                BlockDef &fg = block_defs.getOrUnknown(tile->fg.id);
                BlockDef &bg = block_defs.getOrUnknown(tile->bg.id);

                // Background doesn't block natural light, but we can't be sure
                // about foreground yet
                if (!tile->bg.id || !bg.blocks_light) {
                    setMaxLight(inp.light, natural_light);
                }

                // Background might emit light itself
                if (tile->bg) {
                    setMaxLight(inp.light, bg.light);
                }

                // There is foreground block...
                if (tile->fg) {
                    if (!fg.blocks_light) {
                        setMaxLight(inp.light, fg.light);
                    } else {
                        inp.light = fg.light;
                    }

                    inp.blocks_light = fg.blocks_light;
                } else {
                    // There is no block at foreground, don't block light and
                    // use light value from background/natural light
                    inp.blocks_light = false;
                }
            }
        }
    }

    void LightCalculator::addDynamicLight(const DynamicLightInput &inp) {
        input_dynamic.push_back(inp);
    }

    void LightCalculator::calculateLight() {
        // First set all values at light sources. This would greatly help
        // improve perfomance for cases where a lot of light sources located
        // nearby, such as natural light
        for (int y = 0; y < input.height; ++y) {
            for (int x = 0; x < input.width; ++x) {
                auto &inp = input.at(x, y);
                calculated.at(x, y) = sf::Color(inp.light.x, inp.light.y, inp.light.z, 255-getMaxLight(inp.light));
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
                if (hasLight(inp.light) && isBrighter(calc, inp.light)) {
                    lightSource(x, y, inp.light);
                }
            }
        }
    }

    void LightCalculator::lightSource(int x, int y, const sf::Vector3i &intensity, int ox, int oy, bool recursion) {
        // Too far from light source
        if (ox*ox+oy*oy > LIGHT_RECURSION_LIMIT*LIGHT_RECURSION_LIMIT || getMaxLight(intensity) <= LIGHT_MIN_VALUE) {
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
        if (isBrighter(calc, intensity) && recursion) {
            return;
        }

        setMaxLight(calc, intensity);
        calc.a = 255 - getMaxLight(calc);

        for (int nx = -1; nx <= 1; ++nx) {
            for (int ny = -1; ny <= 1; ++ny) {
                // Don't re-run algorithm for same block
                if (!(nx || ny)) {
                    continue;
                }

                // New intensity is lower. If light spreads through blocks that
                // block light or it spreads diagonally, it becomes even lower
                sf::Vector3f new_intensity(intensity);

                new_intensity *= LIGHT_DROPOFF_DEFAULT;
                if (inp.blocks_light) new_intensity *= LIGHT_DROPOFF_BLOCK;
                if (nx && ny) new_intensity *= LIGHT_DROPOFF_DIAGONAL;

                if (ox*nx >= 0 && oy*ny >= 0)
                    lightSource(x, y, sf::Vector3i(new_intensity), ox+nx, oy+ny, true);
            }
        }
    }

    void LightCalculator::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.transform.scale(sf::Vector2f(Tile::SIZE, Tile::SIZE));
        states.blendMode = sf::BlendMultiply;

        if (dither_enabled && shadow_shader)
            states.shader = shadow_shader.get();

        target.draw(shadow_rect, states);
    }

    void LightCalculator::setSmooth(bool smooth) {
        shadow.setSmooth(smooth);
    }

    void LightCalculator::setDither(bool dither) {
        dither_enabled = dither;

        if (dither && !shadow_shader)
            std::cout<<"Warning: dither shader is not available"<<std::endl;
    }

}
