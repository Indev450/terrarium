function default.register_equipment(name, def)
    def.equip_slot = def.equip_slot or "protection"

    -- Thats weird to use more than one equipment item but ok
    def.max_count = def.max_count or 1

    local do_equip = function(player, itemstack)
        player:equip(def.equip_slot, itemstack)
    end

    def.on_start_use = do_equip
    def.on_start_alt_use = do_equip

    terrarium.register_item(name, def)
end

-- Copper gear
default.register_equipment("default:copper_helmet", {
    description = "Copper helmet\nArmor +1",
    inventory_image = "copper_helmet.png",

    protection = 1,

    equip_slot = "helmet",
})

default.register_equipment("default:copper_breathplate", {
    description = "Copper breathplate\nArmor +2",
    inventory_image = "copper_breathplate.png",

    protection = 2,

    equip_slot = "breathplate",
})

default.register_equipment("default:copper_leggins", {
    description = "Copper leggins\nArmor +1",
    inventory_image = "copper_leggins.png",

    protection = 1,

    equip_slot = "leggins",
})

-- Iron gear
default.register_equipment("default:iron_helmet", {
    description = "Iron helmet\nArmor +2",
    inventory_image = "iron_helmet.png",

    protection = 2,

    equip_slot = "helmet",
})

default.register_equipment("default:iron_breathplate", {
    description = "Iron breathplate\nArmor +3",
    inventory_image = "iron_breathplate.png",

    protection = 3,

    equip_slot = "breathplate",
})

default.register_equipment("default:iron_leggins", {
    description = "Iron leggins\nArmor +2",
    inventory_image = "iron_leggins.png",

    protection = 2,

    equip_slot = "leggins",
})

-- Gold gear
default.register_equipment("default:gold_helmet", {
    description = "Gold helmet\nArmor +3",
    inventory_image = "gold_helmet.png",

    protection = 3,

    equip_slot = "helmet",
})

default.register_equipment("default:gold_breathplate", {
    description = "Gold breathplate\nArmor +4",
    inventory_image = "gold_breathplate.png",

    protection = 4,

    equip_slot = "breathplate",
})

default.register_equipment("default:gold_leggins", {
    description = "Gold leggins\nArmor +3",
    inventory_image = "gold_leggins.png",

    protection = 3,

    equip_slot = "leggins",
})
