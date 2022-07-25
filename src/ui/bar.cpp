#include <sstream>

#include "bar.hpp"

namespace Terrarium {

    Bar::Bar(const sf::Vector2f &size, Gfx &gfx):
        shape(size)
    {
        text.setFont(gfx.font);

        background_color = sf::Color::Black;
        bar_color = sf::Color::Red;

        text.setFillColor(sf::Color::White);
    }

    void Bar::setValue(float new_value) {
        value = new_value;

        updateText();
    }

    void Bar::setMaxValue(float new_max_value) {
        max_value = new_max_value;

        updateText();
    }

    void Bar::setBackgroundColor(const sf::Color &color) {
        background_color = color;
    }

    void Bar::setBarColor(const sf::Color &color) {
        bar_color = color;
    }

    void Bar::setText(const std::string &new_text_string) {
        text_string = new_text_string;

        updateText();
    }

    void Bar::setTextStyle(BarTextStyle style) {
        text_style = style;

        updateText();
    }

    void Bar::setTextSize(unsigned int size) {
        text.setCharacterSize(size);
    }

    void Bar::setTextColor(const sf::Color &color) {
        text.setFillColor(color);
    }

    void Bar::render(sf::RenderTarget &target) {
        sf::Transform transform = getTransform();

        shape.setFillColor(background_color);
        target.draw(shape, transform);

        shape.setScale(std::max(0.f, std::min(value/max_value, 1.f)), 1);

        shape.setFillColor(bar_color);
        target.draw(shape, transform);

        shape.setScale(1, 1);

        if (text_style != None) {
            target.draw(text, transform);
        }
    }

    void Bar::updateText() {
        std::ostringstream fmt_text;

        fmt_text<<text_string<<std::fixed<<std::setprecision(2);

        switch (text_style) {
            case Division:
            {
                fmt_text<<value<<'/'<<max_value;
            }
            break;

            case Value:
            {
                fmt_text<<value;
            }
            break;

            case None:
                fmt_text.str("");
                fmt_text.clear();
            break;
        }

        text.setString(fmt_text.str());
    }

}
