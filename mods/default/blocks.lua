terrarium.register_block("default:stone", {
    image = "stone.png",

    slippery = 0.2,

    is_solid = true,
})

terrarium.register_block("default:dirt", {
    image = "dirt.png",

    slippery = 0.8,

    is_solid = true,
})

terrarium.register_block("default:grass", {
    image = "grass.png",

    slippery = 0.4,

    is_solid = true,
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
})

terrarium.register_block_alias("default:stone", "mapgen:filler_block")
terrarium.register_block_alias("default:stone", "mapgen:filler_wall")
