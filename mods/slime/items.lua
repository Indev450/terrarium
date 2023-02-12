terrarium.register_item("slime:slime_heart", {
    description = [[Slime Heart
Contains some kind of power that makes slimes alive
Equipable in health slot
+20 HP, +2 Armor]],

    inventory_image = "slime_heart.png",

    equip_slot = "health",

    protection = 2,

    on_start_use = function(player, itemstack)
        player:equip("health", itemstack)
    end,

    on_equip = function(player)
        player:set_max_hp(player.max_hp + 20)
    end,

    on_unequip = function(player)
        player:set_max_hp(player.max_hp - 20)
    end,
})
