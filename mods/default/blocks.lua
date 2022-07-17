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

terrarium.register_block_alias("default:stone", "mapgen:filler_block")
terrarium.register_block_alias("default:stone", "mapgen:filler_wall")
