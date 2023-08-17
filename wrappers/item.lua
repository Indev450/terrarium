terrarium.registered_items = {}

local function create_use_context(user, itemstack, position, item_name)
    return {
        user = user,
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
    use_rate = 4, -- Uses per second
    alt_use_rate = nil,

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

    if def.alt_use_rate == nil then
        def.alt_use_rate = def.use_rate
    end

    core._register_item(name, def)

    terrarium.registered_items[name] = def
end


-- Keys are tables, so its better to keep them as weak references
core._item_users = setmetatable({}, {__mode='k'})
core._item_alt_users = setmetatable({}, {__mode='k'})
core._item_wielders = setmetatable({}, {__mode='k'})

core._item_timers = setmetatable({}, {__mode='k'})
core._alt_item_timers = setmetatable({}, {__mode='k'})

-- Convert user from item_event.user to table. If user_ref is player,
-- return player table from terrarium.players. If it is not, then it is already
-- table - "lua entity", which is simply returned. (it may be lua entity if
-- some entity will trigger item event by passing self as event.item_event.user
function core.get_user(user_ref)
    if user_ref.is_player then
        -- user_ref:get_player_name()
        local player_name = "singleplayer"

        return terrarium.players[player_name]
    end

    return user_ref
end

core._event_handlers["ItemUseStart"] = function(event)
    local user = core.get_user(event.item_event.user)
    local itemstack = event.item_event.item_stack
    local position = event.item_event.position

    local use_timer = core._item_timers[user]

    if use_timer ~= nil and not use_timer:ready() then
        return
    end

    if use_timer ~= nil then
        use_timer:restart()
    end

    local name = itemstack:get_item_name()

    local def = terrarium.registered_items[name]

    core._item_users[user] = create_use_context(user, itemstack, position, name)
    local ctx = core._item_users[user]

    if def ~= nil then
        def.on_start_use(user, itemstack, position, ctx)
    end
end

core._event_handlers["ItemUseStop"] = function(event)
    local user = core.get_user(event.item_event.user)
    local itemstack = core._item_users[user].itemstack
    local position = event.item_event.position

    local name = itemstack:get_item_name()

    local def = terrarium.registered_items[name]

    if def ~= nil then
        def.on_end_use(user, itemstack, position, core._item_users[user])
    end

    core._item_users[user] = nil
end

core._event_handlers["ItemAltUseStart"] = function(event)
    local user = core.get_user(event.item_event.user)
    local itemstack = event.item_event.item_stack
    local position = event.item_event.position

    local use_timer = core._alt_item_timers[user]

    if use_timer ~= nil and not use_timer:ready() then
        return
    end

    if use_timer ~= nil then
        use_timer:restart()
    end

    local name = itemstack:get_item_name()

    local def = terrarium.registered_items[name]

    core._item_alt_users[user] = create_use_context(user, itemstack, position, name)
    local ctx = core._item_alt_users[user]

    if def ~= nil then
        def.on_start_alt_use(user, itemstack, position, ctx)
    end
end

core._event_handlers["ItemAltUseStop"] = function(event)
    local user = core.get_user(event.item_event.user)
    local itemstack = core._item_alt_users[user].itemstack
    local position = event.item_event.position

    local name = itemstack:get_item_name()

    local def = terrarium.registered_items[name]

    if def ~= nil then
        def.on_end_alt_use(user, itemstack, position, core._item_alt_users[user])
    end

    core._item_alt_users[user] = nil
end

core._event_handlers["ItemSelect"] = function(event)
    local user = core.get_user(event.item_event.user)
    local itemstack = event.item_event.item_stack
    local position = event.item_event.position

    local name = itemstack:get_item_name()

    -- Here def can be nil, because player can switch to empty itemstack,
    -- but event has to be sent anyway
    local def = terrarium.registered_items[name]

    local old_ctx = core._item_wielders[user]

    if old_ctx ~= nil then
        local old_def = terrarium.registered_items[old_ctx.item_name]

        if old_def ~= nil then
            old_def.on_unselect(user, itemstack, position, old_ctx)
        end
    end

    core._item_wielders[user] = nil

    core._item_timers[user] = nil
    core._alt_item_timers[user] = nil

    -- If player switched to non-empty item stack, execute callback for its item type
    if def ~= nil then
        core._item_timers[user] = timer.new(1/def.use_rate)
        core._alt_item_timers[user] = timer.new(1/def.alt_use_rate)

        core._item_wielders[user] = create_use_context(user, itemstack, position, name)
        local new_ctx = core._item_wielders[user]

        def.on_select(user, itemstack, position, new_ctx)
    end
end

core._update_hooks["item"] = function(dtime)
    -- TODO - get actual list of players
    for user, ctx in pairs(core._item_users) do
        local position = user:get_controls().mouse_pos

        ctx.use_time = ctx.use_time + dtime
        ctx.dtime = dtime

        local def = terrarium.registered_items[ctx.itemstack:get_item_name()]

        if def ~= nil then
            def.on_use(user, ctx.itemstack, position, ctx)
        end
    end

    for user, ctx in pairs(core._item_alt_users) do
        local position = user:get_controls().mouse_pos

        ctx.use_time = ctx.use_time + dtime
        ctx.dtime = dtime

        local def = terrarium.registered_items[ctx.itemstack:get_item_name()]

        if def ~= nil then
            def.on_alt_use(user, ctx.itemstack, position, ctx)
        end
    end

    for user, ctx in pairs(core._item_wielders) do
        local position = user:get_controls().mouse_pos

        ctx.use_time = ctx.use_time + dtime
        ctx.dtime = dtime

        local timer = core._item_timers[user]
        local alt_timer = core._alt_item_timers[user]

        if timer ~= nil then timer:tick(dtime) end
        if alt_timer ~= nil then alt_timer:tick(dtime) end

        local def = terrarium.registered_items[ctx.itemstack:get_item_name()]

        if def ~= nil then
            def.on_wield(user, ctx.itemstack, position, ctx)
        end
    end
end
