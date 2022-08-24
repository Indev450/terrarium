terrarium.register_block("default:stone", {
    image = "stone.png",

    slippery = 0.2,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:copper_ore", {
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
    image = "grass.png",

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

terrarium.register_block("default:cactus", {
    image = "cactus.png",

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
    image = "glass.png",

    slippery = 0.7,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:stone_bricks", {
    image = "stone_bricks.png",

    slippery = 0.2,

    is_solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:chest", {
    image = "chest.png",

    is_solid = false,

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
        terrarium.get_block_inventory(position.x, position.y):resize(8*4)
    end,

    on_interact = function(position, user)
        user:open_inventory_ui(terrarium.get_block_inventory(position.x, position.y), 8, 4)
    end,

    on_destroy = function(position, user)
        if user ~= nil and user.ref.is_player then
            local inventory = terrarium.get_block_inventory(position.x, position.y)
            local user_inventory = user.ref:get_player_inventory()

            for i = 0, (8*4 - 1) do
                user_inventory:add_item(inventory:get(i))
            end

            return true
        end

        return false
    end,
})

terrarium.register_block("default:workbench", {
    image = "workbench.png",

    is_solid = false,

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
    image = "furnace.png",

    is_solid = false,

    groups = {
        ["ground"] = 1,
    },

    on_interact = function(position, user)
        user:show_crafting_ui("furnace")
    end,
})

terrarium.register_block("default:anvil", {
    image = "anvil.png",

    is_solid = false,

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

terrarium.register_block_alias("default:stone", "mapgen:filler_block")
terrarium.register_block_alias("default:stone", "mapgen:filler_wall")
