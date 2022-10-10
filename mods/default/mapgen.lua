terrarium.register_biome("default:grassland", {
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

    decorations = {
        tree = {
            origin = {
                x = 1,
                y = 11,
            },

            tile_aliases = {
                ['I'] = { block = "default:tree_base" },
                ['|'] = { block = "default:tree_trunk" },
                ['q'] = { block = "default:tree_trunk_branch_left" },
                ['p'] = { block = "default:tree_trunk_branch_right" },
                ['A'] = { block = "default:tree_trunk_top" },
                ['<'] = { block = "default:tree_branch_left" },
                ['>'] = { block = "default:tree_branch_right" },
            },

            tiles = {
                " A ",
                " p>",
                "<q ",
                " | ",
                " p>",
                " p>",
                " | ",
                "<q ",
                " | ",
                " p>",
                " | ",
                " I ",
            },

            place_chance = 0.05,
        },

        grass = terrarium.single_tile_decor {
            tile = { block = "default:grass_decor" },
            place_chance = 0.9,
        },
    }
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
        block = "default:grass",
        wall = "default:grass",
    },

    filler_depth = 8,

    stone = {
        block = "default:dirt",
        wall = "default:grass",
    },

    max_depth = 0.2,

    priority = 2,

    decorations = {
        grass = terrarium.single_tile_decor {
            tile = { block = "default:grass_decor" },
            place_chance = 1,
        },
    },
})

terrarium.register_biome("default:desert", {
    humidity_max = 0.4,

    heat_min = -0.2,

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
                ['c'] = { block = "default:cactus" },
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

    max_depth = 0.4,

    tile = {
        block = "default:copper_ore",
    },
})

terrarium.register_ore({
    cluster_tiles = 18,

    min_depth = 0.4,

    tile = {
        block = "default:copper_ore",
    },
})


terrarium.register_ore({
    cluster_tiles = 12,
    distribution = 64,

    min_depth = 0.2,

    max_depth = 0.6,

    tile = {
        block = "default:iron_ore",
    },
})

terrarium.register_ore({
    cluster_tiles = 18,
    distribution = 64,

    min_depth = 0.6,

    tile = {
        block = "default:iron_ore",
    },
})

terrarium.register_ore({
    cluster_tiles = 12,
    distribution = 64,

    min_depth = 0.4,

    tile = {
        block = "default:gold_ore",
    },
})
