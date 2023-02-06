core._savers = {}

function terrarium.register_saver(saver)
    saver.load = saver.load or function() end
    saver.save = saver.save or function() end

    table.insert(core._savers, saver)
end

function core._save(save_dir_path)
    for _, saver in ipairs(core._savers) do saver:save(save_dir_path) end
end

function core._load(save_dir_path)
    for _, saver in ipairs(core._savers) do saver:load(save_dir_path) end
end
