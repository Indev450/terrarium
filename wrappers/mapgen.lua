terrarium.registered_biomes = {}

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

    priority = 0,
}


function terrarium.register_biome(name, def)
    apply_defaults(def, biome_defaults)

    terrarium.registered_biomes[name] = def
end

-- Converts block names into ids
local function convert_registered(def)
    local biome = deep_copy(def)

    biome.top.block = terrarium.get_block_id(biome.top.block)
    biome.top.wall = terrarium.get_block_id(biome.top.wall)

    biome.filler.block = terrarium.get_block_id(biome.filler.block)
    biome.filler.wall = terrarium.get_block_id(biome.filler.wall)

    biome.stone.block = terrarium.get_block_id(biome.stone.block)
    biome.stone.wall = terrarium.get_block_id(biome.stone.wall)

    return biome
end

function core._get_mapgen_data()
    local data = {}

    data.filler = {
        block = terrarium.get_block_id("mapgen:filler_block"),
        wall = terrarium.get_block_id("mapgen:filler_wall"),
    }

    data.biomes = {}

    for _, biome_def in pairs(terrarium.registered_biomes) do
        data.biomes[#data.biomes+1] = convert_registered(biome_def)
    end

    return data
end
