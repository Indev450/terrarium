#ifndef VECTOR2I_HASH_HPP
#define VECTOR2I_HASH_HPP

#include <limits>

#include <SFML/System/Vector2.hpp>

struct HashVector2i {

    size_t operator()(const sf::Vector2i &vec) const {
        return vec.x + static_cast<size_t>(vec.y) * std::numeric_limits<int>::max();
    }

};

#endif
