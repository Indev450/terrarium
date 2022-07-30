#ifndef UTIL_SCREEN_TRANSFORMABLE_HPP
#define UTIL_SCREEN_TRANSFORMABLE_HPP

#include <SFML/Graphics.hpp>

class ScreenTransformable: sf::Transformable {
    // Relative position. Positive coords mean offset from top left corner,
    // negative - offset from bottom right corner
    sf::Vector2f position;

public:
    void setScreenSize(const sf::Vector2f &size) {
        sf::Vector2f new_position = position;

        if (position.x < 0) {
            new_position.x += size.x;
        }

        if (position.y < 0) {
            new_position.y += size.y;
        }

        sf::Transformable::setPosition(new_position);
    }

    void setPosition(const sf::Vector2f &position) {
        this->position = position;
    }

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    const sf::Transform &getTransform() const {
        return sf::Transformable::getTransform();
    }

    const sf::Transform &getInverseTransform() const {
        return sf::Transformable::getInverseTransform();
    }

};

#endif
