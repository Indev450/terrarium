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
