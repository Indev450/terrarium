terrarium.registered_blocks = {}
terrarium.block_names = {} -- lookup table to get block name by id

local block_defaults = {
    image = "",
    slippery = 0,
    is_solid = true,
}

function terrarium.register_block(name, def)
    apply_defaults(def, block_defaults)

    def.id = core._register_block(def)

    terrarium.registered_blocks[name] = def
    terrarium.block_names[def.id] = name
end

function terrarium.register_block_alias(name, alias)
    if terrarium.registered_blocks[alias] ~= nil then
        error("cannot create block alias "..alias..": name already taken")
    end

    terrarium.registered_blocks[alias] = terrarium.registered_blocks[name]
end

function terrarium.get_block_id(name)
    if terrarium.registered_blocks[name] == nil then return 0 end

    return terrarium.registered_blocks[name].id
end

function terrarium.get_tile(x, y)
    local tile = core._get_tile()

    tile.block = terrarium.block_names[tile.block] or "unknown"
    tile.wall = terrarium.block_names[tile.wall] or "unknown"

    return tile
end

function terrarium.get_block(x, y)
    return terrarium.block_names[core._get_block(x, y)] or "unknown"
end

function terrarium.get_wall(x, y)
    return terrarium.block_names[core._get_wall(x, y)] or "unknown"
end

function terrarium.set_block(x, y, block_name)
    core._set_block(x, y, terrarium.get_block_id(block_name))
end

function terrarium.set_wall(x, y, block_name)
    core._set_wall(x, y, terrarium.get_block_id(block_name))
end
