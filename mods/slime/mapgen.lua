local function randfloat(min, max)
    return min + math.random() * (max - min)
end

-- Dig hole in blocks layer
local function fill(hx, hy, radius, block_name)
    for x = hx - radius, hx + radius do
        for y = hy - radius, hy + radius do
            if (hx-x)*(hx-x) + (hy-y)*(hy-y) <= radius*radius then
                terrarium.set_block(math.floor(x), math.floor(y), block_name)
            end
        end
    end
end

local function dig_tunnel(startpos, endpos, radius)
    local step = math.sqrt(radius)
    local posdiff = (endpos - startpos)
    local dist = posdiff:length()

    for i = 0, math.floor(dist/step) do
        local t = i/dist*step

        local pos = startpos + posdiff*t

        fill(pos.x, pos.y, radius*randfloat(0.75, 1.25))
    end
end

local function randtunnel(sections, i, radius)
    assert(#sections ~= 1) -- No infinite loops!

    local to_i

    repeat
        to_i = math.random(1, #sections)
    until to_i ~= i

    if not sections[to_i].tunnel_to then sections[to_i].tunnel_to = {} end
    if not sections[i].tunnel_to then sections[i].tunnel_to = {} end

    -- There is already tunnel to that section, no need to waste time
    if sections[to_i].tunnel_to[i] then return end

    sections[i].tunnel_to[to_i] = true
    sections[to_i].tunnel_to[i] = true

    dig_tunnel(sections[i].center, sections[to_i].center, radius)
end

-- Slime hive
terrarium.register_on_map_generated(function()
    local WORLD_SIZE = terrarium.get_world_size()
    local RADIUS = 200
    local SECTION_RADIUS = RADIUS*0.75
    local SECTION_CORE_RADIUS = SECTION_RADIUS*0.15
    local TUNNEL_RADIUS = 3
    local NUM_SECTION_TUNNELS = 2

    local RADIUS_SQR = RADIUS*RADIUS
    local SECTION_RADIUS_SQR = SECTION_RADIUS*SECTION_RADIUS

    local center = vec2.new(
        math.floor(WORLD_SIZE.x * randfloat(0.2, 0.8)),
        math.floor(WORLD_SIZE.y * randfloat(0.4, 0.6))
    )

    local types = { "blu", "cin", "gra", "pur", "ra", "yel", }

    local sections = {}

    for i = 1, #types do
        sections[i] = { center = center + vec2.new(SECTION_RADIUS * randfloat(0.5, 1), 0):rotate(2*math.pi*i/#types), block = "slime:stone_slime"..types[i], type = types[i], }
    end

    local function get_section(x, y)
        local pos = vec2.new(x, y)

        local section = 1
        local dist_sqr = (pos - sections[1].center):length_squared()

        for i = 2, #sections do
            local section_dist_sqr = (pos - sections[i].center):length_squared()

            if section_dist_sqr < dist_sqr then
                dist_sqr = section_dist_sqr
                section = i
            end
        end

        return sections[section], dist_sqr
    end

    for x = center.x - RADIUS, center.x + RADIUS do
        for y = center.y - RADIUS, center.y + RADIUS do
            local pos = vec2.new(x, y)

            if (pos - center):length_squared() < RADIUS_SQR then
                local section, dist_sqr = get_section(x, y)
                local block = section.block

                if dist_sqr < SECTION_RADIUS_SQR then
                    terrarium.set_block(x, y, block)
                    terrarium.set_wall(x, y, block)
                end
            end
        end
    end

    for i = 1, #sections do
        fill(sections[i].center.x, sections[i].center.y, SECTION_CORE_RADIUS)
        fill(sections[i].center.x, sections[i].center.y, SECTION_CORE_RADIUS*0.6, sections[i].block)
    end

    for i = 1, #sections do
        for i = 1, NUM_SECTION_TUNNELS do
            randtunnel(sections, i, TUNNEL_RADIUS)
        end

        dig_tunnel(center, sections[i].center, TUNNEL_RADIUS)

        dig_tunnel(sections[i].center, sections[i].center + vec2.new(SECTION_RADIUS, 0):rotate(2*math.pi*math.random()), TUNNEL_RADIUS)
    end

    for x = center.x - RADIUS, center.x + RADIUS do
        for y = center.y - RADIUS, center.y + RADIUS do
            local pos = vec2.new(x, y)

            if (pos - center):length_squared() < RADIUS_SQR then
                if math.random() < 0.1 then
                    if not terrarium.get_block(x, y) and not terrarium.get_block(x, y+1) and terrarium.get_block(x, y+2) then
                        local grass = (math.random() < 0.5 and "slime:grass_slime" or "slime:grass2_slime")..get_section(x, y).type

                        terrarium.set_multiblock(x, y, grass)
                    end
                elseif math.random() < 0.25 then
                    if not terrarium.get_block(x, y) and terrarium.get_block(x, y+1) then
                        terrarium.set_block(x, y, "slime:grass_small_slime"..get_section(x, y).type)
                    end
                end
            end
        end
    end
end)
