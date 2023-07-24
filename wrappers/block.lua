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

local function add_multiblock_item(name, def)
    function def.on_use(user, itemstack, position, use_ctx)
        local x = math.floor(position.x)
        local y = math.floor(position.y)

        if terrarium.place_multiblock(x, y, name) then
            itemstack:add(-1)
        end
    end

    def.inventory_image = def.inventory_image or def.image

    terrarium.register_item(name, def)
end

local block_defaults = {
    image = "",
    draw_type = "Image",

    autotile_neighbour = true,
    autotile_single = false,

    slippery = 0.1,
    is_solid = true,
    is_platform = false,

    blocks_light = nil,
    light = { r = 0, g = 0, b = 0, },

    replacable = false,

    groups = {},

    drop = nil,

    dig_sound = nil,
    place_sound = nil,

    -- Should return boolean that means, change block at that position or not
    on_place = function(position, user) return true end,
    on_destroy = function(position, user) return true end,

    -- where can be "up", "down", "left" and "right"
    on_neighbour_destroy = function(position, where, block_name, user) end,

    on_interact = nil,

    multiblock_size = nil,

    -- Block item
    description = "Block",
    max_count = 999,
}

function terrarium.register_block(name, def)
    if def.multiblock_size ~= nil and def.draw_type == nil then
        def.draw_type = "Multiblock"
    end

    if type(def.light) == "number" then
        def.light = { r = def.light, g = def.light, b = def.light, }
    end

    apply_defaults(def, block_defaults)
    def.is_interactive = def.on_interact ~= nil

    if def.blocks_light == nil then
        def.blocks_light = def.is_solid
    end

    -- If you need block that drops nothing, use "" as drops
    def.drop = def.drop or name

    def.block_id = core._register_block(name, def)

    terrarium.registered_blocks[name] = def
    terrarium.block_names[def.block_id] = name

    if def.multiblock_size ~= nil then
        add_multiblock_item(name, def)
    else
        add_block_item(name, def)
    end
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

function terrarium.connected_to_blocks(names)
    return terrarium._connected_to(names, terrarium.dig_block)
end

function terrarium.connected_to_walls(names)
    return terrarium._connected_to(names, terrarium.dig_wall)
end

function terrarium._connected_to(names, dig_func)
    local names_set = {}

    for _, name in ipairs(names) do names_set[name] = true end

    return function(position, where, block_name, user)
        if names_set[where] then
            dig_func(position.x, position.y, user, true)
        end
    end
end

function terrarium.get_tile(x, y)
    local tile = core._get_tile()

    tile.block = terrarium.block_names[tile.block]
    tile.wall = terrarium.block_names[tile.wall]

    return tile
end

function terrarium.get_block(x, y)
    return terrarium.block_names[core._get_block(x, y)]
end

function terrarium.get_wall(x, y)
    return terrarium.block_names[core._get_wall(x, y)]
end

-- These functions only place blocks and don't care about replaced ones (except
-- for multiblocks), use them when you need to modify blocks automatically,
-- like in mapgen
function terrarium.set_block(x, y, block_name, nocall)
    local def = terrarium.registered_blocks[terrarium.get_block(x, y)]

    if def and def.multiblock_size then
        local origin = core._get_multiblock_origin(x, y)
        core._set_multiblock(origin.x, origin.y, def.multiblock_size.x, def.multiblock_size.y, 0)
    end

    def = terrarium.registered_blocks[block_name]

    local block_id = 0

    if def ~= nil then block_id = def.block_id end

    core._set_block(x, y, block_id)

    if def ~= nil and not nocall then
        def.on_place({ x = x, y = y })
    end
end

function terrarium.set_wall(x, y, block_name, nocall)
    local def = terrarium.registered_blocks[block_name]

    local block_id = 0

    if def ~= nil then block_id = def.block_id end

    core._set_wall(x, y, block_id)

    if not nocall then
        def.on_place({ x = x, y = y })
    end
end

function terrarium.set_multiblock(x, y, block_name, nocall)
    local def = terrarium.registered_blocks[block_name]

    local block_id = 0

    if def ~= nil then block_id = def.block_id end

    core._set_multiblock(x, y, def.multiblock_size.x, def.multiblock_size.y, block_id)

    if not nocall then
        def.on_place({ x = x, y = y })
    end
end

-- These functions use callbacks, use them when blocks modified by user
function terrarium.dig_block(x, y, user, nosound)
    return terrarium._dig(x, y, user, nosound, true)
end

function terrarium.dig_wall(x, y, user)
    return terrarium._dig(x, y, user, nosound, false)
end

function terrarium.place_block(x, y, block_name, user, nosound)
    return terrarium._place(x, y, block_name, user, nosound, true)
end

function terrarium.place_wall(x, y, block_name, user, nosound)
    return terrarium._place(x, y, block_name, user, nosound, false)
end

function terrarium.get_multiblock_origin(x, y)
    return core._get_multiblock_origin(x, y)
end

function terrarium.place_multiblock(x, y, block_name, user, nosound)
    local def = terrarium.registered_blocks[block_name]

    if def.multiblock_size == nil then
        error("Attempt to call terrarium.place_multiblock for regular block. Use terrarium.place_block instead")
    end

    local to_dig = {}

    for xoff = 0, def.multiblock_size.x-1 do
        for yoff = 0, def.multiblock_size.y-1 do
            local old_id = core._get_block(x + xoff, y + yoff)

            local old_name = terrarium.block_names[old_id] or "unknown"

            local old_def = terrarium.registered_blocks[old_name]

            -- Block at that position is not replacable, don't place block here
            if old_def and not old_def.replacable then
                return false
            elseif old_def then
                -- There is block, but it is replacable so we need to dig it first
                table.insert(to_dig, { x = x + xoff, y = y + yoff })
            end
        end
    end

    for _, dig_block in ipairs(to_dig) do
        terrarium.dig_block(dig_block.x, dig_block.y, user, nosound)
    end

    -- Change that block
    core._set_multiblock(x, y, def.multiblock_size.x, def.multiblock_size.y, def.block_id)

    def.on_place({ x = x, y = y }, user)

    if def.place_sound ~= nil and not nosound then
        core._play_sound({
            name = def.place_sound.name,
            volume = def.place_sound.volume,
            pitch = def.place_sound.pitch,

            source = {
                x = x,
                y = y,
            },
        })
    end

    return true
end

function terrarium._dig(x, y, user, nosound, fg)
    -- Select layer, foreground or background
    local _get = fg and core._get_block or core._get_wall
    local _get_name = fg and terrarium.get_block or terrarium.get_wall
    local _set = fg and core._set_block or core._set_wall

    local dig_id = _get(x, y)

    -- Block at that position is air, so there is nothing to dig
    if dig_id == 0 then return false end

    local dig_name = terrarium.block_names[dig_id] or "unknown"

    local def = terrarium.registered_blocks[dig_name]

    -- If def is nil, then simply remove that unknown block. If it is not nil,
    -- and on_destroy returns false, we are not allowed to dig block, don't dig
    if def ~= nil then
        local origin = { x = x, y = y }

        if fg then
            origin = core._get_multiblock_origin(x, y)
        end

        if not def.on_destroy(origin, user) then
            return false
        end
    end

    local function trigger_neighbour_callback(neighbour)
        local def = terrarium.registered_blocks[_get_name(
            neighbour.x, neighbour.y
        )]

        if def ~= nil then
            def.on_neighbour_destroy(neighbour, neighbour.where, dig_name, user)
        end
    end

    -- Multiblock handling... Is just a mess :(
    if fg and def.multiblock_size then
        for xoff = 0, def.multiblock_size.x-1 do
            trigger_neighbour_callback({
                x = x + xoff, y = y - 1, where = "down",
            })
            trigger_neighbour_callback({
                x = x + xoff, y = y + def.multiblock_size.y, where = "up",
            })
        end

        for yoff = 0, def.multiblock_size.y-1 do
            trigger_neighbour_callback({
                x = x - 1, y = y + yoff, where = "right",
            })
            trigger_neighbour_callback({
                x = x + def.multiblock_size.x, y = y + yoff, where = "left",
            })
        end
    else
        -- NOTE: where means where is OUR block, relative to neighbour, not where
        -- neighbour itself
        local neighbours = {
            { x = x, y = y - 1, where = "down" },
            { x = x, y = y + 1, where = "up" },
            { x = x - 1, y = y, where = "right" },
            { x = x + 1, y = y, where = "left" },
        }

        for _, neighbour in pairs(neighbours) do
            trigger_neighbour_callback(neighbour)
        end
    end

    -- Change that block to air
    _set(x, y, 0)

    -- Remove multiblock
    if fg and def.multiblock_size then
        local origin = core._get_multiblock_origin(x, y)
        core._set_multiblock(origin.x, origin.y, def.multiblock_size.x, def.multiblock_size.y, 0)
    end

    if def ~= nil and def.drop ~= "" then
        terrarium.drop_item(ItemStack(def.drop), vec2.new(x, y))
    end

    if def ~= nil and def.dig_sound ~= nil and not nosound then
        core._play_sound({
            name = def.dig_sound.name,
            volume = def.dig_sound.volume,
            pitch = def.dig_sound.pitch,

            source = {
                x = x,
                y = y,
            },
        })
    end

    -- true means "digging" was successful
    return true
end

function terrarium._place(x, y, block_name, user, nosound, fg)
    local def = terrarium.registered_blocks[block_name]

    if def.multiblock_size ~= nil then
        error("Attempt to call terrarium.place_block for multiblock. Use terrarium.place_multiblock instead")
    end

    -- Select layer, foreground or background
    local _get = fg and core._get_block or core._get_wall
    local _set = fg and core._set_block or core._set_wall

    local old_id = _get(x, y)

    local old_name = terrarium.block_names[old_id] or "unknown"

    local old_def = terrarium.registered_blocks[old_name]

    -- Block at that position is not replacable, don't place block here
    if old_def and not old_def.replacable then
        return false
    elseif old_def then
        -- There is block, but it is replacable so we need to dig it first
        terrarium._dig(x, y, user, nosound, fg)
    end

    -- Change that block
    _set(x, y, terrarium.get_block_id(block_name))

    def.on_place({ x = x, y = y }, user)

    if def.place_sound ~= nil and not nosound then
        core._play_sound({
            name = def.place_sound.name,
            volume = def.place_sound.volume,
            pitch = def.place_sound.pitch,

            source = {
                x = x,
                y = y,
            },
        })
    end

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
