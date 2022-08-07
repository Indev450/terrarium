terrarium.register_block("default:stone", {
    image = "stone.png",

    slippery = 0.2,

    is_solid = true,
})

terrarium.register_block("default:dirt", {
    image = "dirt.png",

    slippery = 0.8,

    is_solid = true,

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

terrarium.register_block("default:chest", {
    image = "chest.png",

    is_solid = false,

    is_interactive = true,

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

terrarium.register_block_alias("default:stone", "mapgen:filler_block")
terrarium.register_block_alias("default:stone", "mapgen:filler_wall")
