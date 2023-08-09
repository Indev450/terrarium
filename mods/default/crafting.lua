terrarium.add_crafting_category("empty_hands")
terrarium.set_default_crafting_category("empty_hands")

terrarium.register_recipe("empty_hands", {
    requirements = {
        "default:wood 10",
    },

    result = "default:workbench",
})

terrarium.register_recipe("empty_hands", {
    requirements = {
        "default:wood 2",
    },

    result = "default:torch 4",
})

terrarium.register_recipe("empty_hands", {
    requirements = {
        "default:wood",
    },

    result = "default:wood_platform 4",
})

terrarium.add_crafting_category("workbench")

terrarium.register_recipe("workbench", {
    requirements = {
        "default:stone 20", "default:wood 10",
    },

    result = "default:furnace",
})

terrarium.register_recipe("workbench", {
    requirements = {
        "default:iron_ingot 10",
    },

    result = "default:anvil",
})

terrarium.add_crafting_category("furnace")

terrarium.register_recipe("furnace", {
    requirements = {
        "default:copper_ore 2",
    },

    result = "default:copper_ingot",
})

terrarium.register_recipe("furnace", {
    requirements = {
        "default:iron_ore 2",
    },

    result = "default:iron_ingot",
})

terrarium.register_recipe("furnace", {
    requirements = {
        "default:silver_ore 2",
    },

    result = "default:silver_ingot",
})

terrarium.register_recipe("furnace", {
    requirements = {
        "default:gold_ore 2",
    },

    result = "default:gold_ingot",
})

terrarium.register_recipe("furnace", {
    requirements = {
        "default:sand 4",
    },

    result = "default:glass",
})

terrarium.register_recipe("furnace", {
    requirements = {
        "default:stone 4",
    },

    result = "default:stone_bricks 4",
})

terrarium.add_crafting_category("anvil")

-- Copper gear
terrarium.register_recipe("anvil", {
    requirements = {
        "default:copper_ingot 16", "default:wood 8",
    },

    result = "default:copper_pickaxe",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:copper_ingot 12", "default:wood 8",
    },

    result = "default:copper_axe",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:copper_ingot 10", "default:wood 4",
    },

    result = "default:copper_sword",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:copper_ingot 12",
    },

    result = "default:copper_helmet",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:copper_ingot 16",
    },

    result = "default:copper_breathplate",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:copper_ingot 12",
    },

    result = "default:copper_leggins",
})

-- Iron gear
terrarium.register_recipe("anvil", {
    requirements = {
        "default:iron_ingot 16", "default:wood 8",
    },

    result = "default:iron_pickaxe",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:iron_ingot 12", "default:wood 8",
    },

    result = "default:iron_axe",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:iron_ingot 10", "default:wood 4",
    },

    result = "default:iron_sword",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:iron_ingot 12",
    },

    result = "default:iron_helmet",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:iron_ingot 16",
    },

    result = "default:iron_breathplate",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:iron_ingot 12",
    },

    result = "default:iron_leggins",
})

-- Gold gear
terrarium.register_recipe("anvil", {
    requirements = {
        "default:gold_ingot 16", "default:wood 8",
    },

    result = "default:gold_pickaxe",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:gold_ingot 12", "default:wood 8",
    },

    result = "default:gold_axe",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:gold_ingot 10", "default:wood 4",
    },

    result = "default:gold_sword",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:gold_ingot 12",
    },

    result = "default:gold_helmet",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:gold_ingot 16",
    },

    result = "default:gold_breathplate",
})

terrarium.register_recipe("anvil", {
    requirements = {
        "default:gold_ingot 12",
    },

    result = "default:gold_leggins",
})
