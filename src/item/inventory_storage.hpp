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

#ifndef InventoryStorage_HPP
#define InventoryStorage_HPP

#include <memory>
#include <unordered_map>
#include <string>
#include <variant>
#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "inventory.hpp"
#include "../entity/entity.hpp"
#include "../utils/vector2i_hash.hpp"

namespace Terrarium {

    struct GameState;

    typedef std::variant<sf::Vector2i, entityid, std::string> InventoryLocation;

    // Centralized Inventory Storage
    class InventoryStorage {
        std::unordered_map<InventoryLocation, std::shared_ptr<Inventory>> inventories;

    public:
        std::shared_ptr<Inventory> get(const InventoryLocation &location);

        std::shared_ptr<Inventory> getOrCreate(const InventoryLocation &location);

        // InventoryStorage save format:
        // u32                               count
        // vector<{
        //     u8 location_type,
        //     InventoryLocation location
        //     Inventory inventory}>         inventories
        void load(std::istream &s, GameState &game);

        void save(std::ostream &s);
    };

}

#endif
