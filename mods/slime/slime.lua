local max_slimes = 10
local slimes = 0

local despawn_dist = 70
local despawn_time = 10

local see_dist = 20
local see_time = 6

terrarium.register_entity("slime:slime", {
    physics = {
        gravity = 40,
        enable_collision = true,
    },

    image = "slime.png",

    size = {
        width = 2.75,
        height = 1.625,
    },

    animations = {
        jump = {
            time_per_frame = 0.1,
            next = "in_air",
            frames = {
                { rect = { x = 28*0, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*1, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*2, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*3, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*4, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*5, y = 0, width = 28, height = 26, }, },
            },
        },

        in_air = {
            time_per_frame = 0.2,
            frames = {
                { rect = { x = 28*5, y = 0, width = 28, height = 26, }, },
            },
        },

        idle = {
            time_per_frame = 0.2,
            frames = {
                { rect = { x = 28*0, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*2, y = 0, width = 28, height = 26, }, },
            },
        },

        land = {
            time_per_frame = 0.1,
            next = "idle",
            frames = {
                { rect = { x = 28*3, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*2, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*1, y = 0, width = 28, height = 26, }, },
                { rect = { x = 28*0, y = 0, width = 28, height = 26, }, },
            },
        },
    },

    equipment = {
        ["slime"] = {
            protection = 2,

            elemental_resistance = {
                ["fire"] = 0.75,
                ["ice"] = 1.5,
            },
        },
    },

    on_create = function(self)
        self.ref:set_animation("idle")
        self.jump_timer = 1
        self.jump_timer_max = 1
        self.team = 1
        self.hp = 40

        self.see_timer = timer.new(see_time)
        self.target = nil

        self.despawn_timer = timer.new(despawn_time)
        self.inv_timer = timer.new(0)
    end,

    hurt = function(self, damage, source, knockback, inv_time)
        if not self.inv_timer:ready() then return end

        self.inv_timer.time_to_wait = inv_time or 0
        self.inv_timer:restart()

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

        if not self.target and source and source.source then
            self.target = source.source
            self.see_timer:restart()
        end

        self.ref:set_speed(kb_vec)

        self.hp = self.hp - damage

        if self.hp <= 0 then
            if math.random() < 0.05 then
                terrarium.drop_item(ItemStack("slime:slime_heart"), self.ref:get_position())
            end

            self:kill()
            slimes = slimes - 1
        end
    end,

    update = function(self, dtime)
        self.inv_timer:tick(dtime)

        local was_in_air = self.jump_timer == self.jump_timer_max
        local collision = self.ref:get_collision_info()
        local pos = self.ref:get_position()

        local target_pos
        local target_dist

        if self.target == nil then
            for _, player in pairs(terrarium.players) do
                local p_pos = player.ref:get_position().x
                local dist = math.abs(pos.x - p_pos)

                if not target_dist or target_dist > dist then
                    target_dist = dist
                    target_pos = p_pos

                    if target_dist < see_dist then
                        self.target = player
                        self.see_timer:restart()
                    end
                end
            end
        else
            target_pos = self.target.ref:get_position().x
            target_dist = math.abs(pos.x - target_pos)

            if target_dist > see_dist and self.see_timer:tick(dtime) then
                self.target = nil
            end
        end

        if target_dist > despawn_dist then
            if self.despawn_timer:tick(dtime) then
                self:kill()
                slimes = slimes - 1
                return
            end
        else
            self.despawn_timer:restart()
        end

        if self.target == nil then
            target_pos = pos.x + math.random(-1, 1)
        end

        if collision.blockd then
            self.jump_timer = self.jump_timer - dtime

            if self.jump_timer < 0 then
                terrarium.play_sound({
                    name = "slime_jump.wav",
                    volume = 0.1,
                    source = pos,
                })

                self.ref:set_speed({
                    x = target_pos < pos.x and -12 or 12,
                    y = -15,
                })
            elseif self.jump_timer < 0.4 then
                self.ref:set_animation("jump")
            elseif was_in_air then
                self.ref:set_animation("land")

                --[[terrarium.play_sound({
                    name = "slime_land.wav",
                    volume = 0.05,
                    source = pos,
                })]]
            end
        else
            self.jump_timer = self.jump_timer_max
        end

        for enemy in terrarium.iter_entities_with { "hurt", "team" } do
            if enemy.team == 0 and enemy.ref:is_collide(self.ref) then
                damagelib.hurt(enemy, {amount = 6}, self)
            end
        end
    end,
})

local function spawn_slime(player)
    if slimes == max_slimes then return end

    if terrarium.try_spawn_entity_at("slime:slime", player, player, nil,
            function(position)
                local start_x = math.floor(position.x) - 1
                local start_y = math.floor(position.y) - 1

                local has_ground = false

                for x = start_x, start_x+2 do
                    for y = start_y, start_y+2 do
                        local def = terrarium.registered_blocks[terrarium.get_block(x, y)]

                        if def ~= nil and def.is_solid then
                            return false
                        end
                    end

                    local def = terrarium.registered_blocks[terrarium.get_block(x, start_y+3)]

                    if def ~= nil and def.is_solid then
                        has_ground = true
                    end
                end

                return has_ground
            end, 10
        ) ~= nil then
        slimes = slimes + 1
    end
end

terrarium.register_on_step_timed(1, function()
    if slimes < max_slimes then
        for _, player in pairs(terrarium.players) do
            spawn_slime(player)
        end
    end
end)
