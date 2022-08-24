terrarium.override_player({
    size = {
        width = 1.5,
        height = 3
    },

    image = "player.png",
})


terrarium.register_on_player_join(function(player)
    -- TODO - add easier way to add items to player inventory
    player.ref:get_player_inventory():get(0):set("default:copper_pickaxe", 1)
    player.ref:get_player_inventory():get(1):set("default:copper_axe", 1)
    player.ref:get_player_inventory():get(2):set("default:chest", 5)
end)
