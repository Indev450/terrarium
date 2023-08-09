-- Entity for damage indicator
terrarium.register_entity("damagelib:damage_indicator", {
    physics = { gravity = 0, enable_collision = false, },

    image = "null.png",

    on_create = function(self, data)
        data = data or {}

        self.timer = timer.new(data.lifetime or 0.5)
        self.anim = 1

        self.ref:set_text(string.format("%.1f", data.amount or 0), data.color or color.new(230, 0, 0))
    end,

    update = function(self, dtime)
        self.ref:set_speed({ x = 0, y = -self.anim })
        self.anim = math.max(0, self.anim - dtime)

        if self.timer:tick(dtime) then
            self:kill()
        end
    end,
})

-- Default implementation for "entity hurt" protocol
-- damage: number
-- source: entity/nil
-- knockback: number/vec2
-- inv_time: number
--
-- Returns: is damage was actually dealt
--
-- Entity SHOULD have fields:
-- self.hp: number
--
-- Entity CAN have fields:
-- self.inv_timer: timer
-- self.no_knockback: boolean, default = false
-- self.no_kill: boolean, default = false
-- self.default_knockback: number, default = 0.0
-- self.default_inv_time: number, default = 0.0
-- self.knockback_power: number, default = 1.0 (effectively)
-- self.on_death: function(self, damage, source, knockback)
-- self.on_hurt: function(self, damage, source, knockback)
function damagelib.entity_hurt(self, damage, source, knockback, inv_time)
    if self.hp <= 0 then return false end -- We are dead already

    -- If entity has invincibility timer, use it and exit if we have i-frames.
    -- This timer should be updated in entity update function.
    if self.inv_timer then
        if not self.inv_timer:ready() then return false end

        self.inv_timer.time_to_wait = inv_time or self.default_inv_time or 0
        self.inv_timer:restart()
    end

    -- Allow entities to be knockback-immune
    if not self.no_knockback then
        knockback = knockback or self.default_knockback or 0

        -- Optional multiplier to make knockback stronger/weaker
        if self.knockback_power then
            knockback = knockback * self.knockback_power
        end

        local kb_vec = knockback

        -- If it is number, calculate knockback based on position relative to
        -- damage source
        if type(knockback) == "number" then
            kb_vec = { x = 0, y = -knockback }

            if source then
                local our_pos = self.ref:get_center()
                local their_pos = source.ref:get_center()

                kb_vec = (our_pos - their_pos):resize(knockback)
            end
        end

        self.ref:set_speed(kb_vec)
    end

    -- Do actual damage
    self.hp = self.hp - damage

    if self.hp <= 0 then
        -- If there is callback for death, call it
        if self.on_death then
            self:on_death(damage, source, knockback)
        end

        -- Remove entity from world
        if not self.no_kill then
            self:kill()
        end
    else
        -- If there is callback for hurt, call it
        if self.on_hurt then
            self:on_hurt(damage, source, knockback)
        end
    end

    return true
end
