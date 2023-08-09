-- Dunno if i need to generalize and reuse this. Idk if this system is flexible
-- enough to be exported as a separate mod.

local function make_connect_func(self_origin, other_origin)
    local connect_self = "connect_"..self_origin
    local connect_other = "connect_"..other_origin
    local add_connection_self = "add_connection_from_"..self_origin
    local add_connection_other = "add_connection_from_"..other_origin

    return function(self, place_x, place_y, other)
        assert(self[connect_self] ~= nil)
        assert(other[connect_other] ~= nil)

        local other_place_x = place_x + self[connect_self].x - other[connect_other].x
        local other_place_y = place_y + self[connect_self].y - other[connect_other].y

        other:place(other_place_x, other_place_y)

        if self[add_connection_self] then
            self[add_connection_self](self, place_x, place_y)
        end

        if other[add_connection_other] then
            other[add_connection_other](other, other_place_x, other_place_y)
        end

        return other_place_x, other_place_y
    end
end

local connect_left = make_connect_func("left", "right")
local connect_right = make_connect_func("right", "left")
local connect_down = make_connect_func("down", "up")
local connect_up = make_connect_func("up", "down")

local function create_hallway(size, light_freq)
    light_freq = light_freq or 8

    return {
        size = size,

        connect_left = vec2.new(0, size.y - 1),
        connect_right = vec2.new(size.x, size.y - 1),

        place = function(self, place_x, place_y)
            for x = place_x, place_x + self.size.x do
                for y = place_y, place_y + self.size.y do
                    if x <= place_x+1 or x >= place_x + self.size.x-1
                        or y <= place_y+1 or y >= place_y + self.size.y-1 then

                        terrarium.set_block(x, y, "default:metal_block")
                    elseif y == place_y + 2 and x - place_x > 1 and x - place_x < self.size.x - 2 and (x - place_x) % light_freq == 0 then
                        terrarium.set_block(x, y, "default:lab_lamp")
                    else
                        terrarium.set_block(x, y)
                    end

                    terrarium.set_wall(x, y, "default:metal_block")
                end
            end
        end,

        add_connection_from_left = function(self, place_x, place_y)
            for x = place_x, place_x + 1 do
                for y = place_y + self.connect_left.y - 3, place_y + self.connect_left.y-1 do
                    terrarium.set_block(x, y)
                end
            end

            terrarium.set_multiblock(place_x + self.connect_left.x, place_y + self.connect_left.y - 3, "default:steel_door")
        end,

        add_connection_from_right = function(self, place_x, place_y)
            for x = place_x + self.connect_right.x-1, place_x + self.connect_right.x-1 do
                for y = place_y + self.connect_right.y - 3, place_y + self.connect_right.y-1 do
                    terrarium.set_block(x, y)
                end
            end

            terrarium.set_multiblock(place_x + self.connect_right.x, place_y + self.connect_right.y - 3, "default:steel_door")
        end,
    }
end

local function create_stairs(size)
    return {
        size = size,

        connect_up = vec2.new(math.floor(size.x/2), 0),
        connect_left = vec2.new(0, size.y - 1),
        connect_right = vec2.new(size.x, size.y - 1),

        place = function(self, place_x, place_y)
            for x = place_x, place_x + self.size.x do
                for y = place_y, place_y + self.size.y do
                    if x <= place_x+1 or x >= place_x + self.size.x-1
                        or y <= place_y+1 or y >= place_y + self.size.y-1 then

                        if (y > place_y+1 and y < place_y + self.size.y-5) and (x == place_x+1 or x == place_x + self.size.x-1) then
                            terrarium.set_block(x, y, "default:glow_metal_block")
                        else
                            terrarium.set_block(x, y, "default:metal_block")
                        end
                    elseif y - place_y > 2 and y - place_y < self.size.y - 2 and (y - place_y) % 4 == 0 then
                        terrarium.set_block(x, y, "default:metal_platform")
                    else
                        terrarium.set_block(x, y)
                    end

                    terrarium.set_wall(x, y, "default:metal_block")
                end
            end
        end,

        add_connection_from_up = function(self, place_x, place_y)
            for x = place_x + self.connect_up.x - 1, place_x + self.connect_up.x + 1 do
                for y = place_y, place_y + 1 do
                    terrarium.set_block(x, y)
                end
            end
        end,

        add_connection_from_left = function(self, place_x, place_y)
            for x = place_x, place_x + 1 do
                for y = place_y + self.connect_left.y - 3, place_y + self.connect_left.y-1 do
                    terrarium.set_block(x, y)
                end
            end

            terrarium.set_multiblock(place_x + self.connect_left.x, place_y + self.connect_left.y - 3, "default:steel_door")
        end,

        add_connection_from_right = function(self, place_x, place_y)
            for x = place_x + self.connect_right.x-1, place_x + self.connect_right.x-1 do
                for y = place_y + self.connect_right.y - 3, place_y + self.connect_right.y-1 do
                    terrarium.set_block(x, y)
                end
            end

            terrarium.set_multiblock(place_x + self.connect_right.x, place_y + self.connect_right.y - 3, "default:steel_door")
        end,
    }
end

-- Yep thats just smol hallway
local stairs_entrance = create_hallway(vec2.new(8, 9), 4)
stairs_entrance.connect_down = vec2.new(4, 9)
stairs_entrance.add_connection_from_down = function(self, place_x, place_y)
    for x = place_x + self.connect_down.x - 1, place_x + self.connect_down.x + 1 do
        for y = place_y + self.connect_down.y - 1, place_y + self.connect_down.y - 1 do
            terrarium.set_block(x, y, y == place_y + self.connect_down.y - 1 and "default:metal_platform" or nil)
        end
    end
end

local entrance = {
    size = vec2.new(8, 8),

    connect_right = vec2.new(8, 7),

    place = function(self, place_x, place_y)
        for x = place_x, place_x + self.size.x do
            for y = place_y, place_y + self.size.y do
                if x <= place_x+1 or x >= place_x + self.size.x-1
                    or y <= place_y+1 or y >= place_y + self.size.y-1 then

                    terrarium.set_block(x, y, "default:metal_block")
                else
                    terrarium.set_block(x, y)
                end

                terrarium.set_wall(x, y, "default:metal_block")
            end
        end

        terrarium.set_multiblock(place_x, place_y + self.size.y - 4, "default:steel_medium_door")
        terrarium.set_block(place_x + 4, place_y+2, "default:alarm_lamp")
    end,

    add_connection_from_right = function(self, place_x, place_y)
        for x = place_x + self.connect_right.x-1, place_x + self.connect_right.x-1 do
            for y = place_y + self.connect_right.y - 3, place_y + self.connect_right.y-1 do
                terrarium.set_block(x, y)
            end
        end

        terrarium.set_multiblock(place_x + self.connect_right.x, place_y + self.connect_right.y - 3, "default:steel_door")
    end,
}

lootlib.register_loot("default:lab_weapon_container_vm_unique", {
    item = "default:suppressor_vm_damaged",
    count = 1,
    chance = 1,
    unique = true,
})
-- TODO - add more loot
local function placeContainerVM(x, y)
    terrarium.set_multiblock(x, y, "default:lab_weapon_container_vm")

    local inv = terrarium.get_block_inventory(x, y)

    lootlib.add_loot(inv, "default:lab_weapon_container_vm_unique")
end

local hall = {
    size = vec2.new(31, 13),

    connect_left = vec2.new(0, 12),
    connect_right = vec2.new(31, 12),

    place = function(self, place_x, place_y)
        for x = place_x, place_x + self.size.x do
            for y = place_y, place_y + self.size.y do
                if x <= place_x+1 or x >= place_x + self.size.x-1
                    or y <= place_y+1 or y >= place_y + self.size.y-1 then

                    terrarium.set_block(x, y, y == place_y + self.size.y - 1 and "default:glow_metal_block" or "default:metal_block")
                else
                    terrarium.set_block(x, y, math.floor(self.size.x/2) == x - place_x and y == place_y + self.size.y - 2 and "default:ai_projector" or nil)
                end

                terrarium.set_wall(x, y, "default:metal_block")
            end
        end

        placeContainerVM(place_x + 8, place_y + self.size.y - 4)
        placeContainerVM(place_x + self.size.x - 10, place_y + self.size.y - 4)

        terrarium.set_multiblock(place_x + math.floor(self.size.x/2) - 3, place_y + self.size.y - 4, "default:lab_monitor")
        terrarium.set_multiblock(place_x + math.floor(self.size.x/2) + 2, place_y + self.size.y - 4, "default:lab_monitor")
    end,

    add_connection_from_left = function(self, place_x, place_y)
        for x = place_x, place_x + 1 do
            for y = place_y + self.connect_left.y - 3, place_y + self.connect_left.y-1 do
                terrarium.set_block(x, y)
            end
        end

        terrarium.set_multiblock(place_x + self.connect_left.x, place_y + self.connect_left.y - 3, "default:steel_door")
    end,

    add_connection_from_right = function(self, place_x, place_y)
        for x = place_x + self.connect_right.x-1, place_x + self.connect_right.x-1 do
            for y = place_y + self.connect_right.y - 3, place_y + self.connect_right.y-1 do
                terrarium.set_block(x, y)
            end
        end

        terrarium.set_multiblock(place_x + self.connect_right.x, place_y + self.connect_right.y - 3, "default:steel_door")
    end,
}

local function randfloat(a, b)
    return a + math.random()*(b-a)
end

-- TODO
terrarium.register_on_map_generated(function()
    local WORLD_SIZE = terrarium.get_world_size()

    local place_x = math.floor(randfloat(WORLD_SIZE.x/4, 3*WORLD_SIZE.x/4))
    local place_y = math.floor(randfloat(WORLD_SIZE.y/5, WORLD_SIZE.y/4))

    hall:place(place_x, place_y)
    local hallway = create_hallway(vec2.new(math.random(20, 60), 9))
    local hw_left, hw_right = connect_left(hall, place_x, place_y, hallway)

    connect_left(hallway, hw_left, hw_right, entrance)

    local se_place_x, se_place_y = connect_right(hall, place_x, place_y, stairs_entrance)
    connect_right(stairs_entrance, se_place_x, se_place_y, create_hallway(vec2.new(math.random(20, 60), 9)))

    local stairs = create_stairs(vec2.new(8, 21))
    local st_place_x, st_place_y = connect_down(stairs_entrance, se_place_x, se_place_y, stairs)
    connect_right(stairs, st_place_x, st_place_y, create_hallway(vec2.new(math.random(20, 60), 9)))
    connect_left(stairs, st_place_x, st_place_y, create_hallway(vec2.new(math.random(20, 60), 9)))
end)
