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

#include <algorithm>

#include "../utils/math.hpp"
#include "../entity/entity.hpp"
#include "../game.hpp"

#include "particles.hpp"

namespace Terrarium {

    void Particle::update(GameState &game, float dtime) {
        if (isExpired()) return;

        sf::Vector2f speed_diff = speed*speed_mult - speed + accel;

        // Approximation
        speed += speed_diff * dtime * 0.5f;
        PhysicalBody::update(game, dtime);
        speed += speed_diff * dtime * 0.5f;

        lifetime -= dtime;
    }

    void Particle::draw(sf::VertexArray &vertices) {
        const float x = hitbox.left, y = hitbox.top, w = hitbox.width, h = hitbox.height;
        const float tx = texture_rect.left, ty = texture_rect.top;
        const float tw = texture_rect.width, th = texture_rect.height;

        vertices.append(sf::Vertex(
            sf::Vector2f(x, y), color, sf::Vector2f(tx, ty)
        ));
        vertices.append(sf::Vertex(
            sf::Vector2f(x+w, y), color, sf::Vector2f(tx+tw, ty)
        ));
        vertices.append(sf::Vertex(
            sf::Vector2f(x, y+h), color, sf::Vector2f(tx, ty+th)
        ));
        vertices.append(sf::Vertex(
            sf::Vector2f(x+w, y), color, sf::Vector2f(tx+tw, ty)
        ));
        vertices.append(sf::Vertex(
            sf::Vector2f(x+w, y+h), color, sf::Vector2f(tx+tw, ty+th)
        ));
        vertices.append(sf::Vertex(
            sf::Vector2f(x, y+h), color, sf::Vector2f(tx, ty+th)
        ));
    }

    ParticleSpawner::ParticleSpawner(const ParticleSpawnerParams &_params, sf::Vector2f _position, std::weak_ptr<Entity> _source):
        params(_params), position(_position), source(_source), rng(std::random_device()()) {

        particles.reserve(params.amount);

        // Handle one-time emission
        if (params.time == 0.f) {
            if (!source.expired()) {
                auto entity = source.lock();
                position.x = entity->hitbox.left + entity->hitbox.width/2;
                position.y = entity->hitbox.top + entity->hitbox.height/2;
            }

            for (unsigned i = 0; i < params.amount; ++i) {
                spawnParticle();
            }
        }
    }

    void ParticleSpawner::spawnParticle() {
        sf::Vector2f offset, speed;

        offset.x = lerp(params.minoffset.x, params.maxoffset.x, lerpdist(rng));
        offset.y = lerp(params.minoffset.y, params.maxoffset.y, lerpdist(rng));

        speed.x = lerp(params.minspeed.x, params.maxspeed.x, lerpdist(rng));
        speed.y = lerp(params.minspeed.y, params.maxspeed.y, lerpdist(rng));

        particles.emplace_back(lerp(params.minlifetime, params.maxlifetime, lerpdist(rng)), params.texture_rect, params.color);

        auto &particle = particles.back();

        particle.hitbox.left = position.x + offset.x;
        particle.hitbox.top = position.y + offset.y;
        particle.speed = speed;

        if (std::holds_alternative<sf::Vector2f>(params.accel)) {
            particle.accel = std::get<sf::Vector2f>(params.accel);
        } else {
            particle.accel = speed/(speed.x*speed.x + speed.y*speed.y)*std::get<float>(params.accel);
        }

        particle.speed_mult = params.speed_mult;

        particle.physics = params.physics;
    }

    void ParticleSpawner::update(GameState &game, float dtime) {
        spawntimer += dtime;
        lifetime += dtime;

        if (!source.expired()) {
            auto entity = source.lock();
            position.x = entity->hitbox.left + entity->hitbox.width/2;
            position.y = entity->hitbox.top + entity->hitbox.height/2;
        }

        if (params.time < 0 || lifetime < params.time) {
            while (spawntimer > 0) {
                spawntimer -= std::abs(params.time / params.amount);

                spawnParticle();
            }
        }

        for (auto &particle: particles) {
            particle.update(game, dtime);
        }

        particles.erase(std::remove_if(particles.begin(), particles.end(), [] (auto &particle) { return particle.isExpired(); }), particles.end());
    }

    void ParticleSpawner::draw(GameState &game, sf::RenderTarget &target) {
        sf::VertexArray vertices(sf::Triangles);
        sf::RenderStates states;

        for (auto &particle: particles) {
            if (particle.hitbox.intersects(game.camera)) {
                particle.draw(vertices);
            }
        }

        // Apparently, these should go in this order???
        states.transform *= game.blocks_to_pixels;
        states.transform.translate(-game.camera.left, -game.camera.top);

        states.texture = params.texture;

        target.draw(vertices, states);
    }

    void ParticleManager::update(GameState &game, float dtime) {
        std::vector<particlespawnerid> ids_to_remove;

        for (auto &pair: map) {
            pair.second->update(game, dtime);

            if (pair.second->isExpired()) {
                ids_to_remove.push_back(pair.first);
            }
        }

        for (auto id: ids_to_remove) {
            map.erase(id);
        }
    }

    void ParticleManager::draw(GameState &game, sf::RenderTarget &target) {
        for (auto &pair: map) {
            pair.second->draw(game, target);
        }
    }
}
