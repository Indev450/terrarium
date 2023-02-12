function default.register_swing(name, def)
    terrarium.register_entity(name, {
        physics = {
            gravity = 0,
            enable_collision = false,
        },

        image = def.image,

        size = def.size,

        animations = def.animations,

        on_create = function(self, data)
            self.source = data.source
            self.source_team = data.source_team
            self.damage = data.damage or { amount = 1 }
            self.on_land_hit = data.on_land_hit or function() end

            self.timer = timer.new(def.lifetime)

            self.ref:set_animation("swing")
        end,

        update = function(self, dtime)
            if self.timer:tick(dtime) then
                self:kill()
                return
            end

            for enemy in terrarium.iter_entities_with { "team", "hurt" } do
                if (self.source_team == nil or enemy.team ~= self.source_team) and enemy.ref:is_collide(self.ref) then
                    local damage = damagelib.hurt(enemy, self.damage, self.source, self)
                    self.on_land_hit(enemy, damage)
                end
            end
        end,
    })
end

function default.spawn_swing(position, swing_data)
    swing_data = swing_data or {}

    if not swing_data.source_team and not swing_data.no_team and swing_data.source then
        swing_data.source_team = swing_data.source.team
    end

    if swing_data.sound then
        terrarium.play_sound({
            name = swing_data.sound.name,
            volume = swing_data.sound.volume,
            pitch = swing_data.sound.pitch,
            source = position,
        })
    end

    local swing = terrarium.new_entity(swing_data.name, position, swing_data)

    return swing
end
