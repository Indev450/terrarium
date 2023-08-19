local types = { "blu", "cin", "gra", "pur", "ra", "yel", }
local colors = { "Blue", "Cyan", "Green", "Purple", "Red", "Yellow", }

for i = 1, #types do
    local name = "slime:stone_slime"..types[i]
    local description = colors[i].." slimy stone"

    terrarium.register_block(name, {
        description = description,

        draw_type = "Autotile",
        image = "stone_slime"..types[i]..".png",

        slippery = 0.9,

        solid = true,

        groups = {
            ["ground"] = 1,
        },

        inventory_image = "stone_slime"..types[i].."_item.png",
    })

    description = colors[i].." slime grass"

    name = "slime:grass_small_slime"..types[i]
    terrarium.register_block(name, {
        description = description,

        image = "grass_small_slime"..types[i]..".png",

        autotile_neighbour = false,
        solid = false,
        replacable = true,

        drop = "",

        groups = {
            ["ground"] = 1,
        },

        on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
    })

    name = "slime:grass_slime"..types[i]
    terrarium.register_block(name, {
        description = description,

        image = "grass_slime"..types[i]..".png",

        multiblock_size = { x = 1, y = 2, },

        autotile_neighbour = false,
        solid = false,
        replacable = true,

        drop = "",

        groups = {
            ["ground"] = 1,
        },

        on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
    })

    name = "slime:grass2_slime"..types[i]
    terrarium.register_block(name, {
        description = description,

        image = "grass2_slime"..types[i]..".png",

        multiblock_size = { x = 1, y = 2, },

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
