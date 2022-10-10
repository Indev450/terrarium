terrarium.register_item("default:sapling", {
    description = "Sapling",
    inventory_image = "sapling.png",

    max_count = 999,
})

terrarium.register_block("default:tree_base", {
    image = "tree_base.png",

    is_solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk", {
    image = "tree_trunk.png",

    is_solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk_branch_left", {
    image = "tree_trunk_branch_left.png",

    is_solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk_branch_right", {
    image = "tree_trunk_branch_right.png",

    is_solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_trunk_top", {
    image = "tree_trunk_top.png",

    is_solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:wood 2",

    on_neighbour_destroy = terrarium.connected_to_blocks({"down"}),
})

terrarium.register_block("default:tree_branch_left", {
    image = "tree_branch_left.png",

    is_solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:sapling",

    on_neighbour_destroy = terrarium.connected_to_blocks({"right"}),
})

terrarium.register_block("default:tree_branch_right", {
    image = "tree_branch_right.png",

    is_solid = false,

    groups = {
        ["tree"] = 1,
    },

    dig_sound = {
        name = "dig_wood.wav",
        volume = 1.0,
        pitch = 1.0,
    },

    drop = "default:sapling",

    on_neighbour_destroy = terrarium.connected_to_blocks({"left"}),
})
