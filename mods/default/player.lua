terrarium.override_player({
    size = {
        width = 1.5,
        height = 3
    },

    image = "player.png",
})


terrarium.register_on_player_join(function(player)
    -- TODO - add easier way to add items to player inventory
    player:get_player_inventory():get(0):set("default:copper_pickaxe", 30)
end)
