default.register_effect("slime:slime_ointment", {
    start = function(self, user)
        self.heal_timer = timer.new(0.5, true)
    end,

    update = function(self, user, dtime)
        if self.heal_timer:tick(dtime) then
            user:heal(1)
        end
    end,
})
