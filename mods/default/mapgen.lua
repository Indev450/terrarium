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

    max_depth = 0.2,

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

    max_depth = 0.2,

    priority = 2,
})

terrarium.register_biome("default:desert", {
    humidity_max = 0.9,

    heat_min = -0.9,

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

    decorations = {
        cactus = {
            origin = {
                x = 2,
                y = 4,
            },

            tile_aliases = {
                ['c'] = { block = "default:grass" },
            },

            tiles = {
                "  c  ",
                "c c c",
                "ccccc",
                "  c  ",
                "  c  ",
            },

            place_chance = 0.05,
        }
    }
})

terrarium.register_ore({
    cluster_tiles = 12,

    tile = {
        block = "default:copper_ore",
    }
})
