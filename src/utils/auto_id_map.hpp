/*
 * Copyright 2022 Indev
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

#ifndef AUTO_ID_MAP_HPP
#define AUTO_ID_MAP_HPP

#include <unordered_map>
#include <memory>

namespace Terrarium {

    // Utility class that stores data associated with id that can be overflowed,
    // such as uint16_t
    template <class Key, class Value>
    class AutoIDMap {
        Key last_key = 0;

        Key getFreeKey() {
            Key start_key = last_key;

            while (!isKeyFree(++last_key)) {
                if (last_key == start_key) {
                    return 0;
                }
            }

            return last_key;
        }

    protected:
        std::unordered_map<Key, std::shared_ptr<Value>> map;

        virtual bool isKeyFree(Key key) {
            return key != 0 && map.find(key) == map.end();
        }
    public:
        Key add(std::shared_ptr<Value> value) {
            Key key = getFreeKey();

            if (key == 0) {
                return 0;
            }

            map[key] = value;

            return key;
        }

        std::shared_ptr<Value> get(Key key) {
            if (isKeyFree(key)) {
                return nullptr;
            }

            return map[key];
        }

        void set(Key key, std::shared_ptr<Value> value) {
            map[key] = value;
        }

        void del(Key key) {
            map.erase(key);
        }

        typename std::unordered_map<Key, std::shared_ptr<Value>>::iterator begin() {
            return map.begin();
        }

        typename std::unordered_map<Key, std::shared_ptr<Value>>::iterator end() {
            return map.end();
        }
    };

} // namespace Terrarium

#endif
