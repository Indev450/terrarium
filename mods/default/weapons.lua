damagelib.damage_color["antimagic"] = color.new(80, 90, 240)

local function register_sword(metal, damage, description)
    default.register_swing("default:"..metal.."_sword_swing", {
        image = metal.."_sword_swing.png",

        size = { width = 0.6, height = 3.3, },

        animations = {
            swing = {
                time_per_frame = 0.2,
                next = "invisible",

                easing = "OutCubic",

                frames = {
                    { rect = { x = 0, y = 0, width = 7, height = 34, }, rotation_center = { x = 4, y = 29, }, rotation = 0, },
                    { rect = { x = 0, y = 0, width = 7, height = 34, }, rotation_center = { x = 4, y = 29, }, rotation = -math.pi/2, },
                },
            },

            invisible = {
                time_per_frame = 999,

                frames = {
                    { rect = { x = 0, y = 0, width = 0, height = 0, }, },
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
            local swing_pos = vec2.new(0.5, -1.4)
            local mirror

            local area = vec2.new(2.5, 3)

            -- Hardcoded player and swing width :(
            if player_pos.x + 0.75 < position.x then
                mirror = false
            else
                mirror = true
                swing_pos.x = swing_pos.x * -1
                area.x = area.x * -1
            end

            local swing = default.spawn_swing(swing_pos, {
                name = "default:"..metal.."_sword_swing",
                damage = { amount = damage, invincible_time = 0.2, knockback = { x = (mirror and -1 or 1) * 3, y = -3, } },
                source = player,
                collision_check = default.area_collision_check(area),
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

terrarium.register_item("default:revolver", {
    description = "Revolver",
    inventory_image = "revolver_item.png",
    max_count = 1,

    use_rate = 2,

    on_start_use = function(user, itemstack, position)
        if user.ref.is_player then
            if user.ref:get_player_inventory():take_items("default:pistol_bullet", 1) ~= 0 then return end
        end

        local spawn_position = user.ref:get_center()

        local bullet_data = {
            speed = (position - spawn_position):resize(80),
            damage = { amount = 10 },
            fuse = 5,
            source = user,
        }

        terrarium.new_entity("default:small_bullet", spawn_position, bullet_data)

        terrarium.play_sound({
            name = "pistol_shoot.wav",
            position = spawn_position,
            volume = 0.2,
        })
    end,
})

terrarium.register_item("default:suppressor_vm_damaged", {
    description = "Suppressor mk1 (damaged)\n4 ranged damage\nStandard abnormality suppression weapon... Was...\nClass: VM",
    inventory_image = "suppressor_vm_damaged.png",
    max_count = 1,

    use_rate = 2,

    on_start_use = function(user, itemstack, position)
        if user.ref.is_player then
            if not user:use_energy(8) then return end
        end

        local spawn_position = user.ref:get_center()

        local bullet_data = {
            speed = (position - spawn_position):resize(80):rotate((math.random()-0.5)*2*(math.pi/24)),
            damage = { amount = 4, element = "antimagic", },
            fuse = 5,
            source = user,
        }

        terrarium.new_entity("default:small_energy_bullet", spawn_position, bullet_data)

        terrarium.play_sound({
            name = "energy_pistol_shoot.wav",
            position = spawn_position,
            volume = 0.2,
        })
    end,
})

terrarium.register_item("default:pistol_bullet", {
    description = "Pistol bullet\nUniversally fits for any kind of pistol or revolver",
    inventory_image = "pistol_bullet.png",
    max_count = 999,
})
