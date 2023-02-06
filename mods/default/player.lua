default_player_saver = {
    load = function(self, save_dir_path)
        local file = io.open(save_dir_path.."/".."default.save")

        if not file then return end

        for line in file:lines() do
            local cmd_start, cmd_end = line:find("[^%s]+")

            if cmd_start then
                local cmd = line:sub(cmd_start, cmd_end)

                if cmd == "gave_init_stuff" then
                    local name = line:sub(line:find(".+", cmd_end+2))

                    self.gave_init_stuff[name] = true
                end
            end
        end

        file:close()
    end,

    save = function(self, save_dir_path)
        local file = io.open(save_dir_path.."/".."default.save", "w")

        for name, _ in pairs(self.gave_init_stuff) do
            file:write("gave_init_stuff "..name.."\n")
        end

        file:close()
    end,

    gave_init_stuff = {},
}

terrarium.override_player({
    size = {
        width = 1.5,
        height = 3
    },

    image = "player.png",

    animations = {
        invisible = {
            time_per_frame = 999,

            frames = {
                { x = 24*4, y = 0, width = 24, height = 48 },
            },
        },

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
    -- player:get_player_name()
    local name = "singleplayer"

    if not default_player_saver.gave_init_stuff[name] then
        default_player_saver.gave_init_stuff[name] = true

        player.ref:get_player_inventory():add_item(ItemStack("default:copper_pickaxe", 1))
        player.ref:get_player_inventory():add_item(ItemStack("default:copper_axe", 1))
        player.ref:get_player_inventory():add_item(ItemStack("default:chest", 5))
    end

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
        if not self.inv_timer:ready() or self.hp <= 0 then return end

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

        if self.hp <= 0 then
            terrarium.send_cmd(self, "default:player_died")
        end
    end

    player.respawn = function(self)
        player.hp = player.max_hp
        player.ref:set_player_controlled(true)
        player.ref:set_position({x = 0, y = 0})
    end

    player.ref:set_animation("idle")
end)

terrarium.register_on_player_update(function(player, dtime)
    player:set_bar_value("hp", math.max(0, player.hp))
    player.inv_timer:tick(dtime)

    if player.hp <= 0 then
        player.ref:set_player_controlled(false)
    end
end)

terrarium.on_cmd("default:player_respawn", function(player)
    if player.hp <= 0 then player:respawn() end
end)

-- Update animation
terrarium.register_on_player_update(function(player, dtime)
    local speed = player.ref:get_speed()
    local collision = player.ref:get_collision_info()
    local input = player.ref:get_player_controls()

    if player.hp <= 0 then
        player.ref:set_animation("invisible")
        return
    end

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

terrarium.register_saver(default_player_saver)
