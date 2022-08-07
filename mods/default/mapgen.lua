terrarium.register_biome("default:grassland", {
    humidity_min = -0.2,
    humidity_max = 0.6,

    heat_min = -0.2,
    heat_max = 0.8,

    top = {
        block = "default:grass",
        wall = "default:grass",
    },

    filler = {
        block = "default:dirt",
        wall = "default:dirt",
    },

    filler_depth = 8,

    stone = {
        block = "default:stone",
        wall = "default:dirt",
    },

    priority = 1,
})

terrarium.register_biome("default:grassland_overgrowth", {
    humidity_min = 0.6,

    heat_min = 0.4,

    top = {
        block = "default:grass",
        wall = "default:grass",
    },

    top_depth = 4,

    filler = {
        block = "default:dirt",
        wall = "default:grass",
    },

    filler_depth = 8,

    stone = {
        block = "default:stone",
        wall = "default:grass",
    },

    priority = 2,
})

terrarium.register_biome("default:desert", {
    humidity_max = 0.2,

    heat_min = -0.4,

    top = {
        block = "default:sand",
        wall = "default:sand",
    },

    top_depth = 1,

    filler = {
        block = "default:sand",
        wall = "default:sand",
    },

    filler_depth = 1,

    stone = {
        block = "default:sand",
        wall = "default:sand",
    },

    priority = 2,
})
