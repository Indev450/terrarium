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
    solid = true,
    platform = false,

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

    on_timer = nil,

    multiblock_size = nil,

    dig_particles_def = {
        time = 0,
        amount = 20,
        physics = {
            gravity = 10,
            enable_collision = false,
        },
        texture_rect = { x = 0, y = 0, width = 2, height = 2, },
        minoffset = vec2.new(0, 0),
        maxoffset = vec2.new(1-2/16, 1-2/16),
        minspeed = vec2.new(-0.5, -0.5),
        maxspeed = vec2.new(0.5, 0.5),
        minlifetime = 2,
        maxlifetime = 4,
        accel = 0,
        speed_mult = 0.6,
    },

    -- Block item
    description = "Block",
    max_count = 999,
}

function terrarium.register_block(name, def)
    def = deep_copy(def)

    if def.multiblock_size ~= nil and def.draw_type == nil then
        def.draw_type = "Multiblock"
    end

    if type(def.light) == "number" then
        def.light = { r = def.light, g = def.light, b = def.light, }
    end

    apply_defaults(def, block_defaults)
    def.interactive = def.on_interact ~= nil

    if def.blocks_light == nil then
        def.blocks_light = def.solid
    end

    -- If you need block that drops nothing, use "" as drops
    def.drop = def.drop or name

    def.block_id = core._register_block(name, def)

    if def.dig_particles_def.color == nil and def.dig_particles_def.texture == nil then
        def.dig_particles_def.color = core._average_block_color(def.block_id)
    end

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
        def.on_place({ x = x, y = y, is_wall = false })
    end
end

function terrarium.set_wall(x, y, block_name, nocall)
    local def = terrarium.registered_blocks[block_name]

    local block_id = 0

    if def ~= nil then block_id = def.block_id end

    core._set_wall(x, y, block_id)

    if not nocall then
        def.on_place({ x = x, y = y, is_wall = true })
    end
end

function terrarium.set_multiblock(x, y, block_name, nocall)
    local def = terrarium.registered_blocks[block_name]

    local block_id = 0
    local width = 1
    local height = 1

    if def then
        if not def.multiblock_size then
            error("Attempt to use terrarium.set_multiblock with usual block. Use terrarium.set_block instead")
        end

        block_id = def.block_id
        width = def.multiblock_size.x
        height = def.multiblock_size.y
    end

    core._set_multiblock(x, y, width, height, block_id)

    if not nocall then
        def.on_place({ x = x, y = y, is_wall = false })
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

    def.on_place({ x = x, y = y, is_wall = false }, user)

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

    if fg and def and def.multiblock_size then
        local origin = core._get_multiblock_origin(x, y)

        x = origin.x
        y = origin.y
    end

    local pos = vec2.new(x, y)

    -- If def is nil, then simply remove that unknown block. If it is not nil,
    -- and on_destroy returns false, we are not allowed to dig block, don't dig
    if def ~= nil then
        if not def.on_destroy(pos, user) then
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
    if fg and def and def.multiblock_size then
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
    if fg and def and def.multiblock_size then
        core._set_multiblock(x, y, def.multiblock_size.x, def.multiblock_size.y, 0)
    end

    if def then
        local w = def.multiblock_size and def.multiblock_size.x or 1
        local h = def.multiblock_size and def.multiblock_size.y or 1

        for px = x, x + w-1 or 0 do
            for py = y, y + h-1 or 0 do
                core._add_particle_spawner(def.dig_particles_def, { x = px, y = py })
            end
        end
    end

    if def ~= nil and def.drop ~= "" then
        terrarium.drop_item(ItemStack(def.drop), pos)
    end

    if def ~= nil and def.dig_sound ~= nil and not nosound then
        core._play_sound({
            name = def.dig_sound.name,
            volume = def.dig_sound.volume,
            pitch = def.dig_sound.pitch,

            source = pos,
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

    def.on_place({ x = x, y = y, is_wall = not fg }, user)

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

function terrarium.start_block_timer(x, y, timer)
    local origin = core._get_multiblock_origin(x, y)
    core._start_block_timer(origin.x, origin.y, timer)
end

terrarium.start_wall_timer = core._start_wall_timer

function terrarium.get_block_inventory(x, y)
    return core._get_block_inventory(x, y)
end

core._event_handlers["BlockActivate"] = function(event)
    local block_id = core._get_block(event.block_event.position.x, event.block_event.position.y)

    local block_def = terrarium.registered_blocks[terrarium.block_names[block_id]]

    block_def.on_interact(event.block_event.position, core.get_user(event.block_event.user))
end

core._on_block_timer = function(x, y, id, is_wall)
    local block_def = terrarium.registered_blocks[terrarium.block_names[id]]

    if block_def and block_def.on_timer then
        block_def.on_timer({ x = x, y = y, is_wall = is_wall })
    end
end
