lootlib = {
    registered = {},
}

local loot_defaults = {
    item = "",
    count = 1,
    chance = 0.01,
    unique = false,
}

function lootlib.register_loot(group, def)
    apply_defaults(def, loot_defaults)

    lootlib.registered[group] = lootlib.registered[group] or {}

    table.insert(lootlib.registered[group], def)
end

function lootlib.get_random_loot(group, unique_table)
    local items = lootlib.registered[group]

    if items == nil then return end

    for i = 1, #items do
        local i = math.random(1, #items)

        if not (unique_table and unique_table[i]) and math.random() < items[i].chance then
            local count

            if type(items[i].count) == 'table' then
                count = math.random(items[i].count.min, items[i].count.max)
            else
                count = items[i].count
            end

            if unique_table then unique_table[i] = items[i].unique end

            return ItemStack(items[i].item, count), items[i]
        end
    end
end

function lootlib.add_loot(inv, group, unique_table)
    local item, lootdef = lootlib.get_random_loot(group, unique_table)

    if item then inv:add_item(item) end

    return item, lootdef
end
