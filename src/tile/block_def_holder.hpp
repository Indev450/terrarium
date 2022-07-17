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

#include "block_def.hpp"
#include "../utils/overflowing_map.hpp"

namespace Terrarium {

    class BlockDefHolder: public OverflowingMap<blockid, BlockDef> {
        BlockDef unknown;

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
    };

} // namespace Terrarium

#endif
