terrarium.register_block("default:stone", {
    description = "Stone",

    draw_type = "Autotile",
    image = "stone.png",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "stone_item.png",
})

terrarium.register_block("default:ice", {
    description = "Ice block",

    draw_type = "Autotile",
    image = "ice.png",

    slippery = 0.85,

    solid = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "ice_item.png",
})

terrarium.register_block("default:obsidian", {
    description = "Obsidian",

    image = "obsidian.png",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 3,
    },
})

terrarium.register_block("default:copper_ore", {
    description = "Copper ore",

    draw_type = "Autotile",
    image = "copper.png",

    slippery = 0.7,

    solid = true,

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

    draw_type = "Autotile",
    image = "iron.png",

    slippery = 0.2,

    solid = true,

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

terrarium.register_block("default:silver_ore", {
    description = "Silver ore",

    draw_type = "Autotile",
    image = "silver.png",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },

    dig_sound = {
        name = "dig_metal.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    inventory_image = "silver_ore.png",
    max_count = 999,
})

terrarium.register_block("default:gold_ore", {
    description = "Gold ore",

    draw_type = "Autotile",
    image = "gold.png",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
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

    draw_type = "Autotile",
    image = "dirt.png",

    slippery = 0.8,

    solid = true,

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

    inventory_image = "dirt_item.png",
})

terrarium.register_block("default:grass", {
    description = "Grassy dirt block",

    image = "grass_dirt.png",
    draw_type = "Autotile",
    inventory_image = "grass_block.png",

    drop = "default:dirt",

    slippery = 0.4,

    solid = true,

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

    draw_type = "Autotile",
    image = "snow.png",

    slippery = 0.6,

    solid = true,

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

    inventory_image = "snow_item.png",
})

terrarium.register_block("default:overgrowth_grass", {
    description = "Overgrowth grassy dirt block",

    draw_type = "Autotile",
    image = "grass_block.png",

    slippery = 0.4,

    solid = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "grass_block_item.png",

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

for i = 0, 3 do
    terrarium.register_block("default:grass_decor_"..i, {
        description = "Grass",

        image = "grass"..i..".png",

        autotile_neighbour = false,
        solid = false,
        replacable = true,

        drop = "",

        groups = {
            ["ground"] = 1,
        },

        on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
    })
end

terrarium.register_block("default:bush", {
    description = "Bush",

    draw_type = "Multiblock",
    image = "bush.png",
    multiblock_size = { x = 3, y = 2 },

    autotile_neighbour = false,
    solid = false,
    replacable = true,

    drop = "",

    groups = {
        ["ground"] = 1,
    },

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:cactus", {
    description = "Cactus",

    draw_type = "Autotile",
    image = "cactus.png",

    solid = false,

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

    inventory_image = "cactus_item.png",
})

terrarium.register_block("default:glow_cactus", {
    description = "Glowy cactus",

    draw_type = "Autotile",
    image = "glow_cactus.png",

    solid = false,

    light = { r = 120, g = 120, b = 20 },

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

    inventory_image = "glow_cactus_item.png",
})

terrarium.register_block("default:wood", {
    description = "Wood",

    draw_type = "Autotile",
    image = "wood_block.png",

    solid = true,

    groups = {
        ["ground"] = 1,
    },

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

terrarium.register_block("default:wood_platform", {
    description = "Wood platform",

    image = "wood_platform.png",

    draw_type = "AutotilePlatform",
    autotile_single = true,
    autotile_neighbour = false,
    blocks_light = false,

    solid = true,
    platform = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "wood_platform_item.png",

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

terrarium.register_block("default:old_bricks_platform", {
    description = "Old platform",

    image = "old_bricks_platform.png",

    draw_type = "AutotilePlatform",
    autotile_single = true,
    autotile_neighbour = false,
    blocks_light = false,

    solid = true,
    platform = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "old_bricks_platform_item.png",
})

terrarium.register_block("default:sand", {
    description = "Sand",

    draw_type = "Autotile",
    image = "sand.png",

    slippery = 0.4,

    solid = true,

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

    inventory_image = "sand_item.png",
})

terrarium.register_block("default:glass", {
    description = "Glass",

    draw_type = "Autotile",
    image = "glass.png",

    slippery = 0.7,

    solid = true,

    blocks_light = false,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "glass_item.png",
})

terrarium.register_block("default:stone_bricks", {
    description = "Stone bricks",

    draw_type = "Autotile",
    image = "stone_bricks.png",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "stone_bricks_item.png",
})

terrarium.register_block("default:old_bricks", {
    description = "Old bricks",

    draw_type = "Autotile",
    image = "old_bricks.png",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "old_bricks_item.png",
})

terrarium.register_block("default:old_broken_bricks", {
    description = "Old broken bricks",

    draw_type = "Autotile",
    image = "old_broken_bricks.png",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "old_broken_bricks_item.png",
})

terrarium.register_block("default:roof_tiles", {
    description = "Roof tiles",

    draw_type = "Autotile",
    image = "roof_tiles.png",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },

    inventory_image = "roof_tiles_item.png",
})

terrarium.register_block("default:metal_block", {
    description = "Metal block",

    image = "metal_block.png",
    inventory_image = "metal_block_item.png",

    draw_type = "Autotile",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:metal_beam", {
    description = "Metal beam",

    image = "metal_beam.png",
    inventory_image = "metal_beam_item.png",

    draw_type = "Autotile",
    autotile_single = true,

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:lab_bricks", {
    description = "Lab bricks",

    image = "lab_bricks.png",
    inventory_image = "lab_bricks_item.png",

    draw_type = "Autotile",

    slippery = 0.2,

    solid = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:glow_metal_block", {
    description = "Glow metal block",

    image = "glow_metal_block.png",
    inventory_image = "glow_metal_block_item.png",

    draw_type = "Autotile",

    slippery = 0.2,

    solid = true,

    light = { r = 20, g = 30, b = 100 },

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:metal_platform", {
    description = "Metal platform",

    image = "metal_platform.png",
    inventory_image = "metal_platform_item.png",

    draw_type = "AutotilePlatform",
    autotile_single = true,
    autotile_neighbour = false,
    blocks_light = false,

    slippery = 0.2,

    solid = true,
    platform = true,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:chest", {
    description = "Chest",

    image = "chest.png",

    autotile_neighbour = false,
    solid = false,

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
    solid = false,

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
    solid = false,

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
    solid = false,

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

local function register_door(name, def)
    terrarium.register_block(name, {
        description = def.description,

        image = def.closed_image,
        inventory_image = def.inventory_image,

        autotile_neighbour = false,
        solid = true,

        multiblock_size = def.size,

        groups = def.groups or {
            ["ground"] = 1,
        },

        on_interact = function(position)
            local origin = terrarium.get_multiblock_origin(position.x, position.y)

            terrarium.set_multiblock(origin.x, origin.y, name.."_open")
        end,
    })

    terrarium.register_block(name.."_open", {
        description = def.description.." (open)",

        image = def.open_image,
        inventory_image = def.inventory_image,

        drop = name,

        autotile_neighbour = false,
        solid = false,

        multiblock_size = def.size,

        groups = def.groups or {
            ["ground"] = 1,
        },

        on_interact = function(position)
            local origin = terrarium.get_multiblock_origin(position.x, position.y)

            terrarium.set_multiblock(origin.x, origin.y, name)
        end,
    })
end

register_door("default:steel_door", {
    description = "Steel Door",

    closed_image = "steel_door.png",
    open_image = "steel_door_open.png",
    inventory_image = "steel_door_item.png",

    size = { x = 1, y = 3, },
})

register_door("default:steel_medium_door", {
    description = "Medium Steel Door",

    closed_image = "steel_medium_door.png",
    open_image = "steel_medium_door_open.png",
    inventory_image = "steel_door_item.png",

    size = { x = 2, y = 3, },
})

register_door("default:steel_hatch", {
    description = "Steel Hatch",

    closed_image = "steel_hatch.png",
    open_image = "steel_hatch_open.png",
    inventory_image = "steel_hatch_item.png",

    size = { x = 3, y = 1, },
})

-- Big F-ing Door
register_door("default:steel_big_door", {
    description = "Big Steel Door",

    closed_image = "steel_big_door.png",
    open_image = "steel_big_door_open.png",
    inventory_image = "steel_door_item.png",

    size = { x = 2, y = 4, },
})

terrarium.register_block("default:picture_sungarden", {
    description = "Painting\n\"Sunflower's Garden!\"",

    image = "sungarden.png",
    autotile_neighbour = false,
    solid = false,

    multiblock_size = { x = 2, y = 2, },

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
})

terrarium.register_block("default:picture_starbound", {
    description = "Painting\n\"Starbound!\"",

    image = "starbound.png",
    autotile_neighbour = false,
    solid = false,

    multiblock_size = { x = 2, y = 2, },

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
})

terrarium.register_block("default:picture_fediverse", {
    description = "Painting\n\"Fediverse!\"",

    image = "fediverse.png",
    autotile_neighbour = false,
    solid = false,

    multiblock_size = { x = 4, y = 4, },

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
})

terrarium.register_block("default:vine", {
    description = "Vine",

    image = "vine.png",

    autotile_neighbour = false,
    solid = false,

    groups = {
        ["ground"] = 1,
    },

    on_neighbour_destroy = terrarium.connected_to_blocks({"up"}),
})

terrarium.register_block("default:torch", {
    description = "Torch",

    image = "torch.png",

    autotile_neighbour = false,
    solid = false,

    light = { r = 255, g = 180, b = 140 },

    groups = {
        ["ground"] = 1,
    },

    on_select = function(user)
        user.ref:set_light({ r = 230, g = 150, b = 110 })
    end,

    on_unselect = function(user)
        user.ref:set_light({ r = 0, g = 0, b = 0 })
    end,
})

local crystals = {
    { name = "Cyan",   light = color.new(60, 80, 120) },
    { name = "Green",  light = color.new(60, 120, 80) },
    { name = "Purple", light = color.new(120, 60, 120) },
    { name = "Red", light = color.new(120, 80, 60) },
    { name = "White", light = color.new(120, 120, 120) },
    { name = "Yellow", light = color.new(120, 120, 60) },
}

for _, crystal in ipairs(crystals) do
    terrarium.register_block("default:"..crystal.name:lower().."_crystal", {
        description = crystal.name.." crystal",

        image = crystal.name:lower().."_crystal_block.png",

        autotile_neighbour = false,
        solid = false,

        light = crystal.light,

        groups = {
            ["ground"] = 1,
        },

        inventory_image = crystal.name:lower().."_crystal.png",
    })
end

terrarium.register_block("default:alarm_lamp", {
    description = "Alarm Lamp",

    image = "alarm_lamp.png",

    autotile_neighbour = false,
    solid = false,

    light = { r = 255, g = 30, b = 10 },

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:lab_lamp", {
    description = "Lab Lamp",

    image = "lab_lamp.png",

    autotile_neighbour = false,
    solid = false,

    light = { r = 245, g = 250, b = 255 },

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:lamp", {
    description = "Lamp",

    image = "lamp.png",

    autotile_neighbour = false,
    solid = false,

    light = { r = 255, g = 255, b = 220 },

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:ai_projector", {
    description = "AI Projector",

    image = "ai_projector.png",

    autotile_neighbour = false,
    solid = false,

    on_interact = function(position, user)
        local size = terrarium.registered_entities["default:ai_holo"].size
        local holo_pos = position + vec2.new(0.5, 1) - vec2.new(size.x/2, size.y)

        if user.ai_holo and user.ai_holo.ref:is_valid() then
            user.ai_holo:reset_position(holo_pos)
        else
            user.ai_holo = terrarium.new_entity("default:ai_holo", holo_pos, {user=user})
        end
    end,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:lab_monitor", {
    description = "Lab monitor",

    image = "lab_monitor_off.png",

    multiblock_size = { x = 2, y = 1, },

    autotile_neighbour = false,
    solid = false,

    on_interact = function(position)
        local origin = terrarium.get_multiblock_origin(position.x, position.y)

        terrarium.set_multiblock(origin.x, origin.y, "default:lab_monitor_on")
    end,

    groups = {
        ["ground"] = 1,
    },
})

terrarium.register_block("default:lab_monitor_on", {
    description = "Lab monitor",

    image = "lab_monitor_on.png",

    drop = "default:lab_monitor",

    multiblock_size = { x = 2, y = 1, },

    autotile_neighbour = false,
    solid = false,

    on_interact = function(position)
        local origin = terrarium.get_multiblock_origin(position.x, position.y)

        terrarium.set_multiblock(origin.x, origin.y, "default:lab_monitor")
    end,

    groups = {
        ["ground"] = 1,
    },

    light = color.new(120, 170, 230),
})

terrarium.register_block("default:lab_weapon_container_vm", {
    description = "Lab weapon container (VM class)",

    image = "lab_weapon_container_vm.png",

    autotile_neighbour = false,
    solid = false,

    multiblock_size = { x = 2, y = 2 },

    groups = {
        ["ground"] = 1,
    },

    light = color.new(120, 170, 230),

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

terrarium.register_block_alias("default:stone", "mapgen:filler_block")
terrarium.register_block_alias("default:stone", "mapgen:filler_wall")
