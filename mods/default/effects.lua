default.effects = {}

local active_effects = {}

local effect_defaults = {
    start = function(self, user)
    end,

    update = function(self, user, dtime)
    end,
}

local function effect_main()

    while timer.tick(coroutine.yield()) do
        update(data, user, dtime)
    end
end

function default.register_effect(name, def)
    default.effects[name] = setmetatable(def, { __index = effect_defaults })
end

function default.give_effect(user, name, time)
    local effect = setmetatable({
        timer = timer.new(time),
        name = name,
    }, { __index = default.effects[name] })

    local effect_thread = coroutine.create(function()
        effect:start(user)

        repeat
            local dtime = coroutine.yield()

            effect:update(user, dtime)
        until not user.ref:is_valid() or effect.timer:tick(dtime)
    end)

    -- Start the effect
    coroutine.resume(effect_thread)

    table.insert(active_effects, effect_thread)
end

terrarium.register_on_step(function(dtime)
    -- Going in reverse order allows removing expired effects right away
    for i = #active_effects, 1, -1 do
        local ok, err = coroutine.resume(active_effects[i], dtime)

        if not ok then
            print(string.format("Error in an effect thread:\n\t%s", err))
        end

        if coroutine.status(active_effects[i]) == "dead" then
            table.remove(active_effects, i)
        end
    end
end)
