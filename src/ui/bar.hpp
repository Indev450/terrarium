#ifndef UI_BAR_HPP
#define UI_BAR_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include "../graphics/gfx.hpp"

namespace Terrarium {

    enum BarTextStyle {
        Division, // 80/100
        Value, // 80
        None, // no text
    };

    // For various progress bars/hp bars. I'm keeping this separate from
    // UIElement's, because i haven't figured out way to safely modify Bar
    class Bar: public sf::Transformable {
        float value = 0;
        float max_value = 100;

        sf::Text text; // For text like (hp: 80/100)
        std::string text_string;
        BarTextStyle text_style = BarTextStyle::None;

        sf::Color background_color;
        sf::Color bar_color;

        sf::RectangleShape shape;

        void updateText();

    public:
        Bar(const sf::Vector2f &size, Gfx &gfx);

        void setValue(float new_value);

        void setMaxValue(float new_max_value);

        void setBackgroundColor(const sf::Color &color);

        void setBarColor(const sf::Color &color);

        void setText(const std::string &text_string);

        void setTextStyle(BarTextStyle style);

        void setTextSize(unsigned int size);

        void setTextColor(const sf::Color &color);

        void render(sf::RenderTarget &target);

    };

}

#endif
