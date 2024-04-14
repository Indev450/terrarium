default_player_saver = {
    load = function(self)
        local save = datafile.from_file("default_save.txt")

        for pname, pdata in save:iter_children() do
            self.data[pname] = {
                gave_init_stuff = pdata["gave_init_stuff"]:get_bool(),
                equipment = {}
            }

            local equipment = self.data[pname].equipment

            for slot, itemdata in pdata["equipment"]:iter_children() do
                equipment[slot] = ItemStack(itemdata["name"]:get_string(), itemdata["count"]:get_number())
            end
        end
    end,

    save = function(self)
        local save = datafile.new()

        for name, save_data in pairs(self.data) do
            local pdata = save[name]

            if save_data.gave_init_stuff then
                pdata["gave_init_stuff"]:set_bool(true)
            end

            local equipment = pdata["equipment"]

            for slot, item_stack in pairs(save_data.equipment) do
                local itemdata = equipment[slot]
                itemdata["name"]:set_string(item_stack:get_item_name())
                itemdata["count"]:set_number(item_stack:get_item_count())
            end
        end

        save:to_file("default_save.txt")
    end,

    get_save_data = function(self, playername)
        if not self.data[playername] then
            self.data[playername] = {
                gave_init_stuff = false,
                equipment = {},
            }
        end

        return self.data[playername]
    end,

    data = {}
}

terrarium.override_player({
    size = {
        x = 1.5,
        y = 3,
    },

    hitbox = {
        x = 0.2,
        y = 0.4,
        width = 1.1,
        height = 3 - 1/16 - 0.4,
    },

    image = "player.png",

    animations = {
        invisible = {
            time_per_frame = 999,

            frames = {
                { rect = { x = 24*4, y = 0, width = 24, height = 48, }, },
            },
        },

        idle = {
            time_per_frame = 999,

            frames = {
                { rect = { x = 0, y = 0, width = 24, height = 48, }, },
            },
        },

        jump_start = {
            time_per_frame = 0.2/3,

            next = "jump",

            easing = "InCubic",

            frames = {
                { rect = { x = 24*1, y = 0, width = 24, height = 48, }, scale_center = { x = 12, y = 24, }, },
                { rect = { x = 24*2, y = 0, width = 24, height = 48, }, scale = { x = 0.8, y = 1.2, }, scale_center = { x = 12, y = 24, }, },
                { rect = { x = 24*2, y = 0, width = 24, height = 48, }, scale_center = { x = 12, y = 24, }, },
            },
        },

        jump = {
            time_per_frame = 999,

            frames = {
                { rect = { x = 24*3, y = 0, width = 24, height = 48, }, },
            },
        },

        walk = {
            time_per_frame = 0.09,

            frames = {
                { rect = { x = 24*0, y = 48, width = 24, height = 48, }, },
                { rect = { x = 24*1, y = 48, width = 24, height = 48, }, },
                { rect = { x = 24*2, y = 48, width = 24, height = 48, }, },
                { rect = { x = 24*3, y = 48, width = 24, height = 48, }, },
                { rect = { x = 24*4, y = 48, width = 24, height = 48, }, },
                { rect = { x = 24*5, y = 48, width = 24, height = 48, }, },
            }
        }
    },
})

terrarium.register_on_player_join(function(player)
    -- player:get_player_name()
    local name = "singleplayer"

    local save_data = default_player_saver:get_save_data(name)

    if not save_data.gave_init_stuff then
        save_data.gave_init_stuff = true

        player.ref:get_player_inventory():add_item(ItemStack("default:copper_pickaxe", 1))
        player.ref:get_player_inventory():add_item(ItemStack("default:copper_axe", 1))
        player.ref:get_player_inventory():add_item(ItemStack("default:copper_sword", 1))
    end

    player.equipment = save_data.equipment

    player.team = 0

    player.max_hp = 100
    player.hp = player.max_hp

    -- HP/second
    player.hp_regeneration = 0.2
    player.hp_regeneration_timer = timer.new(1, true)

    -- Invincibility timer. 0.5 seconds of invincibility when spawn
    player.inv_timer = timer.new(0.5)

    player:add_bar("hp", {
        geometry = {
            x = -210,
            y = 10,
            width = 200,
            height = 40,
        },

        text_style = "Division",
        text = "HP: ",
        text_size = 24,
        text_color = color.new(255, 255, 255),

        background_color = color.new(0, 0, 0),
        bar_color = color.new(212, 24, 12),
        max_value = player.max_hp,
    })

    player.max_energy = 40
    player.energy = player.max_energy

    -- It is in units/second too, but it works different way - it will start
    -- regenerate only if player haven't used any energy for short period of
    -- time
    player.energy_regeneration = 10
    player.energy_regeneration_timer = timer.new(1.0)

    player:add_bar("energy", {
        geometry = {
            x = -210,
            y = 60,
            width = 200,
            height = 40,
        },

        text_style = "Division",
        text = "EP: ",
        text_size = 24,
        text_color = color.new(255, 255, 255),

        background_color = color.new(0, 0, 0),
        bar_color = color.new(96, 16, 142),
        max_value = player.max_energy,
    })

    local function send_stats()
        local protection, resists, powers, dmg_modifier = damagelib.equipment.get_stats(player.equipment)

        local buff = {}

        table.insert(buff, "Protection: "..protection)

        if resists then
            table.insert(buff, "Elemental armor bonuses:")

            for name, value in pairs(resists) do
                table.insert(buff, name..": x"..value)
            end
        end

        if powers then
            table.insert(buff, "Elemental attack bonuses:")

            for name, value in pairs(powers) do
                table.insert(buff, name..": x"..value)
            end
        end

        if dmg_modifier ~= 1 then
            table.insert(buff, "Damage modifier: x"..dmg_modifier)
        end

        terrarium.send_cmd(player, "default:stats", table.concat(buff, "\n"))
    end

    player.set_max_hp = function(self, max_hp)
        self:set_bar_max_value("hp", max_hp)
        self.max_hp = max_hp

        if self.hp > max_hp then
            self.hp = max_hp
        end
    end

    player.heal = function(self, hp)
        if self.hp <= 0 then return end

        self.hp = math.min(self.max_hp, self.hp + hp)
    end

    -- Flag for damagelib to not remove player entity when player is killed.
    player.no_kill = true

    -- Don't kill player in 1 touch pls
    player.default_inv_time = 0.5

    player.on_death = function(self, damage, source, knockback)
        terrarium.send_cmd(self, "default:player_died")
    end

    player.hurt = damagelib.entity_hurt

    player.use_energy = function(self, amount)
        if amount > self.energy then return false end

        self.energy = self.energy - amount

        self.energy_regeneration_timer:restart()

        return true
    end

    player.restore_energy = function(self, amount)
        self.energy = math.min(self.max_energy, self.energy + amount)
    end

    player.set_max_energy = function(self, max_energy)
        self:set_bar_max_value("energy", max_energy)
        self.max_energy = max_energy

        if self.energy > max_energy then
            self.energy = max_energy
        end
    end

    player.equip = function(self, slot, item_stack)
        local item_def = terrarium.registered_items[item_stack:get_item_name()]

        if not item_def or not item_def.equip_slot == slot then return end

        if self.equipment[slot] then
            self:unequip(slot, item_stack)
        end

        self.equipment[slot] = ItemStack(item_stack)
        item_stack:set("", 0)

        if item_def.on_equip then item_def.on_equip(self, self.equipment[slot]) end

        terrarium.send_cmd(self, "default:equip", slot, item_def.inventory_image)
        send_stats()
    end

    player.unequip = function(self, slot)
        if not self.equipment[slot] then return end

        local item_def = terrarium.registered_items[self.equipment[slot]:get_item_name()]

        local item_stack = self.equipment[slot]

        self.equipment[slot] = nil

        if item_def and item_def.on_unequip then
            item_def.on_unequip(self, item_stack)
        end

        player.ref:get_player_inventory():add_item(item_stack)

        terrarium.send_cmd(self, "default:unequip", slot)
        send_stats()
    end

    player.respawn = function(self)
        player.hp = player.max_hp
        player.ref:set_player_controlled(true)
        player.ref:set_position({x = 0, y = 0})
    end

    for slot, item_stack in pairs(player.equipment) do
        local def = terrarium.registered_items[item_stack:get_item_name()]

        if def then
            if def.on_equip then def.on_equip(player, item_stack) end

            terrarium.send_cmd(self, "default:equip", slot, def.inventory_image)
        end
    end

    send_stats()

    player.ref:set_animation("idle")
end)

terrarium.register_on_player_update(function(player, dtime)
    player:set_bar_value("hp", math.max(0, player.hp))
    player:set_bar_value("energy", math.max(0, player.energy))
    player.inv_timer:tick(dtime)
    player.energy_regeneration_timer:tick(dtime)

    if player.hp <= 0 then
        player.ref:set_player_controlled(false)
        return
    end

    if player.energy_regeneration_timer:ready() then
        player:restore_energy(player.energy_regeneration * dtime)
    end

    if player.hp_regeneration_timer:tick(dtime) then
        player:heal(player.hp_regeneration)
    end
end)

terrarium.on_cmd("default:player_respawn", function(player)
    if player.hp <= 0 then player:respawn() end
end)

terrarium.on_cmd("default:unequip", function(player, args)
    local slot = args[1]

    if not slot then return end

    player:unequip(slot)
end)

-- Update animation
terrarium.register_on_player_update(function(player, dtime)
    local speed = player.ref:get_speed()
    local collision = player.ref:get_collision_info()
    local input = player.ref:get_player_controls()

    if player.hp <= 0 then
        player.ref:set_animation("invisible")
        return
    end

    if input.right and speed.x > 0 then
        player.ref:set_mirror(false)
    end

    if input.left and speed.x < 0 then
        player.ref:set_mirror(true)
    end

    if collision.blockd and input.jump then
        player.ref:set_animation("jump_start")
        return
    end

    if (input.left or input.right) and collision.blockd then
        player.ref:set_animation("walk")
        return
    end

    if collision.blockd then
        player.ref:set_animation("idle")
        return
    elseif player.ref:get_animation() ~= "jump_start" then
        player.ref:set_animation("jump")
    end
end)

terrarium.register_saver(default_player_saver)
