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

terrarium.register_item("slime:slime_blob", {
    description = "Slime Blob\nIt can absorb and store organic matter",

    inventory_image = "slime_blob.png",

    max_count = 999,
})

terrarium.register_item("slime:slime_ointment", {
    description = "Slime Ointment\nProvides regeneration for 1 minute",

    inventory_image = "slime_ointment.png",

    on_start_use = function(player, itemstack)
        player.slime_ointment_timer = timer.new(60)
        player.slime_ointment_regen_timer = timer.new(1, true)
        itemstack:add(-1)
    end,

    max_count = 8,
})

-- TODO - unified effects
terrarium.register_on_player_update(function(player, dtime)
    if not player.slime_ointment_timer then return end

    if player.slime_ointment_timer:tick(dtime) then
        player.slime_ointment_timer = nil
    end

    if player.slime_ointment_regen_timer:tick(dtime) then
        player:heal(0.5)
    end
end)
