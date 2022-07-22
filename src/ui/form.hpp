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

#ifndef UI_FORM_HPP
#define UI_FORM_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <SFML/System/Vector2.hpp>

#include "element.hpp"
#include "../entity/entity.hpp"

namespace Terrarium {

    class FormField: public UIElement {
    public:
        // Should return true if field has been submitted. For example, buttons
        // return true when clicked
        virtual bool isReady() { return false; }

        // Called when field is ready to send
        virtual std::string getValue() = 0;
    };

    struct FormSource {
        const enum Type {
            Entity,
            Block,
        } type;

        const union {
            std::weak_ptr<Terrarium::Entity> entity;
            sf::Vector2i block;
        };

        FormSource(std::weak_ptr<Terrarium::Entity> _entity):
            type(Entity), entity(_entity)
        {}

        FormSource(sf::Vector2i _block):
            type(Block), block(_block)
        {}

        ~FormSource() {
            if (type == Entity) {
                entity.~weak_ptr();
            }
        }
    };

    class Form: public UIElement {
        std::unordered_map<std::string, std::unique_ptr<FormField>> fields;

        std::string name;

        std::shared_ptr<FormSource> source;

        // If some field is ready, send UISubmit event
        void checkFields(GameState &game);
    public:
        Form(const std::string &name, std::shared_ptr<FormSource> source = nullptr);

        void addField(const std::string &name, std::unique_ptr<FormField> field);

        bool click(GameState &game, const sf::Vector2f &position) override;

        void render(sf::RenderTarget &target, GameState &game, const sf::Transform &parent_transform) override;
    };

}

#endif
