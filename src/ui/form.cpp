#include "form.hpp"
#include "../event/event.hpp"
#include "../game.hpp"

namespace Terrarium {

    Form::Form(const std::string &_name, std::shared_ptr<FormSource> _source):
        name(_name), source(_source)
    {}

    void Form::addField(const std::string &name, std::unique_ptr<FormField> field) {
        fields[name] = std::move(field);
    }

    bool Form::click(GameState &game, const sf::Vector2f &position) {
        bool clicked = false;

        sf::Vector2f rpos = getInverseTransform().transformPoint(position);

        for (auto &field: fields) {
            if (field.second->click(game, rpos)) {
                clicked = true;
            }
        }

        checkFields(game);

        return clicked;
    }

    void Form::render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) {
        sf::Transform combined_transform = getTransform() * parent_transform;

        for (auto &field: fields) {
            field.second->render(target, game, combined_transform);
        }
    }

    void Form::checkFields(GameState &game) {
        bool ready = false;

        for (auto &field: fields) {
            if (field.second->isReady()) {
                ready = true;
            }
        }

        if (ready) {
            UIEvent *ui_event = new UIEvent();

            std::vector<std::pair<std::string, std::string>> field_values;

            for (auto &field: fields) {
                field_values.push_back(std::make_pair(field.first, field.second->getValue()));
            }

            ui_event->form = name;

            ui_event->fields = std::move(field_values);

            ui_event->user = game.player;

            ui_event->form_source = source;

            game.events.emplace(Event::Type::UISubmit, ui_event);
        }
    }
}
