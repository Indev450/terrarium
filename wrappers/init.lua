terrarium = {}

core._event_handlers = {}
core._update_hooks = {}

dofile("wrappers/util.lua")

dofile("wrappers/block.lua")
dofile("wrappers/item.lua")
dofile("wrappers/entity.lua")
dofile("wrappers/mapgen.lua")

function core._on_event(event)
    core._event_handlers[event.name](event)
end

function core._update(dtime)
    for _, hook in pairs(core._update_hooks) do
        hook(dtime)
    end
end
