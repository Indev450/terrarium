local MAX_DISTANCE = 100
local MAX_DISTANCE_SQR = MAX_DISTANCE*MAX_DISTANCE
local LIGHT = color.new(120, 170, 230)

terrarium.register_entity("default:ai_holo", {
    physics = {
        gravity = 0,
        enable_collision = false,
    },

    size = {
        width = 25/16,
        height = 48/16,
    },

    image = "ai.png",

    on_create = function(self, data)
        self.user = data and data.user
        self.anim = timer.new(1)
        self.ref:set_light(LIGHT)
    end,

    update = function(self, dtime)
        if not (self.user and self.user.ref:is_valid()) or (self.user.ref:get_center() - self.ref:get_center()):length_squared() > MAX_DISTANCE_SQR then
            self:kill() -- ltg.mp4
        end

        -- A little pop up animation
        if not self.anim:tick(dtime) then
            self.ref:set_speed(vec2.new(0, -self.anim.timer))
        else
            self.ref:set_speed(vec2.new(0, 0))
        end

        -- Look at player
        local m = self.user.ref:get_center().x > self.ref:get_center().x
        self.ref:set_mirror(m)
    end,

    reset_position = function(self, position, vertflip)
        self.ref:set_position(position)
        self.anim:restart()
        self.ref:set_vertmirror(vertflip) -- Funni
    end,
})
