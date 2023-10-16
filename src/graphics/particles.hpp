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

#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include <random>
#include <variant>

#include <SFML/Graphics.hpp>

#include "../entity/physical_body.hpp"
#include "../tile/tile.hpp"
#include "../utils/auto_id_map.hpp"

namespace Terrarium {

    class Entity;

    typedef uint16_t particlespawnerid;

    class Particle: public PhysicalBody {
        float lifetime = 0;

        sf::FloatRect texture_rect;
        sf::Color color;

    public:
        sf::Vector2f accel;
        float speed_mult = 1.0;

    public:
        Particle(float _lifetime, sf::FloatRect _texture_rect, sf::Color _color):
            lifetime(_lifetime), texture_rect(_texture_rect), color(_color)
        {
            hitbox = { 0, 0, texture_rect.width/Tile::SIZE, texture_rect.height/Tile::SIZE };
        }

        inline bool isExpired() const {
            return lifetime < 0;
        }

        void update(GameState &game, float dtime) override;

        void draw(sf::VertexArray &vertices);
    };

    struct ParticleSpawnerParams {
        float time = 1.0;

        unsigned amount = 1;

        PhysicsParams physics;

        const sf::Texture *texture = nullptr;
        sf::FloatRect texture_rect = { 0.f, 0.f, 1.f, 1.f };

        sf::Color color = sf::Color::White;

        sf::Vector2f minoffset, maxoffset;
        sf::Vector2f minspeed, maxspeed;

        std::variant<sf::Vector2f, float> accel = 0.f;

        float speed_mult = 1.0;

        float minlifetime = 1.0, maxlifetime = 2.0;
    };

    class ParticleSpawner {
        std::vector<Particle> particles;

        ParticleSpawnerParams params;

        // Spawn position, will be updated to source if possible
        sf::Vector2f position;

        // Optional source entity
        std::weak_ptr<Entity> source;

        // Grows over time. Every frame, particles spawn and decrease it by
        // 1/amount, while it is greatter than 0
        float spawntimer = 0;

        // Increased just like spawntimer but doesn't get decreased
        float lifetime = 0;

        std::default_random_engine rng;
        std::uniform_real_distribution<float> lerpdist {0.f, 1.f};

        void spawnParticle();

    public:
        ParticleSpawner(const ParticleSpawnerParams &_params, sf::Vector2f _position = { 0, 0 }, std::weak_ptr<Entity> _source = std::weak_ptr<Entity>());

        // ParticleSpawner is expired if it's lifetime greatter than params.time
        // and if its particles list is empty, so particles don't all disappear
        // suddenly
        bool isExpired() const {
            return params.time >= 0 && lifetime >= params.time && particles.empty();
        }

        // Prevents ParticleSpawner from spawning any new particles. Game will
        // still wait for any existing particles until they are all expired
        void stop() {
            lifetime = params.time;
            params.time = 0;
        }

        // Update/spawn particles remove expired particles
        void update(GameState &game, float dtime);

        // Draw particles that are visible on screen
        void draw(GameState &game, sf::RenderTarget &target);
    };

    class ParticleManager: public AutoIDMap<particlespawnerid, ParticleSpawner> {
    public:
        void update(GameState &game, float dtime);
        void draw(GameState &game, sf::RenderTarget &target);
    };

}

#endif
