local function register_sword(metal, damage, description)
    default.register_swing("default:"..metal.."_sword_swing", {
        image = metal.."_sword_swing.png",

        size = { width = 6, height = 3, },

        animations = {
            swing = {
                time_per_frame = 0.05,
                next = "invisible",

                frames = {
                    { rect = { x = 0, y = 0*24, width = 48, height = 24, }, },
                    { rect = { x = 0, y = 1*24, width = 48, height = 24, }, },
                    { rect = { x = 0, y = 2*24, width = 48, height = 24, }, },
                    { rect = { x = 0, y = 3*24, width = 48, height = 24, }, },
                },
            },

            invisible = {
                time_per_frame = 999,

                frames = {
                    { rect = { x = 0, y = 4*24, width = 48, height = 24, }, },
                },
            },
        },

        lifetime = 0.2,
    })

    terrarium.register_item("default:"..metal.."_sword", {
        description = description,
        inventory_image = metal.."_sword.png",
        max_count = 1,

        on_start_use = function(player, itemstack, position)
            local player_pos = player.ref:get_position()
            local swing_pos
            local mirror

            -- Hardcoded player and swing width :(
            if player_pos.x + 0.75 < position.x then
                swing_pos = vec2.new(1.2, -1)
                mirror = false
            else
                swing_pos = vec2.new(-5.7, -1)
                mirror = true
            end

            local swing = default.spawn_swing(swing_pos, {
                name = "default:"..metal.."_sword_swing",
                damage = { amount = 4, invincible_time = 0.2, },
                source = player,
            })

            swing.ref:set_mirror(mirror)
            swing.ref:attach_to(player.ref)
            swing.ref:set_local_position(swing_pos)
        end,
    })
end

register_sword("copper", 2, "Copper sword\n2 melee damage")
register_sword("iron", 4, "Iron sword\n4 melee damage")
register_sword("gold", 6, "Gold sword\n6 melee damage")
