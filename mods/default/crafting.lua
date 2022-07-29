terrarium.add_crafting_category("empty_hands")
terrarium.set_default_crafting_category("empty_hands")

terrarium.register_recipe("empty_hands", {
    requirements = {
        "default:dirt 10", "default:grass 10",
    },

    result = "default:grass 20",
})
