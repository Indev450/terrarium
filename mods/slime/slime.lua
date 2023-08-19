local max_slimes = 10
local slimes = 0

local despawn_dist = 70
local despawn_time = 10

local see_dist = 20
local see_time = 6

-- Kinda weird function, will probably need to rewrite it or something.
local function register_slime(name, image, hp, damage, sprite_width, sprite_height, big)
    sprite_width = sprite_width or 36
    sprite_height = sprite_height or 30

    terrarium.register_entity(name, {
        physics = {
            gravity = 40,
            enable_collision = true,
        },

        image = image,

        size = {
            x = sprite_width/16,
            y = sprite_height/16,
        },

        hitbox = {
            x = (sprite_width/16)/8,
            y = 0,
            width = sprite_width/16 - 2*(sprite_width/16)/8,
            height = (sprite_height-2)/16
        },

        animations = {
            jump = {
                time_per_frame = 0.1 / (big and 2 or 1),
                next = "in_air",
                frames = {
                    { rect = { x = sprite_width*0, y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*1, y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*2, y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*3, y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*4, y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*5, y = 0, width = sprite_width, height = sprite_height, }, },
                    big and { rect = { x = sprite_width*6, y = 0, width = sprite_width, height = sprite_height, }, } or nil,
                    big and { rect = { x = sprite_width*7, y = 0, width = sprite_width, height = sprite_height, }, } or nil,
                    big and { rect = { x = sprite_width*8, y = 0, width = sprite_width, height = sprite_height, }, } or nil,
                    big and { rect = { x = sprite_width*9, y = 0, width = sprite_width, height = sprite_height, }, } or nil,
                },
            },

            in_air = {
                time_per_frame = 0.2,
                frames = {
                    { rect = { x = sprite_width*(big and 9 or 5), y = 0, width = sprite_width, height = sprite_height, }, },
                },
            },

            idle = {
                time_per_frame = 0.2 / (big and 2 or 1),
                frames = {
                    { rect = { x = sprite_width*0, y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*2, y = 0, width = sprite_width, height = sprite_height, }, },
                },
            },

            land = {
                time_per_frame = 0.1 / (big and 2 or 1),
                next = "idle",
                frames = {
                    { rect = { x = sprite_width*(big and 6 or 3), y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*(big and 5 or 2), y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*(big and 4 or 1), y = 0, width = sprite_width, height = sprite_height, }, },
                    { rect = { x = sprite_width*(big and 3 or 0), y = 0, width = sprite_width, height = sprite_height, }, },
                    big and { rect = { x = sprite_width*2, y = 0, width = sprite_width, height = sprite_height, }, } or nil,
                    big and { rect = { x = sprite_width*1, y = 0, width = sprite_width, height = sprite_height, }, } or nil,
                    big and { rect = { x = sprite_width*0, y = 0, width = sprite_width, height = sprite_height, }, } or nil,
                },
            },
        },

        equipment = {
            ["slime"] = {
                protection = 2,

                elemental_resistance = {
                    ["normal"] = 0.5,
                    ["fire"] = 0.75,
                    ["ice"] = 1.5,
                    ["antimagic"] = 1.25,
                },
            },
        },

        on_create = function(self)
            self.ref:set_animation("idle")
            self.jump_timer = 1
            self.jump_timer_max = 1
            self.team = 1
            self.default_knockback = 2
            self.hp = hp or 40

            self.see_timer = timer.new(see_time)
            self.target = nil

            self.despawn_timer = timer.new(despawn_time)
            self.inv_timer = timer.new(0)
        end,

        on_hurt = function(self, damage, source)
            if not self.target and source and source.source then
                self.target = source.source
                self.see_timer:restart()
            end
        end,

        on_death = function(self, damage, source)
            if math.random() < 0.05 then
                terrarium.drop_item(ItemStack("slime:slime_heart"), self.ref:get_position())
            end

            slimes = slimes - 1
        end,

        hurt = damagelib.entity_hurt,

        update = function(self, dtime)
            self.inv_timer:tick(dtime)

            local was_in_air = self.jump_timer == self.jump_timer_max
            local collision = self.ref:get_collision_info()
            local pos = self.ref:get_position()

            local target_pos
            local target_dist

            if self.target == nil then
                for _, player in pairs(terrarium.players) do
                    local p_pos = player.ref:get_position()
                    local dist = (pos - p_pos):length()

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
                target_pos = self.target.ref:get_position()
                target_dist = (pos - target_pos):length()

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
                target_pos = vec2.new_from_table(pos)
                target_pos.x = target_pos.x + math.random(-1, 1)
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
                        x = target_pos.x < pos.x and -12 or 12,
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
                    damagelib.hurt(enemy, {amount = damage or 6, knockback = 2}, self)
                end
            end
        end,
    })
end

register_slime("slime:slimeblu", "slimeblu.png")
register_slime("slime:slimecin", "slimecin.png")
register_slime("slime:slimegra", "slimegra.png")
register_slime("slime:slimepur", "slimepur.png")
register_slime("slime:slimera",  "slimera.png")
register_slime("slime:slimeyel", "slimeyel.png")

register_slime("slime:spiky_slimeblu", "spiky_slimeblu.png", 60, 12, 34, 38)
register_slime("slime:spiky_slimecin", "spiky_slimecin.png", 60, 12, 34, 38)
register_slime("slime:spiky_slimegra", "spiky_slimegra.png", 60, 12, 34, 38)
register_slime("slime:spiky_slimepur", "spiky_slimepur.png", 60, 12, 34, 38)
register_slime("slime:spiky_slimera",  "spiky_slimera.png",  60, 12, 34, 38)
register_slime("slime:spiky_slimeyel", "spiky_slimeyel.png", 60, 12, 34, 38)

register_slime("slime:big_slimeblu", "big_slimeblu.png", 120, 4, 70, 60, true)
register_slime("slime:big_slimecin", "big_slimecin.png", 120, 4, 70, 60, true)
register_slime("slime:big_slimegra", "big_slimegra.png", 120, 4, 70, 60, true)
register_slime("slime:big_slimepur", "big_slimepur.png", 120, 4, 70, 60, true)
register_slime("slime:big_slimera",  "big_slimera.png",  120, 4, 70, 60, true)
register_slime("slime:big_slimeyel", "big_slimeyel.png", 120, 4, 70, 60, true)

local function spawn_slime(player)
    if slimes == max_slimes then return end

    local types = {
        { name = "slime:slimeblu",        kind = "blu" },
        { name = "slime:slimecin",        kind = "cin" },
        { name = "slime:slimegra",        kind = "gra" },
        { name = "slime:slimepur",        kind = "pur" },
        { name = "slime:slimera",         kind = "ra" },
        { name = "slime:slimeyel",        kind = "yel" },
        { name = "slime:flying_slimeblu", kind = "blu" },
        { name = "slime:flying_slimecin", kind = "cin" },
        { name = "slime:flying_slimegra", kind = "gra" },
        { name = "slime:flying_slimepur", kind = "pur" },
        { name = "slime:flying_slimera",  kind = "ra" },
        { name = "slime:flying_slimeyel", kind = "yel" },
        { name = "slime:spiky_slimeblu",  kind = "blu" },
        { name = "slime:spiky_slimecin",  kind = "cin" },
        { name = "slime:spiky_slimegra",  kind = "gra" },
        { name = "slime:spiky_slimepur",  kind = "pur" },
        { name = "slime:spiky_slimera",   kind = "ra" },
        { name = "slime:spiky_slimeyel",  kind = "yel" },
        { name = "slime:big_slimeblu",    kind = "blu" },
        { name = "slime:big_slimecin",    kind = "cin" },
        { name = "slime:big_slimegra",    kind = "gra" },
        { name = "slime:big_slimepur",    kind = "pur" },
        { name = "slime:big_slimera",     kind = "ra" },
        { name = "slime:big_slimeyel",    kind = "yel" },
    }

    local slimetype = types[math.random(1, #types)]
    local name = slimetype.name
    local kind = slimetype.kind

    if terrarium.try_spawn_entity_at(name, player, player, nil,
            function(position)
                local start_x = math.floor(position.x) - 1
                local start_y = math.floor(position.y) - 1

                local slime_biome = false

                for x = start_x, start_x+2 do
                    for y = start_y, start_y+2 do
                        local def = terrarium.registered_blocks[terrarium.get_block(x, y)]

                        if def ~= nil and def.solid then
                            return false
                        end
                    end

                    local wall = terrarium.get_wall(start_x, start_y)

                    if wall == "slime:stone_slime"..kind then
                        slime_biome = true
                    end
                end

                return slime_biome
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
