-- Some values are squared to do less calculations (to call math.sqrt less
-- frequently)
terrarium.item_entity_settings = {
    magnet_dist_sqr = 5 ^ 2,

    magnet_accel = 80,
    magnet_max_speed_sqr = 50 ^ 2,

    pick_up_dist_sqr = 1 ^ 2,
}

local item_entity = terrarium.item_entity_settings

terrarium.register_entity("__builtin:item_entity", {
    physics = {
        gravity = 20,
        enable_collision = true,
    },

    image = "null.png",

    size = { width = 1, height = 1 },

    on_create = function(self, item_stack)
        self:set_item_stack(item_stack)
    end,

    set_item_stack = function(self, item_stack)
        local existing = self.item_stack

        if not item_stack:empty() then
            self.item_stack = item_stack

            self.ref:set_texture(
                terrarium.registered_items[item_stack:get_item_name()].inventory_image
            )
        else
            self.item_stack = nil

            self.ref:set_texture("null.png")
        end

        return existing
    end,

    update = function(self, dtime)
        if not self.item_stack then return self:kill() end

        local closest = {
            player = nil,
            dist_sqr = nil,
            dir = nil,
        }

        local pos = self.ref:get_position()
        local speed = self.ref:get_speed()

        for _, player in pairs(terrarium.players) do
            local player_pos = player.ref:get_position()

            local dir = player_pos - pos
            local dist_sqr = dir:length_squared()
            local dir = dir:normal()

            if dist_sqr < item_entity.magnet_dist_sqr and
               (closest.player == nil or dist_sqr < closest.dist_sqr) then
                closest.player = player
                closest.dist_sqr = dist_sqr
                closest.dir = dir
            end
        end

        if closest.player ~= nil then
            self.ref:set_collision_enabled(false)

            if closest.dist_sqr < item_entity.pick_up_dist_sqr then
                closest.player.ref:get_player_inventory():add_item(self.item_stack)

                self:kill()

                return
            else
                speed = closest.dir*speed:length() + closest.dir*item_entity.magnet_accel*dtime

                if speed:length_squared() > item_entity.magnet_max_speed_sqr then
                    speed:resize_ip(math.sqrt(item_entity.max_speed_sqr))
                end

                self.ref:set_speed(speed)
            end
        else
            self.ref:set_collision_enabled(true)
        end
    end,
})

function terrarium.drop_item(item_stack, position)
    return terrarium.new_entity("__builtin:item_entity", position, item_stack)
end
