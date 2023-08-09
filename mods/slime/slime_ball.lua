terrarium.register_entity("slime:slime_ball", {
    physics = {
        gravity = 40,
        enable_collision = true,
    },

    image = "slime_ball.png",

    size = {
        width = 0.25,
        height = 0.25,
    },

    on_create = function(self, data)
        self.damage = data.damage or { amount = 1 }
        self.source = data.source

        self.timer = timer.new(data.fuse or 10)

        if self.source ~= nil then
            self.team = self.source.team
        end

        if data.speed ~= nil then
            self.ref:set_speed(data.speed)
            self.ref:set_rotation(data.speed:direction())
        end
    end,

    update = function(self, dtime)
        if self.timer:tick(dtime) then
            return self:kill()
        end

        local position = self.ref:get_position()
        local collision_info = self.ref:get_collision_info()

        if collision_info.blockl or collision_info.blockr or collision_info.blocku or collision_info.blockd then
            return self:kill()
        end

        for enemy in terrarium.iter_entities_with { "hurt", "team" } do
            if enemy.team ~= self.team and enemy.ref:is_collide(self.ref) then
                damagelib.hurt(enemy, self.damage, self.source, self)
                return self:kill()
            end
        end
    end,
})
