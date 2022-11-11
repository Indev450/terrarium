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

    player.team = 0

    player.max_hp = 100
    player.hp = player.max_hp

    -- Invincibility timer. 0.5 seconds of invincibility when spawn
    player.inv_timer = timer.new(0.5)

    player:add_bar("hp", {
        geometry = {
            x = -210,
            y = 10,
            width = 200,
            height = 40,
        },

        text_style = "Division",
        text = "HP: ",
        text_size = 24,
        text_color = color.new(255, 255, 255),

        background_color = color.new(0, 0, 0),
        bar_color = color.new(212, 24, 12),
        max_value = player.max_hp,
    })

    player.hurt = function(self, damage, source, knockback, inv_time)
        if not self.inv_timer:ready() then return end

        knockback = knockback or 2

        local kb_vec = {
            x = 0,
            y = -knockback,
        }

        if source ~= nil then
            kb_vec.x = knockback

            local our_pos = self.ref:get_position()
            local their_pos = source.ref:get_position()

            if our_pos.x < their_pos.x then kb_vec.x = kb_vec.x * -1 end
        end

        self.ref:set_speed(kb_vec)

        self.hp = self.hp - damage

        self.inv_timer.time_to_wait = inv_time or 0.5
        self.inv_timer:restart()

        if self.hp < 0 then self.hp = 0 end
    end
end)

terrarium.register_on_player_update(function(player, dtime)
    player:set_bar_value("hp", player.hp)
    player.inv_timer:tick(dtime)

    if player.hp == 0 then
        player.hp = player.max_hp
        player.ref:set_position({x = 0, y = 0})
    end
end)
