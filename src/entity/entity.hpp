#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "../tile/tile.hpp"
#include "../graphics/animation_map.hpp"

namespace Terrarium {

    typedef uint16_t entityid;

    class GameState;

    struct CollisionInfo {
        bool blocku = false;
        bool blockd = false;
        bool blockl = false;
        bool blockr = false;
    };

    struct PhysicsParams {
        float gravity = 40;
        float slippery = 0;

        bool enable_collision = true;
    };

    class Entity {
        void collide(GameState &game, bool by_x);

    public:
        sf::FloatRect hitbox = {0, 0, 0, 0};

        CollisionInfo collision_info;

        PhysicsParams physics;

        sf::Vector2f speed = {0, 0};

        entityid id = 0;

        AnimationMap anims;

        virtual void update(GameState &game, float dtime);
    };

} // namespace Terrarium

#endif
