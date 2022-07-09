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
