local TREE_TYPES = 8

local grassland_biome = {
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
        vine = {
            origin = {
                x = 0,
                y = 0
            },

            tile_aliases = {
                ['|'] = { block = "default:vine" },
            },

            tiles = {
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
            },

            conditions = {
                {
                    position = { x = 0, y = -1 },
                    fg = { type = "AnySolidBlock" },
                },
            },

            place_chance = 0.1,
        },

        long_vine = {
            origin = {
                x = 0,
                y = 0
            },

            tile_aliases = {
                ['|'] = { block = "default:vine" },
            },

            tiles = {
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
            },

            conditions = {
                {
                    position = { x = 0, y = -1 },
                    fg = { type = "AnySolidBlock" },
                },
            },

            place_chance = 0.05,
        },

        bush = {
            origin = { x = 1, y = 1, },

            tile_aliases = {
                ['0'] = { block = "default:bush", multiblock_offset = { x = 0, y = 0 } },
                ['1'] = { block = "default:bush", multiblock_offset = { x = 1, y = 0 } },
                ['2'] = { block = "default:bush", multiblock_offset = { x = 2, y = 0 } },
                ['3'] = { block = "default:bush", multiblock_offset = { x = 0, y = 1 } },
                ['4'] = { block = "default:bush", multiblock_offset = { x = 1, y = 1 } },
                ['5'] = { block = "default:bush", multiblock_offset = { x = 2, y = 1 } },
            },

            tiles = {
                "012",
                "345",
            },

            conditions = {
                {
                    position = { x = -1, y = 1, },
                    fg = { type = "AnySolidBlock" },
                },
                {
                    position = { x = 0, y = 1, },
                    fg = { type = "AnySolidBlock" },
                },
                {
                    position = { x = 1, y = 1, },
                    fg = { type = "AnySolidBlock" },
                },
            },

            place_chance = 0.1,
        }
    }
}

for i = 1, TREE_TYPES do
    grassland_biome.decorations["tree"..i] = default.random_tree(0.2/TREE_TYPES)
end

for i = 0, 3 do
    grassland_biome.decorations["grass"..i] = terrarium.single_tile_decor {
        tile = { block = "default:grass_decor_"..i },
        place_chance = 0.2,
        conditions = {
            {
                position = { x = 0, y = 1 },
                fg = { type = "AnySolidBlock" }
            }
        },
    }
end

terrarium.register_biome("default:grassland", grassland_biome)

terrarium.register_biome("default:grassland_overgrowth", {
    humidity_min = 0.6,

    heat_min = 0.4,

    top = {
        block = "default:overgrowth_grass",
        wall = "default:overgrowth_grass",
    },

    top_depth = 4,

    filler = {
        block = "default:overgrowth_grass",
        wall = "default:overgrowth_grass",
    },

    filler_depth = 8,

    stone = {
        block = "default:dirt",
        wall = "default:overgrowth_grass",
    },

    max_depth = 0.2,

    priority = 2,

    decorations = {
        grass0 = terrarium.single_tile_decor {
            tile = { block = "default:grass_decor_0" },
            place_chance = 0.2,
            conditions = {
                {
                    position = { x = 0, y = 1 },
                    fg = { type = "AnySolidBlock" }
                }
            },
        },

        grass1 = terrarium.single_tile_decor {
            tile = { block = "default:grass_decor_1" },
            place_chance = 0.2,
            conditions = {
                {
                    position = { x = 0, y = 1 },
                    fg = { type = "AnySolidBlock" }
                }
            },
        },

        grass2 = terrarium.single_tile_decor {
            tile = { block = "default:grass_decor_2" },
            place_chance = 0.2,
            conditions = {
                {
                    position = { x = 0, y = 1 },
                    fg = { type = "AnySolidBlock" }
                }
            },
        },

        grass3 = terrarium.single_tile_decor {
            tile = { block = "default:grass_decor_3" },
            place_chance = 0.2,
            conditions = {
                {
                    position = { x = 0, y = 1 },
                    fg = { type = "AnySolidBlock" }
                }
            },
        },

        vine = {
            origin = {
                x = 0,
                y = 0
            },

            tile_aliases = {
                ['|'] = { block = "default:vine" },
            },

            tiles = {
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
            },

            conditions = {
                {
                    position = { x = 0, y = -1 },
                    fg = { type = "AnySolidBlock" },
                },
            },

            place_chance = 0.4,
        },

        long_vine = {
            origin = {
                x = 0,
                y = 0
            },

            tile_aliases = {
                ['|'] = { block = "default:vine" },
            },

            tiles = {
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
                "|",
            },

            conditions = {
                {
                    position = { x = 0, y = -1 },
                    fg = { type = "AnySolidBlock" },
                },
            },

            place_chance = 0.2,
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

    max_depth = 0.2,

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

            conditions = {
                {
                    position = { x = 0, y = 1 },
                    fg = { type = "AnySolidBlock" }
                }
            },
        },

        upwards_cactus = {
            origin = {
                x = 2,
                y = 0,
            },

            tile_aliases = {
                ['c'] = { block = "default:cactus" },
            },

            tiles = {
                "  c  ",
                "  c  ",
                "ccccc",
                "c c c",
                "  c  ",
            },

            place_chance = 0.01,

            conditions = {
                {
                    position = { x = 0, y = -1 },
                    fg = { type = "AnySolidBlock" }
                }
            },
        }
    }
})

terrarium.register_biome("default:underground_desert", {
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

    min_depth = 0.2,

    decorations = {
        cactus = {
            origin = {
                x = 2,
                y = 4,
            },

            tile_aliases = {
                ['c'] = { block = "default:glow_cactus" },
            },

            tiles = {
                "  c  ",
                "c c c",
                "ccccc",
                "  c  ",
                "  c  ",
            },

            place_chance = 0.02,

            conditions = {
                {
                    position = { x = 0, y = 1 },
                    fg = { type = "AnySolidBlock" }
                }
            },
        },

        upwards_cactus = {
            origin = {
                x = 2,
                y = 0,
            },

            tile_aliases = {
                ['c'] = { block = "default:glow_cactus" },
            },

            tiles = {
                "  c  ",
                "  c  ",
                "ccccc",
                "c c c",
                "  c  ",
            },

            place_chance = 0.02,

            conditions = {
                {
                    position = { x = 0, y = -1 },
                    fg = { type = "AnySolidBlock" }
                }
            },
        }
    }
})

terrarium.register_biome("default:frozen_surface", {
    heat_max = 0.4,

    top = {
        block = "default:snow",
        wall = "default:snow",
    },

    filler = {
        block = "default:snow",
        wall = "default:snow",
    },

    filler_depth = 4,

    stone = {
        block = "default:ice",
        wall = "default:ice",
    },

    max_depth = 0.2,

    priority = 2,
})

terrarium.register_biome("default:frozen_caves", {
    heat_max = 0.4,

    top = {
        block = "default:ice",
        wall = "default:ice",
    },

    filler = {
        block = "default:ice",
        wall = "default:ice",
    },

    filler_depth = 1,

    stone = {
        block = "default:ice",
        wall = "default:ice",
    },

    min_depth = 0.2,
    max_depth = 0.6,

    priority = 2,
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

local crystals = {
    "cyan",
    "green",
    "purple",
    "red",
    "white",
    "yellow",
}

local crystal_decorations = {}

for _, name in ipairs(crystals) do
    crystal_decorations[name.."_crystal"] = terrarium.single_tile_decor {
        tile = { block = "default:"..name.."_crystal" },
        place_chance = 0.01,
        conditions = {
            {
                position = { x = 0, y = 1 },
                fg = { type = "AnySolidBlock" }
            }
        },
    }
end

terrarium.register_biome("default:crystal_caves", {
    humidity_min = 0.2,
    heat_min = 0.6,

    top = {
        block = "default:stone",
        wall = "default:stone",
    },

    top_depth = 1,

    filler = {
        block = "default:stone",
        wall = "default:stone",
    },

    filler_depth = 1,

    stone = {
        block = "default:stone",
        wall = "default:stone",
    },

    priority = 2,

    min_depth = 0.6,

    decorations = crystal_decorations,
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

terrarium.register_ore({
    cluster_tiles = 14,
    distribution = 64,

    min_depth = 0.4,

    tile = {
        block = "default:silver_ore",
    },
})

-- Test dugeons
lootlib.register_loot("default:test_dungeon_loot_unique", {
    item = "default:revolver",
    count = 1,
    chance = 1,
    unique = true, -- Don't add multiple revolvers in one chest

    comes_with_bullets = true, -- Add some ammo so player can pew pew
})

lootlib.register_loot("default:test_dungeon_loot", {
    item = "default:gold_ingot",
    count = { min = 1, max = 4, },
    chance = 0.1,
})

lootlib.register_loot("default:test_dungeon_loot", {
    item = "default:torch",
    count = { min = 10, max = 15, },
    chance = 0.2,
})

terrarium.register_on_map_generated(function()
    local WORLD_SIZE = terrarium.get_world_size()

    local DUNGEONS = 20

    local SIZE = vec2.new(10, 10)

    local PICS = {
        "default:picture_sungarden",
        "default:picture_starbound",
    }

    for i = 1, DUNGEONS do
        local root_x = math.floor(WORLD_SIZE.x * i/(DUNGEONS+1))
        local root_y = math.floor(WORLD_SIZE.y / 4 + math.random()*(WORLD_SIZE.y / 4))

        --print("Generated dungeon at "..root_x.." "..root_y)

        local bricks = function() return (math.random() < 0.2) and "default:old_broken_bricks" or "default:old_bricks" end

        for offx = 0, SIZE.x-1 do
            for offy = 0, SIZE.y-1 do
                if offx == 0 or offx == SIZE.x-1 or offy == 0 or offy == SIZE.y-1 then
                    terrarium.set_block(root_x + offx, root_y + offy, bricks())
                elseif (offx == 1 or offx == SIZE.x-2) and offy == math.floor(SIZE.y/2) then
                    terrarium.set_block(root_x + offx, root_y + offy, "default:torch")
                else
                    terrarium.set_block(root_x + offx, root_y + offy, nil)
                end

                terrarium.set_wall(root_x + offx, root_y + offy, bricks())
            end
        end

        -- Place a chest
        local offx = math.floor(SIZE.x / 2) - 1
        local offy = SIZE.y - 3

        terrarium.set_multiblock(root_x + offx, root_y + offy, "default:chest")

        local inv = terrarium.get_block_inventory(root_x + offx, root_y + offy)
        local unique_table = {}
        local add_unique = math.random() < 0.5 -- Add 1 unique item

        for i = 1, 8 do
            local loot_group = "default:test_dungeon_loot"

            if add_unique then
                loot_group = "default:test_dungeon_loot_unique"
                add_unique = false
            end

            local item, lootdef = lootlib.add_loot(inv, loot_group, unique_table)

            if lootdef and lootdef.comes_with_bullets then
                inv:add_item(ItemStack("default:pistol_bullet", math.random(20, 30)))
            end
        end

        -- Place random painting
        offy = offy - 3

        terrarium.set_multiblock(root_x + offx, root_y + offy, PICS[math.random(1, #PICS)])
    end
end)
