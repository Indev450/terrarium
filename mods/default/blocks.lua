terrarium.register_block("default:stone", {
    description = "Stone",

    image = "stone.png",

    slippery = 0.2,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:ice", {
    description = "Ice block",

    image = "ice.png",

    slippery = 0.85,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:obsidian", {
    description = "Obsidian",

    image = "obsidian.png",

    slippery = 0.2,

    is_solid = true,

    groups = {
        ["ground"] = 3,
    },
})

terrarium.register_block("default:copper_ore", {
    description = "Copper ore",

    image = "copper.png",

    slippery = 0.7,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_metal.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    inventory_image = "copper_ore.png",
    max_count = 999,
})

terrarium.register_block("default:iron_ore", {
    description = "Iron ore",

    image = "iron.png",

    slippery = 0.2,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_metal.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    inventory_image = "iron_ore.png",
    max_count = 999,
})

terrarium.register_block("default:gold_ore", {
    description = "Gold ore",

    image = "gold.png",

    slippery = 0.2,

    is_solid = true,

    groups = {
        ["ground"] = 2,
    },

    dig_sound = {
        name = "dig_metal.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    inventory_image = "gold_ore.png",
    max_count = 999,
})

terrarium.register_block("default:dirt", {
    description = "Dirt",

    image = "dirt.png",

    slippery = 0.8,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.4,
    },
})

terrarium.register_block("default:grass", {
    description = "Grassy dirt block",

    image = "grass_dirt.png",
    draw_type = "Autotile",
    inventory_image = "grass_block.png",

    drop = "default:dirt",

    slippery = 0.4,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.4,
    },
})

terrarium.register_block("default:snow", {
    description = "Snow block",

    image = "snow.png",

    slippery = 0.6,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.4,
    },
})

terrarium.register_block("default:overgrowth_grass", {
    description = "Overgrowth grassy dirt block",

    image = "grass_block.png",

    slippery = 0.4,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.4,
    },
})

terrarium.register_block("default:grass_decor", {
    description = "Grass",

    image = "grass.png",

    autotile_neighbour = false,
    is_solid = false,
    replacable = true,

    drop = "",

    groups = {
        ["ground"] = 1,
    },

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:cactus", {
    description = "Cactus",

    image = "cactus.png",

    autotile_neighbour = false,
    is_solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.2,
    },
})

terrarium.register_block("default:wood", {
    description = "Wood",

    image = "wood_block.png",

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },

    description = "Wood",
    inventory_image = "wood.png",

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.2,
    },
})

terrarium.register_block("default:sand", {
    description = "Sand",

    image = "sand.png",

    slippery = 0.4,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_soil.wav",
        volume = 1.0,
        pitch = 1.4,
    },
})

terrarium.register_block("default:glass", {
    description = "Glass",

    image = "glass.png",

    slippery = 0.7,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:stone_bricks", {
    description = "Stone bricks",

    image = "stone_bricks.png",

    slippery = 0.2,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:chest", {
    description = "Chest",

    image = "chest.png",

    autotile_neighbour = false,
    is_solid = false,

    multiblock_size = { x = 2, y = 2 },

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.2,
    },

    on_place = function(position, user)
        local origin = terrarium.get_multiblock_origin(position.x, position.y)

        terrarium.get_block_inventory(origin.x, origin.y):resize(8*4)
    end,

    on_interact = function(position, user)
        local origin = terrarium.get_multiblock_origin(position.x, position.y)

        user:open_inventory_ui(terrarium.get_block_inventory(origin.x, origin.y), 8, 4)
    end,

    on_destroy = function(position, user)
        local origin = terrarium.get_multiblock_origin(position.x, position.y)

        if user ~= nil and user.ref.is_player then
            local inventory = terrarium.get_block_inventory(origin.x, origin.y)
            local user_inventory = user.ref:get_player_inventory()

            for i = 0, (8*4 - 1) do
                user_inventory:add_item(inventory:get(i))
            end

            inventory:resize(0)

            return true
        end

        return false
    end,
})

terrarium.register_block("default:workbench", {
    description = "Workbench",

    image = "workbench.png",

    autotile_neighbour = false,
    is_solid = false,

    multiblock_size = { x = 2, y = 1 },

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    place_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.2,
    },

    on_interact = function(position, user)
        user:show_crafting_ui("workbench")
    end,
})

terrarium.register_block("default:furnace", {
    description = "Furnace",

    image = "furnace.png",

    autotile_neighbour = false,
    is_solid = false,

    multiblock_size = { x = 2, y = 2 },

    groups = {
        ["ground"] = 1,
    },

    on_interact = function(position, user)
        user:show_crafting_ui("furnace")
    end,
})

terrarium.register_block("default:anvil", {
    description = "Anvil",

    image = "anvil.png",

    autotile_neighbour = false,
    is_solid = false,

    multiblock_size = { x = 2, y = 1 },

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_metal.wav",
        volume = 1.0,
        pitch = 0.6,
    },

    place_sound = {
        name = "dig_metal.wav",
        volume = 1.0,
        pitch = 0.8,
    },

    on_interact = function(position, user)
        user:show_crafting_ui("anvil")
    end,
})

terrarium.register_block("default:vine", {
    description = "Vine",

    image = "vine.png",

    autotile_neighbour = false,
    is_solid = false,

    groups = {
        ["ground"] = 1,
    },

    on_neighbour_destroy = terrarium.connected_to_blocks({"up"}),
})

terrarium.register_block_alias("default:stone", "mapgen:filler_block")
terrarium.register_block_alias("default:stone", "mapgen:filler_wall")
