terrarium.add_crafting_category("empty_hands")
terrarium.set_default_crafting_category("empty_hands")

terrarium.register_recipe("empty_hands", {
    requirements = {
        "default:wood 10",
    },

    result = "default:workbench",
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

