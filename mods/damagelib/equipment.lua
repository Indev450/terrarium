damagelib.equipment = {}

function damagelib.equipment.get_elemental_power(gear, element)
    local total = 1

    if not gear then return total end

    for name, item in pairs(gear) do
        local accessory = item

        if type(item) == "userdata" then
            accessory = terrarium.registered_items[item:get_item_name()] or {}
        end

        total = total * ((accessory.elemental_power or {})[element] or 1)
    end

    return total
end

function damagelib.equipment.get_elemental_resistance(gear, element)
    local total = 1

    if not gear then return total end

    for _, item in pairs(gear) do
        local accessory = item

        if type(item) == "userdata" then
            accessory = terrarium.registered_items[item:get_item_name()] or {}
        end

        total = total * ((accessory.elemental_resistance or {})[element] or 1)
    end

    return total
end

function damagelib.equipment.get_protection(gear)
    local total = 0

    if not gear then return total end

    for _, item in pairs(gear) do
        local accessory = item

        if type(item) == "userdata" then
            accessory = terrarium.registered_items[item:get_item_name()] or {}
        end

        total = total + (accessory.protection or 0)
    end

    return total
end

function damagelib.equipment.get_stats(gear)
    local protection = damagelib.equipment.get_protection(gear)

    local resists = {}

    -- #elements will always return 0, but we wanna know if we have any
    -- resistances
    local have_elemental_resistances = false

    for _, item in pairs(gear) do
        local accessory = item

        if type(item) == "userdata" then
            accessory = terrarium.registered_items[item:get_item_name()] or {}
        end

        for element, resistance in pairs(accessory.elemental_resistance or {}) do
            resists[element] = (resists[element] or 1) * resistance
            have_elemental_resistances = true
        end
    end

    if not have_elemental_resistances then
        resists = nil
    end

    local powers = {}

    -- Same as with resists
    local have_elemental_powers = false

    for _, item in pairs(gear) do
        local accessory = item

        if type(item) == "userdata" then
            accessory = terrarium.registered_items[item:get_item_name()] or {}
        end

        for element, resistance in pairs(accessory.elemental_power or {}) do
            powers[element] = (powers[element] or 1) * resistance
            have_elemental_powers = true
        end
    end

    if not have_elemental_powers then
        powers = nil
    end

    return protection, resists, powers
end
