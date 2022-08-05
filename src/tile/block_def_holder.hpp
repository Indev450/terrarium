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

#ifndef BLOCK_DEF_HOLDER_HPP
#define BLOCK_DEF_HOLDER_HPP

#include <string>
#include <iostream>

#include "block_def.hpp"
#include "../utils/overflowing_map.hpp"
#include "../utils/binary_io.hpp"

namespace Terrarium {

    class BlockDefHolder: public OverflowingMap<blockid, BlockDef> {
        BlockDef unknown;
        std::unordered_map<std::string, blockid> block_names;

    public:
        // Instead of pointer, returns reference to found block def,
        // or reference to "unknown" block. It makes easier to control
        // unknown block behavior.
        BlockDef &getOrUnknown(blockid block) {
            std::shared_ptr<BlockDef> def = get(block);

            if (def == nullptr) {
                return unknown;
            }

            return *def;
        }

        blockid add(std::shared_ptr<BlockDef> def) {
            auto pair = block_names.find(def->name);

            if (pair != block_names.end()) {
                set(pair->second, def);

                return pair->second;
            } else {
                blockid id = OverflowingMap<blockid, BlockDef>::add(def);

                block_names[def->name] = id;

                return id;
            }
        }

        // Methods to save existing block ids into file. Used to keep same block ids
        // after world saved, so loading new mods won't corrupt world save
        //
        // Save format:
        // blockid                           entries_count
        // vector<{string name, blockid id}> block_names
        void load(std::istream &s) {
            // Use blockid type because there can be (in theory) 2^(sizeof(blockid)*8)
            // possible block ids
            blockid count = read<blockid>(s);

            for (unsigned int i = 0; i < count; ++i) {
                std::string name = read<std::string>(s);
                blockid id = read<blockid>(s);

                block_names[name] = id;
            }
        }

        void save(std::ostream &s) {
            blockid count = block_names.size();

            write(s, count);

            for (auto &pair: block_names) {
                write<const std::string&>(s, pair.first); // name
                write<blockid>(s, pair.second); // id
            }
        }
    };

} // namespace Terrarium

#endif
