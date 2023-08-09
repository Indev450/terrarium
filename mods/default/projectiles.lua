function default.register_projectile(name, def)
    terrarium.register_entity(name, {
        image = def.image,

        physics = def.physics,

        size = def.size,

        animations = def.animations,

        on_create = function(self, data)
            self.damage = data.damage or { amount = 1 }
            self.source = data.source

            self.timer = timer.new(data.fuse or 10)

            if self.source ~= nil then
                self.target_team = self.source.team
            end

            if data.speed ~= nil then
                self.ref:set_speed(data.speed)
                self.ref:set_rotation(data.speed:direction())
            end

            if def.light then self.ref:set_light(def.light) end
        end,

        update = function(self, dtime)
            if self.timer:tick(dtime) then
                return self:kill()
            end

            local position = self.ref:get_position()
            local collision_info = self.ref:get_collision_info()

            if collision_info.blockl or collision_info.blockr or collision_info.blocku or collision_info.blockd then
                if def.hit_ground_sound ~= nil then
                    terrarium.play_sound(def.hit_ground_sound)
                end
                return self:kill()
            end

            for enemy in terrarium.iter_entities_with { "hurt", "team" } do
                if enemy.team ~= self.target_team and enemy.ref:is_collide(self.ref) then
                    damagelib.hurt(enemy, self.damage, self.source, self)
                    if def.hit_entity_sound ~= nil then
                        terrarium.play_sound(def.hit_entity_sound)
                    end
                    return self:kill()
                end
            end
        end,
    })
end

default.register_projectile("default:small_bullet", {
    image = "small_bullet.png",

    size = {
        width = 9/16,
        height = 1/16,
    },

    light = color.new(120, 30, 20),
})

default.register_projectile("default:small_energy_bullet", {
    image = "small_energy_bullet.png",

    physics = {
        gravity = 0,
        enable_collision = true,
    },

    size = {
        width = 9/16,
        height = 1/16,
    },

    light = color.new(20, 30, 120),
})
