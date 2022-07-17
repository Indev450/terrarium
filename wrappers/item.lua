terrarium.registered_items = {}

local function create_use_context(user_name, itemstack, position, item_name)
    return {
        user_name = user_name,
        itemstack = itemstack,
        item_name = item_name,
        start_position = position,
        use_time = 0,
        dtime = 0,
    }
end

local item_defaults = {
    description = "Default description",
    inventory_image = "",
    max_count = 1,

    on_start_use = function(user, itemstack, position, use_ctx) end,
    on_start_alt_use = function(user, itemstack, position, use_ctx) end,
    on_select = function(user, itemstack, position, use_ctx) end,

    on_use = function(user, itemstack, position, use_ctx) end,
    on_alt_use = function(user, itemstack, position, use_ctx) end,
    on_wield = function(user, itemstack, position, use_ctx) end,

    on_end_use = function(user, itemstack, position, use_ctx) end,
    on_end_alt_use = function(user, itemstack, position, use_ctx) end,
    on_unselect = function(user, itemstack, position, use_ctx) end,
}

function terrarium.register_item(name, def)
    apply_defaults(def, item_defaults)

    core._register_item(name, def)

    terrarium.registered_items[name] = def
end


core._item_users = {}
core._item_alt_users = {}
core._item_wielders = {}

core._event_handlers["ItemUseStart"] = function(event)
    local user = event.item_event.user
    local itemstack = event.item_event.item_stack
    local position = event.item_event.position

    local name = itemstack:get_item_name()

    local def = terrarium.registered_items[name]

    -- TODO - get actual player name
    local user_name = "singleplayer"

    core._item_users[user_name] = create_use_context(user_name, itemstack, position, name)
    local ctx = core._item_users[user_name]

    def.on_start_use(user, itemstack, position, ctx)
end

core._event_handlers["ItemUseStop"] = function(event)
    local user = event.item_event.user
    local itemstack = event.item_event.item_stack
    local position = event.item_event.position

    local name = itemstack:get_item_name()

    local def = terrarium.registered_items[name]

    -- TODO - get actual player name
    local user_name = "singleplayer"

    def.on_end_use(user, itemstack, position, core._item_users[user_name])

    core._item_users[user_name] = nil
end

core._event_handlers["ItemAltUseStart"] = function(event)
    local user = event.item_event.user
    local itemstack = event.item_event.item_stack
    local position = event.item_event.position

    local name = itemstack:get_item_name()

    local def = terrarium.registered_items[name]

    -- TODO - get actual player name
    local user_name = "singleplayer"

    core._item_alt_users[user_name] = create_use_context(user_name, itemstack, position, name)
    local ctx = core._item_alt_users[user_name]

    def.on_start_alt_use(user, itemstack, position, ctx)
end

core._event_handlers["ItemAltUseStop"] = function(event)
    local user = event.item_event.user
    local itemstack = event.item_event.item_stack
    local position = event.item_event.position

    local name = itemstack:get_item_name()

    local def = terrarium.registered_items[name]

    -- TODO - get actual player name
    local user_name = "singleplayer"

    def.on_end_alt_use(user, itemstack, position, core._item_alt_users[user_name])

    core._item_alt_users[user_name] = nil
end

core._event_handlers["ItemSelect"] = function(event)
    local user = event.item_event.user
    local itemstack = event.item_event.item_stack
    local position = event.item_event.position

    local name = itemstack:get_item_name()

    -- Here def can be nil, because player can switch to empty itemstack,
    -- but event has to be sent anyway
    local def = terrarium.registered_items[name]

    -- TODO - get actual player name
    local user_name = "singleplayer"

    local old_ctx = core._item_alt_users[user_name]

    if old_ctx ~= nil then
        terrarium.registered_items[old_ctx.item_name].on_unselect(user, itemstack, position, old_ctx)
    end

    core._item_wielders[user_name] = nil

    -- If player switched to non-empty item stack, execute callback for its item type
    if def ~= nil then
        core._item_alt_users[user_name] = create_use_context(user_name, itemstack, position, name)
        local new_ctx = core._item_alt_users[user_name]

        def.on_select(user, itemstack, position, new_ctx)

        core._item_wielders[user_name] = new_ctx
    end
end

core._update_hooks["item"] = function(dtime)
    -- TODO - get actual list of players
    for _, user_name in pairs({"singleplayer"}) do
        local use_ctx = core._item_users[user_name]
        local alt_use_ctx = core._item_alt_users[user_name]
        local wield_ctx = core._item_wielders[user_name]

        -- Argument is currently ignored
        local user = core._get_player(user_name)

        local position = user:get_player_controls().mouse_pos

        if use_ctx ~= nil then
            use_ctx.use_time = use_ctx.use_time + dtime
            use_ctx.dtime = dtime
            terrarium.registered_items[use_ctx.item_name].on_use(user, use_ctx.itemstack, position, use_ctx)
        end

        if alt_use_ctx ~= nil then
            alt_use_ctx.use_time = alt_use_ctx.use_time + dtime
            alt_use_ctx.dtime = dtime
            terrarium.registered_items[alt_use_ctx.item_name].on_alt_use(user, alt_use_ctx.itemstack, position, alt_use_ctx)
        end

        if wield_ctx ~= nil then
            wield_ctx.use_time = wield_ctx.use_time + dtime
            wield_ctx.dtime = dtime
            terrarium.registered_items[wield_ctx.item_name].on_wield(user, wield_ctx.itemstack, position, wield_ctx)
        end
    end
end
