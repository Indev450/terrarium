core._on_step = {}

function terrarium.register_on_step(fn)
    table.insert(core._on_step, fn)
end

function terrarium.register_on_step_timed(time, fn)
    local t = timer.new(time, true)

    terrarium.register_on_step(function(dtime)
        if t:tick(dtime) then fn() end
    end)
end

core._update_hooks["step"] = function(dtime)
    for _, fn in ipairs(core._on_step) do
        fn(dtime)
    end
end