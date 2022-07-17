terrarium.registered_blocks = {}
terrarium.block_names = {} -- lookup table to get block name by id

local function add_block_item(name, def)
    function def.on_use(user, itemstack, position, use_ctx)
        local x = math.floor(position.x)
        local y = math.floor(position.y)

        if core._get_block(x, y) == 0 then
            itemstack:add(-1)
            core._set_block(x, y, def.block_id)
        end
    end

    function def.on_alt_use(user, itemstack, position, use_ctx)
        local x = math.floor(position.x)
        local y = math.floor(position.y)

        if core._get_wall(x, y) == 0 then
            itemstack:add(-1)
            core._set_wall(x, y, def.block_id)
        end
    end

    def.inventory_image = def.inventory_image or def.image

    terrarium.register_item(name, def)
end

local block_defaults = {
    image = "",
    slippery = 0,
    is_solid = true,

    description = "Block",
    max_items = 999,
}

function terrarium.register_block(name, def)
    apply_defaults(def, block_defaults)

    def.block_id = core._register_block(def)

    terrarium.registered_blocks[name] = def
    terrarium.block_names[def.block_id] = name

    add_block_item(name, def)
end

function terrarium.register_block_alias(name, alias)
    if terrarium.registered_blocks[alias] ~= nil then
        error("cannot create block alias "..alias..": name already taken")
    end

    terrarium.registered_blocks[alias] = terrarium.registered_blocks[name]
end

function terrarium.get_block_id(name)
    if terrarium.registered_blocks[name] == nil then return 0 end

    return terrarium.registered_blocks[name].block_id
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
