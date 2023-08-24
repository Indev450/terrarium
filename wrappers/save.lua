core._savers = {}

function terrarium.register_saver(saver)
    saver.load = saver.load or function() end
    saver.save = saver.save or function() end

    table.insert(core._savers, saver)
end

terrarium.open = core._open

function core._save()
    for _, saver in ipairs(core._savers) do saver:save() end
end

function core._load()
    for _, saver in ipairs(core._savers) do saver:load() end
end
