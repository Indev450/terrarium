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

#include "cis.hpp"
#include "../utils/binary_io.hpp"

namespace Terrarium {

    std::shared_ptr<Inventory> CIS::get(const InventoryLocation &location) {
        auto pair = inventories.find(location);

        if (pair != inventories.end()) {
            return pair->second;
        }

        return nullptr;
    }

    std::shared_ptr<Inventory> CIS::getOrCreate(const InventoryLocation &location) {
        if (inventories.count(location) == 0) {
            inventories[location] = std::make_shared<Inventory>(0);
        }

        return inventories[location];
    }

    void CIS::load(std::istream &s, GameState &game) {
        uint32_t count = read<uint32_t>(s);

        for (unsigned int i = 0; i < count; ++i) {
            uint8_t location_type = read<uint8_t>(s);

            InventoryLocation location;

            switch (location_type) {
                case 0: // Block position
                {
                    uint16_t x = read<uint16_t>(s);
                    uint16_t y = read<uint16_t>(s);

                    location = sf::Vector2i(x, y);
                }
                break;

                case 1: // Entity id
                {
                    location = read<entityid>(s);
                }
                break;

                case 2: // Detached (string name)
                {
                    location = read<std::string>(s);
                }
                break;

                default:
                {
                    throw std::invalid_argument("unknown inventory location type");
                }
            }

            inventories[location] = std::make_shared<Inventory>(0);
            inventories[location]->load(s, game);
        }
    }

    void CIS::save(std::ostream &s) {
        write<uint32_t>(s, inventories.size());

        for (auto &pair: inventories) {
            uint8_t location_type = pair.first.index();

            write(s, location_type);

            switch (location_type) {
                case 0: // Block position
                {
                    sf::Vector2i position = std::get<0>(pair.first);

                    write<uint16_t>(s, position.x);
                    write<uint16_t>(s, position.y);
                }
                break;

                case 1: // Entity id
                {
                    write<entityid>(s, std::get<1>(pair.first));
                }
                break;

                case 2: // Detached (string name)
                {
                    write<const std::string&>(s, std::get<2>(pair.first));
                }
                break;

                default:
                {
                    throw std::invalid_argument("unknown inventory location type");
                }
            }

            pair.second->save(s);
        }
    }

}
