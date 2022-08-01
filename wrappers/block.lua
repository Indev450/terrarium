terrarium.registered_blocks = {}
terrarium.block_names = {} -- lookup table to get block name by id

local function add_block_item(name, def)
    function def.on_use(user, itemstack, position, use_ctx)
        local x = math.floor(position.x)
        local y = math.floor(position.y)

        if terrarium.place_block(x, y, name) then
            itemstack:add(-1)
        end
    end

    -- Don't place interactive blocks at background layer
    if not def.is_interactive then
        function def.on_alt_use(user, itemstack, position, use_ctx)
            local x = math.floor(position.x)
            local y = math.floor(position.y)

            if terrarium.place_wall(x, y, name) then
                itemstack:add(-1)
            end
        end
    end

    def.inventory_image = def.inventory_image or def.image

    terrarium.register_item(name, def)
end

local block_defaults = {
    image = "",
    slippery = 0,
    is_solid = true,
    is_interactive = false,

    description = "Block",
    max_count = 999,

    -- Should return boolean that means, change block at that position or not
    on_place = function(position, user) return true end,
    on_destroy = function(position, user) return true end,

    on_interact = function(position, user) end
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

-- These functions don't use callbacks, use them when you need to
-- modify blocks automatically, like in mapgen
function terrarium.set_block(x, y, block_name)
    core._set_block(x, y, terrarium.get_block_id(block_name))
end

function terrarium.set_wall(x, y, block_name)
    core._set_wall(x, y, terrarium.get_block_id(block_name))
end

-- These functions use callbacks, use them when blocks modified by user
function terrarium.dig_block(x, y, user)
    return terrarium._dig(x, y, user, true)
end

function terrarium.dig_wall(x, y, user)
    return terrarium._dig(x, y, user, false)
end

function terrarium.place_block(x, y, block_name, user)
    return terrarium._place(x, y, block_name, user, true)
end

function terrarium.place_wall(x, y, block_name, user)
    return terrarium._place(x, y, block_name, user, false)
end


function terrarium._dig(x, y, user, fg)
    -- Select layer, foreground or background
    local _get = fg and core._get_block or core._get_wall
    local _set = fg and core._set_block or core._set_wall

    local dig_id = _get(x, y)

    -- Block at that position is air, so there is nothing to dig
    if dig_id == 0 then return false end

    local dig_name = terrarium.block_names[dig_id] or "unknown"

    local def = terrarium.registered_blocks[dig_name]

    -- If def is nil, then simply remove that unknown block. If it is not nil,
    -- and on_destroy returns false, we are not allowed to dig block, don't dig
    if def ~= nil and not def.on_destroy({ x = x, y = y }, user) then
        return false
    end

    -- Change that block to air
    _set(x, y, 0)

    -- true means "digging" was successful
    return true
end

function terrarium._place(x, y, block_name, user, fg)
    -- Select layer, foreground or background
    local _get = fg and core._get_block or core._get_wall
    local _set = fg and core._set_block or core._set_wall

    local old_id = _get(x, y)

    -- Block at that position is not air, don't place block here
    if old_id ~= 0 then return false end

    -- Change that block
    _set(x, y, terrarium.get_block_id(block_name))

    terrarium.registered_blocks[block_name].on_place({ x = x, y = y }, user)

    return true
end

function terrarium.get_block_inventory(x, y)
    return core._get_block_inventory(x, y)
end


core._event_handlers["BlockActivate"] = function(event)
    local block_id = core._get_block(event.block_event.position.x, event.block_event.position.y)

    local block_def = terrarium.registered_blocks[terrarium.block_names[block_id]]

    block_def.on_interact(event.block_event.position, core.get_user(event.block_event.user))
end
