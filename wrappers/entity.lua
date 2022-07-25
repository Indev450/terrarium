terrarium.registered_entities = {}

local entity_defaults = {
    physics = {
        gravity = 40,
        enable_collision = true,
    },

    size = {
        width = 1,
        height = 1,
    },

    image = "",

    animations = {},

    on_create = function(self, data) end,

    update = function(self, dtime) end,

    on_removed = function(self) end,
}

local entity_methods = {
    kill = function(self)
        if not self.obj:is_valid() then return end

        core._entities.to_be_removed[self.id] = true
    end,
}

function terrarium.register_entity(name, def)
    apply_defaults(def, entity_defaults)

    def.prefab_id = core._register_entity_prefab(def)

    terrarium.registered_entities[name] = def
end

function terrarium.override_entity(name, def)
    if terrarium.registered_entities[name] == nil then
        error("attempt to override entity prefab "..name.." that doesn't exist")
    end

    apply_defaults(def, entity_default)

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

    luaentity.id = core._entities:get_free_id()

    core._entities.to_be_added[luaentity.id] = luaentity

    luaentity:on_create(data)

    return luaentity
end


core._entities = {
    last_id = 0,

    get_free_id = function(self)
        self.last_id = self.last_id + 1

        while self.list[self.last_id] ~= nil or self.to_be_added[self.last_id] ~= nil do
            self.last_id = self.last_id + 1
        end

        return self.last_id
    end,

    to_be_added = {}, -- In case new_entity is called when iterating over list

    to_be_removed = {},

    list = {},
}

core._update_hooks["entity"] = function(dtime)
    if #core._entities.to_be_added ~= 0 then
        for id, entity in pairs(core._entities.to_be_added) do
            core._entities.list[id] = entity
        end

        core._entities.to_be_added = {}
    end

    if #core._entities.to_be_removed ~= 0 then
        for id, _ in pairs(core._entities.to_be_removed) do
            core._entities.list[id]:on_removed()
            core._entities.list[id].obj:kill()
            core._entities.list[id] = nil
        end

        core._entities.to_be_removed = {}
    end

    for _, entity in pairs(core._entities.list) do
        entity:update(dtime)
    end
end
