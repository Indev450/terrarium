terrarium.registered_entities = {}

local entity_defaults = {
    physics = {
        gravity = 40,
        enable_collision = true,
    },

    size = {
        x = 1,
        y = 1,
    },

    image = "",

    animations = {},

    on_create = function(self, data) end,

    update = function(self, dtime) end,

    on_removed = function(self) end,
}

local entity_methods = {
    kill = function(self)
        if not self.ref:is_valid() and not core._entities.to_be_removed[self] then return end

        self:on_removed()

        core._entities.to_be_removed[self] = true
    end,
}

function terrarium.register_entity(name, def)
    apply_defaults(def, entity_defaults)

    def.prefab_id = core._new_entity_prefab(def)

    terrarium.registered_entities[name] = def
end

function terrarium.override_entity(name, def)
    if terrarium.registered_entities[name] == nil then
        error("attempt to override entity prefab "..name.." that doesn't exist")
    end

    apply_defaults(def, entity_defaults)

    core._override_entity_prefab(terrarium.registered_entities[name].prefab_id, def)

    terrarium.registered_entities[name] = def
end

function terrarium.override_player(def)
    apply_defaults(def, entity_defaults)

    -- 1 is guaranteed to be player prefab id
    core._override_entity_prefab(1, def)
end

function terrarium.new_entity(name, position, data)
    local def = terrarium.registered_entities[name]

    if def == nil then error("attempt to create entity of unknown type '"..name.."'") end

    local luaentity = setmetatable(deep_copy(def), {__index = entity_methods})

    luaentity.ref = core._new_entity(def.prefab_id)

    luaentity.ref:set_position(position)

    table.insert(core._entities.to_be_added, luaentity)

    luaentity:on_create(data)

    return luaentity
end

-- Tries to spawn entity off every player's screen, but near specified player.
-- `dist` is maximum distance from player camera border
-- If `predicate` isn't nil, it is used as `function(position)` that should
-- return true if entity should be spawned at selected spot. If attempts count
-- set, this function will retry at most `attempts` times until it doesn't fail.
-- Infinite amount attempts not supported to avoid locking the game
-- FIXME - too many arguments.
function terrarium.try_spawn_entity_at(name, data, player, dist, predicate, attempts)
    local dist = dist or 4
    local predicate = predicate or function(position) return true end
    local attempts = attempts or 1

    local camera = player.ref:get_player_camera()

    while attempts > 0 do
        attempts = attempts - 1

        local variant = math.random(1, 3)
        -- 3 |  2  | 3
        -- --+-----+--
        --   |  o  |
        -- 1 | /T\ | 1
        --   |  A  |
        -- --+-----+--
        -- 3 |  2  | 3
        local position = {
            x = 0,
            y = 0,
        }

        if variant == 1 then
            local xoff = dist - math.random()*2*dist

            position.x = camera.x + xoff

            if xoff >= 0 then position.x = position.x + camera.width end

            position.y = camera.y + math.random()*camera.height
        elseif variant == 2 then
            local yoff = dist - math.random()*2*dist

            position.x = camera.x + math.random()*camera.width

            position.y = camera.y + yoff

            if yoff >= 0 then position.y = position.y + camera.height end
        else
            local xoff = dist - math.random()*2*dist
            local yoff = dist - math.random()*2*dist

            position.x = camera.x + xoff
            position.y = camera.y + yoff

            if xoff >= 0 then position.x = position.x + camera.width end
            if yoff >= 0 then position.y = position.y + camera.height end
        end

        -- Should iterate over every player and check if this position is within
        -- their camera. No need for this currently, because there is only one
        -- player and no api designed for multiple players

        if predicate(position) then
            return terrarium.new_entity(name, position, data)
        end
    end
end

-- Returns an iterator over entity list. If predicate is not nil, then
-- it used as function that takes one argument - entity, and returns boolean.
-- Iterator goes through all entities which for predicate returns true
function terrarium.iter_entities(predicate)
    local predicate = predicate or function(entity) return true end
    local i = 0

    return function()
        repeat
            i = i + 1
        until i > #core._entities.list or predicate(core._entities.list[i])

        return core._entities.list[i]
    end
end

-- Returns iterator that goes through all entities which have all required keys
-- For example: terrarium.iter_entities_with {"health", "hunger"}
function terrarium.iter_entities_with(keys)
    return terrarium.iter_entities(function(entity)
        for _, key in pairs(keys) do
            if entity[key] == nil then return false end
        end

        return true
    end)
end

-- Returns iterator that goes through all entities within certain area.
-- TODO - Combine that with variant above
function terrarium.iter_entities_within_area(area)
    return terrarium.iter_entities(function(entity)
        return entity.ref:is_in_area(area)
    end)
end

core._entities = {
    -- List of entities to add in this frame
    to_be_added = {},

    -- Set of entities to remove in this frame (keys are entities themself,
    -- values are true, usually)
    to_be_removed = {},

    -- List of active lua entities
    list = {},
}

core._update_hooks["entity"] = function(dtime)
    -- Add entities
    if #core._entities.to_be_added ~= 0 then
        for i = 1, #core._entities.to_be_added do
            table.insert(core._entities.list, core._entities.to_be_added[i])
        end

        core._entities.to_be_added = {}
    end

    -- on_removed callback may modify core._entities.to_be_removed, so we
    -- need this little protection
    local to_be_removed = core._entities.to_be_removed
    core._entities.to_be_removed = {}

    local indices_to_remove = {}

    for i, entity in ipairs(core._entities.list) do
        -- If entity needed to be removed, add its index to list, otherwise just
        -- update it
        local valid = entity.ref:is_valid()

        if to_be_removed[entity] or not valid then
            table.insert(indices_to_remove, i)

        else
            entity:update(dtime)
        end
    end

    -- indices_to_remove is sorted, so we can just iterate
    -- backwards, and because of that we don't need to shift indices
    for i = #indices_to_remove, 1, -1 do
        core._entities.list[indices_to_remove[i]].ref:kill()
        table.remove(core._entities.list, indices_to_remove[i])
    end
end
