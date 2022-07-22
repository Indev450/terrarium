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
