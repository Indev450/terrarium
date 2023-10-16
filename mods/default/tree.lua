terrarium.register_block("default:sapling", {
    description = "Sapling",

    image = "sapling_block.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    on_place = function(position)
        if position.is_wall then return false end

        -- Takes from 3 to 6 minutes to grow (would probably adjust that later)
        terrarium.start_block_timer(position.x, position.y, math.random()*60*3 + 60*3)
    end,

    on_timer = function(position)
        if not default.try_place_random_tree(position.x, position.y) then
            -- 30-60 seconds to try to grow again
            terrarium.start_block_timer(position.x, position.y, math.random()*30 + 30)
        end
    end,

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),

    inventory_image = "sapling.png",

    max_count = 999,
})

terrarium.register_block("default:tree_trunk_root_both", {
    image = "tree_trunk_root_both.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk_root_left", {
    image = "tree_trunk_root_left.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk_root_right", {
    image = "tree_trunk_root_right.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_root_left", {
    image = "tree_root_left.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood",

    on_neighbour_destroy = terrarium.connected_to_blocks({"right"}),
})

terrarium.register_block("default:tree_root_right", {
    image = "tree_root_right.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood",

    on_neighbour_destroy = terrarium.connected_to_blocks({"left"}),
})

terrarium.register_block("default:tree_trunk", {
    image = "tree_trunk.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk_branch_both", {
    image = "tree_trunk_branch_both.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk_branch_left", {
    image = "tree_trunk_branch_left.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk_branch_right", {
    image = "tree_trunk_branch_right.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_heavy,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_top", {
    draw_type = "Multiblock",
    image = "tree_top.png",
    multiblock_size = { x = 3, y = 3 },

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_dust,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_branch_left", {
    image = "tree_branch_left.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_dust,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:sapling",

    on_neighbour_destroy = terrarium.connected_to_blocks({"right"}),
})

terrarium.register_block("default:tree_branch_right", {
    image = "tree_branch_right.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_particles_def = default.particle_dust,

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:sapling",

    on_neighbour_destroy = terrarium.connected_to_blocks({"left"}),
})

local function is_replacable(x, y)
    local name = terrarium.get_block(x, y)

    if not name then return true end

    local def = terrarium.registered_blocks[name]

    return def == nil or def.replacable
end

function default.try_place_random_tree(x, y)
    if terrarium.get_block(x, y+1) == nil then return false end

    local root = nil
    local root_left = false
    local root_right = false

    if terrarium.get_block(x-1, y+1) and is_replacable(x-1, y) then
        root = "default:tree_trunk_root_left"
        root_left = true
    end

    if terrarium.get_block(x+1, y+1) and is_replacable(x+1, y) then
        root = root and "default:tree_trunk_root_both" or "default:tree_trunk_root_right"
        root_right = true
    end

    if root == nil then return false end

    local height = math.random(10, 14)

    for check_y = y-height+3, y-1 do
        if not is_replacable(x, check_y) then return false end
    end

    for check_x = x-1, x+1 do
        for check_y = y - height, y - height + 2 do
            if not is_replacable(x, check_y) then return false end
        end
    end

    terrarium.set_block(x, y, root)

    if root_left then terrarium.set_block(x-1, y, "default:tree_root_left") end
    if root_right then terrarium.set_block(x+1, y, "default:tree_root_right") end

    -- Don't place branches right next to tree top...
    local prev_placed_branches = true
    for place_y = y-height+3, y-1 do
        local trunk = "default:tree_trunk"

        -- ...and next to roots too.
        if place_y == y-1 then prev_placed_branches = true end

        if not prev_placed_branches and math.random() < 0.75 then
            local branch_left = is_replacable(x-1, place_y) and math.random() < 0.5
            local branch_right = is_replacable(x+1, place_y) and math.random() < 0.5

            prev_placed_branches = true

            if branch_left and branch_right then
                trunk = "default:tree_trunk_branch_both"
                terrarium.set_block(x-1, place_y, "default:tree_branch_left")
                terrarium.set_block(x+1, place_y, "default:tree_branch_right")
            elseif branch_left then
                trunk = "default:tree_trunk_branch_left"
                terrarium.set_block(x-1, place_y, "default:tree_branch_left")
            elseif branch_right then
                trunk = "default:tree_trunk_branch_right"
                terrarium.set_block(x+1, place_y, "default:tree_branch_right")
            else
                prev_placed_branches = false
            end
        else
            prev_placed_branches = false
        end

        terrarium.set_block(x, place_y, trunk)
    end

    terrarium.set_multiblock(x-1, y-height, "default:tree_top")

    return true
end

function default.random_tree(place_chance)
    local tile_aliases = {
        ['I'] = { block = "default:tree_trunk_root_both" },
        ['j'] = { block = "default:tree_trunk_root_left" },
        ['l'] = { block = "default:tree_trunk_root_right" },
        ['/'] = { block = "default:tree_root_left" },
        ['\\'] = { block = "default:tree_root_right" },
        ['|'] = { block = "default:tree_trunk" },
        ['T'] = { block = "default:tree_trunk_branch_both" },
        ['q'] = { block = "default:tree_trunk_branch_left" },
        ['p'] = { block = "default:tree_trunk_branch_right" },
        ['<'] = { block = "default:tree_branch_left" },
        ['>'] = { block = "default:tree_branch_right" },

        -- Still terrible but at least now it's an actual multiblock
        ['0'] = { block = "default:tree_top", multiblock_offset = { x = 0, y = 0 } },
        ['1'] = { block = "default:tree_top", multiblock_offset = { x = 1, y = 0 } },
        ['2'] = { block = "default:tree_top", multiblock_offset = { x = 2, y = 0 } },
        ['3'] = { block = "default:tree_top", multiblock_offset = { x = 0, y = 1 } },
        ['4'] = { block = "default:tree_top", multiblock_offset = { x = 1, y = 1 } },
        ['5'] = { block = "default:tree_top", multiblock_offset = { x = 2, y = 1 } },
        ['6'] = { block = "default:tree_top", multiblock_offset = { x = 0, y = 2 } },
        ['7'] = { block = "default:tree_top", multiblock_offset = { x = 1, y = 2 } },
        ['8'] = { block = "default:tree_top", multiblock_offset = { x = 2, y = 2 } },
    }

    local root_variants = {
        "/I\\",
        "/j ",
        " l\\",
    }

    local root_conditions = {}

    for i = 1, #root_variants do
        root_conditions[i] = {}

        for j = 1, 3 do
            if root_variants[i]:sub(j, j) ~= ' ' then
                table.insert(root_conditions[i], {
                    position = { x = j - 2, y = 1 }, -- -2 because 1 is lua index offset, and another 1 is offset from origin, which is 1 for x
                    fg = { type = "AnySolidBlock" },
                })
            end
        end
    end

    local trunk_variants = {
        "<T>",
        "<q ",
        " p>",
        " | ",
        " | ",
        " | ",
        " | ",
        " | ",
        " | ",
    }

    local tree_top = {
        "012",
        "345",
        "678",
    }

    local trunk_height = math.random(8, 12)
    local root_variant = math.random(1, #root_variants)

    local tree_height = #tree_top + 1 + trunk_height

    local origin = { x = 1, y = tree_height - 1, }

    local conditions = root_conditions[root_variant]

    local model = {}

    for i = 1, #tree_top do
        table.insert(model, tree_top[i])
    end

    local prev_variant = 0
    for i = 1, trunk_height do
        if prev_variant > 3 then
            prev_variant = math.random(1, #trunk_variants)
        else
            prev_variant = 4
        end

        table.insert(model, trunk_variants[prev_variant])
    end

    table.insert(model, root_variants[root_variant])

    return { origin = origin, tile_aliases = tile_aliases, tiles = model, conditions = conditions, place_chance = place_chance, }
end
