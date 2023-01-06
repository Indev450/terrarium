terrarium.override_player({
    size = {
        width = 1.5,
        height = 3
    },

    image = "player.png",

    animations = {
        idle = {
            time_per_frame = 999,

            frames = {
                { x = 0, y = 0, width = 24, height = 48 },
            },
        },

        jump_start = {
            time_per_frame = 0.1,

            next = "jump",

            frames = {
                { x = 24*1, y = 0, width = 24, height = 48 },
                { x = 24*2, y = 0, width = 24, height = 48 },
            },
        },

        jump = {
            time_per_frame = 999,

            frames = {
                { x = 24*3, y = 0, width = 24, height = 48 },
            },
        },

        walk = {
            time_per_frame = 0.09,

            frames = {
                { x = 24*0, y = 48, width = 24, height = 48 },
                { x = 24*1, y = 48, width = 24, height = 48 },
                { x = 24*2, y = 48, width = 24, height = 48 },
                { x = 24*3, y = 48, width = 24, height = 48 },
                { x = 24*4, y = 48, width = 24, height = 48 },
                { x = 24*5, y = 48, width = 24, height = 48 },
            }
        }
    },
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

    player.ref:set_animation("idle")
end)

terrarium.register_on_player_update(function(player, dtime)
    player:set_bar_value("hp", player.hp)
    player.inv_timer:tick(dtime)

    if player.hp == 0 then
        player.hp = player.max_hp
        player.ref:set_position({x = 0, y = 0})
    end
end)

-- Update animation
terrarium.register_on_player_update(function(player, dtime)
    local speed = player.ref:get_speed()
    local collision = player.ref:get_collision_info()
    local input = player.ref:get_player_controls()

    if input.right and speed.x > 0 then
        player.ref:set_mirror(false)
    end

    if input.left and speed.x < 0 then
        player.ref:set_mirror(true)
    end

    if collision.blockd and input.jump then
        player.ref:set_animation("jump_start")
        return
    end

    if (input.left or input.right) and collision.blockd then
        player.ref:set_animation("walk")
        return
    end

    if collision.blockd then
        player.ref:set_animation("idle")
        return
    elseif player.ref:get_animation() ~= "jump_start" then
        player.ref:set_animation("jump")
    end
end)
