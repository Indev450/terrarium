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
    player.ref:get_player_inventory():get(1):set("default:chest", 5)

    player:add_bar("energy", {
        max_value = 20,

        bar_color = color.new(10, 255, 20),

        text_style = "Division",

        text_color = color.new(40, 10, 150),

        text = "Energy: ",

        geometry = {
            x = 600,
            y = 38,

            width = 128,
            height = 32,
        }
    })

    player:set_bar_value("energy", 15)
end)
