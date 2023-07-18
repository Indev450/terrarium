terrarium.registered_biomes = {}
terrarium.registered_ores = {}
terrarium.on_map_generated = {}

local biome_defaults = {
    humidity_min = -1.0,
    humidity_max = 1.0,

    heat_min = -1.0,
    heat_max = 1.0,

    top = {
        block = "__builtin:air",
        wall = "__builtin:air",
    },

    top_depth = 1,

    filler = {
        block = "__builtin:air",
        wall = "__builtin:air",
    },

    filler_depth = 1,

    stone = {
        block = "__builtin:air",
        wall = "__builtin:air",
    },

    min_depth = -1.0,

    max_depth = 1.0,

    priority = 0,

    decorations = {},
}

local ore_defaults = {
    cluster_tiles = 12,
    distribution = 48,

    min_depth = 0.0,
    max_depth = 1.0,

    tile = {
        block = "__builtin:air",
        wall = "__builtin:air",
    }
}

local decor_defaults = {
    origin = { x = 0, y = 0, },
    place_chance = 0.5,
    conditions = {},
}


function terrarium.register_biome(name, def)
    apply_defaults(def, biome_defaults)

    terrarium.registered_biomes[name] = def
end

function terrarium.register_ore(def)
    apply_defaults(def, ore_defaults)

    terrarium.registered_ores[#terrarium.registered_ores + 1] = def
end

function terrarium.single_tile_decor(def)
    return {
        origin = { x = 0, y = 0 },
        tile_aliases = { ['b'] = def.tile },
        tiles = {"b"},
        place_chance = def.place_chance,
        conditions = def.conditions,
    }
end

function terrarium.register_on_map_generated(func)
    table.insert(terrarium.on_map_generated, func)
end

function terrarium.get_world_size()
    return core._get_world_size()
end

-- Converts block names into ids
local function parse_tile(tile)
    return {
        block = terrarium.get_block_id(tile.block),
        wall = terrarium.get_block_id(tile.wall),
        multiblock_offset = tile.multiblock_offset,
    }
end

local function parse_condition(cond)
    if cond == nil then return end

    local result = {
        type = cond.type,
    }

    if cond.type == "AnyOfBlocks" or cond.type == "NoneOfBlocks" then
        result.blocks = {}

        for i, block_name in ipairs(cond.blocks) do
            result.blocks[i] = terrarium.get_block_id(block_name)
        end
    elseif cond.type == "ExactBlock" then
        result.block = terrarium.get_block_id(cond.block)
    end

    return result
end

-- Tile condition is simply constructed from 2 block conditions
local function parse_tile_condition(tile_cond)
    local result = {
        position = tile_cond.position or { x = 0, y = 0 },
    }

    result.fg = parse_condition(tile_cond.fg)
    result.bg = parse_condition(tile_cond.bg)

    return result
end

local function parse_decoration(def)
    apply_defaults(def, decor_defaults)

    local result = {
        origin = def.origin,

        width = 0,
        height = #def.tiles,

        place_chance = def.place_chance,

        tiles = {},

        conditions = {},
    }

    -- First, create 2d array of tiles
    local tiles_tmp = {}

    for i = 1, #def.tiles do
        local width = 0
        tiles_tmp[i] = {}

        -- Iterate over every character in string
        for c in def.tiles[i]:gmatch('.') do
            local tile = def.tile_aliases[c] or { block = "", wall = "" }
            tiles_tmp[i][#tiles_tmp[i]+1] = parse_tile(tile)
            width = width + 1
        end

        if width > result.width then
            result.width = width
        end
    end

    for y = 1, result.height do
        for x = 1, result.width do
            result.tiles[#result.tiles+1] = tiles_tmp[y][x] or { block = 0, wall = 0 }
        end
    end

    for i = 1, #def.conditions do
        result.conditions[i] = parse_tile_condition(def.conditions[i])
    end

    return result
end

local function convert_registered_biome(def)
    local biome = deep_copy(def)

    biome.top = parse_tile(biome.top)

    biome.filler = parse_tile(biome.filler)

    biome.stone = parse_tile(biome.stone)

    for key, decor in pairs(def.decorations) do
        biome.decorations[key] = parse_decoration(decor)
    end

    return biome
end

local function convert_registered_ore(def)
    local ore = deep_copy(def)

    ore.tile = parse_tile(ore.tile)

    return ore
end

function core._get_mapgen_data()
    local data = {}

    data.filler = {
        block = terrarium.get_block_id("mapgen:filler_block"),
        wall = terrarium.get_block_id("mapgen:filler_wall"),
    }

    data.biomes = {}

    for _, biome_def in pairs(terrarium.registered_biomes) do
        data.biomes[#data.biomes+1] = convert_registered_biome(biome_def)
    end

    data.ores = {}

    for _, ore_def in pairs(terrarium.registered_ores) do
        data.ores[#data.ores+1] = convert_registered_ore(ore_def)
    end

    return data
end

function core._on_mapgen_finish()
    for _, func in ipairs(terrarium.on_map_generated) do
        func()
    end
end
