local function register_flying_slime(name, image, projectile)
    terrarium.register_entity(name, {
        physics = {
            gravity = 0,
            enable_collision = true,
        },

        image = image,

        size = {
            x = 22/16,
            y = 37/16,
        },

        animations = {
            idle = {
                time_per_frame = 0.1,

                frames = {
                    { rect = { x = 0*22, y = 0*37, width = 22, height = 37, } },
                    { rect = { x = 1*22, y = 0*37, width = 22, height = 37, } },
                    { rect = { x = 2*22, y = 0*37, width = 22, height = 37, } },
                    { rect = { x = 3*22, y = 0*37, width = 22, height = 37, } },
                    { rect = { x = 4*22, y = 0*37, width = 22, height = 37, } },
                    { rect = { x = 5*22, y = 0*37, width = 22, height = 37, } },
                    { rect = { x = 6*22, y = 0*37, width = 22, height = 37, } },
                },
            },

            attack = {
                time_per_frame = 0.1,

                next = "idle",

                frames = {
                    { rect = { x = 0*22, y = 1*37, width = 22, height = 37, } },
                    { rect = { x = 1*22, y = 1*37, width = 22, height = 37, } },
                    { rect = { x = 2*22, y = 1*37, width = 22, height = 37, } },
                    { rect = { x = 3*22, y = 1*37, width = 22, height = 37, } },
                    { rect = { x = 4*22, y = 1*37, width = 22, height = 37, } },
                    { rect = { x = 5*22, y = 1*37, width = 22, height = 37, } },
                    { rect = { x = 6*22, y = 1*37, width = 22, height = 37, } },
                },
            },
        },

        equipment = {
            ["slime"] = {
                protection = 0,

                elemental_resistance = {
                    ["normal"] = 0.5,
                    ["fire"] = 0.75,
                    ["ice"] = 1.5,
                },
            },
        },

        on_create = function(self)
            self.ref:set_animation("idle")

            self.team = 1

            self.hp = 30

            self.anim = timer.new(2.0, true)
        end,

        update = function(self, dtime)
            if self.anim:tick(dtime) then
                self.ref:set_animation("attack")

                terrarium.new_entity(
                    projectile,
                    self.ref:get_center(),
                    {
                        damage = { amount = 6, },
                        speed = vec2.new(0, 10),
                        source = self,
                    })
            end
        end,
    })
end

register_flying_slime("slime:flying_slimeblu", "flying_slimeblu.png", "slime:slime_ball")
register_flying_slime("slime:flying_slimecin", "flying_slimecin.png", "slime:slime_ball")
register_flying_slime("slime:flying_slimegra", "flying_slimegra.png", "slime:slime_ball")
register_flying_slime("slime:flying_slimepur", "flying_slimepur.png", "slime:slime_ball")
register_flying_slime("slime:flying_slimera", "flying_slimera.png", "slime:slime_ball")
register_flying_slime("slime:flying_slimeyel", "flying_slimeyel.png", "slime:slime_ball")
