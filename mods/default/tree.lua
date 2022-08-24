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

    on_neighbour_destroy = function(position, where, block_name, user)
        if where == "down" then
            if user ~= nil then
                terrarium.give_block_drops("default:tree_base", user)
            end

            terrarium.dig_block(position.x, position.y, user, true)
        end
    end
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

    on_neighbour_destroy = function(position, where, block_name, user)
        if where == "down" then
            if user ~= nil then
                terrarium.give_block_drops("default:tree_trunk", user)
            end

            terrarium.dig_block(position.x, position.y, user, true)
        end
    end
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

    on_neighbour_destroy = function(position, where, block_name, user)
        if where == "down" then
            if user ~= nil then
                terrarium.give_block_drops("default:tree_trunk_branch_left", user)
            end

            terrarium.dig_block(position.x, position.y, user, true)
        end
    end
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

    on_neighbour_destroy = function(position, where, block_name, user)
        if where == "down" then
            if user ~= nil then
                terrarium.give_block_drops("default:tree_trunk_branch_right", user)
            end

            terrarium.dig_block(position.x, position.y, user, true)
        end
    end
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

    on_neighbour_destroy = function(position, where, block_name, user)
        if where == "down" then
            if user ~= nil then
                terrarium.give_block_drops("default:tree_trunk_top", user)
            end

            terrarium.dig_block(position.x, position.y, user, true)
        end
    end
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

    on_neighbour_destroy = function(position, where, block_name, user)
        if where == "right" then
            if user ~= nil then
                terrarium.give_block_drops("default:tree_branch_left", user)
            end

            terrarium.dig_block(position.x, position.y, user, true)
        end
    end
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

    on_neighbour_destroy = function(position, where, block_name, user)
        if where == "left" then
            if user ~= nil then
                terrarium.give_block_drops("default:tree_branch_right", user)
            end

            terrarium.dig_block(position.x, position.y, user, true)
        end
    end
})
