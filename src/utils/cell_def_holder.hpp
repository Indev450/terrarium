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

#include "overflowing_map.hpp"

namespace Terrarium {
    template <class Id, class CellDef>
    class CellDefHolder: public OverflowingMap<Id, CellDef> {
        CellDef unknown;
        std::unordered_map<std::string, Id> cell_names;

        bool isKeyFree(Id key) override {
            return key != 0 && OverflowingMap<Id, CellDef>::map.find(key) == OverflowingMap<Id, CellDef>::map.end() && std::all_of(
                cell_names.begin(),
                cell_names.end(),
                [&] (auto &pair) { return pair.second != key; }
            );
        }

    public:
        // Instead of pointer, returns reference to found cell def,
        // or reference to "unknown" cell. It makes easier to control
        // unknown cell behavior.
        CellDef &getOrUnknown(Id id) {
            std::shared_ptr<CellDef> def = OverflowingMap<Id, CellDef>::get(id);

            if (def == nullptr) {
                return unknown;
            }

            return *def;
        }

        Id add(std::shared_ptr<CellDef> def) {
            auto pair = cell_names.find(def->name);

            if (pair != cell_names.end()) {
                OverflowingMap<Id, CellDef>::set(pair->second, def);

                return pair->second;
            } else {
                Id id = OverflowingMap<Id, CellDef>::add(def);

                cell_names[def->name] = id;

                return id;
            }
        }

        // Methods to save existing cell ids into file. Used to keep same cell ids
        // after world saved, so loading new mods won't corrupt world save
        //
        // Save format:
        // Id                                entries_count
        // vector<{string name, Id id}>      cell_names
        void load(std::istream &s) {
            // Use Id type because there can be (in theory) 2^(sizeof(Id)*8)
            // possible cell id
            Id count = read<Id>(s);

            for (unsigned int i = 0; i < count; ++i) {
                std::string name = read<std::string>(s);
                Id id = read<Id>(s);

                cell_names[name] = id;
            }
        }

        void save(std::ostream &s) {
            Id count = cell_names.size();

            write(s, count);

            for (auto &pair: cell_names) {
                write<const std::string&>(s, pair.first); // name
                write<Id>(s, pair.second); // id
            }
        }
    };

}
