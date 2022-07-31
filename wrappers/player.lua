terrarium.players = {}

terrarium.on_player_join = {}

local hud_bar_defaults = {
    geometry = {
        x = 0,
        y = 0,
        width = 128,
        height = 32,
    },

    text_style = "None",

    text_size = 18,

    text = "",

    text_color = color.new(255, 255, 255),

    background_color = color.new(0, 0, 0),

    bar_color = color.new(255, 0, 0),

    max_value = 100,
}

local player_methods = {
    get_controls = function(self)
        return self.ref:get_player_controls()
    end,

    -- Currently, those methods don't care (as well as core functions)
    -- about player. When (and if) multiplayer will be added, they will
    -- probably pass player name, or call to self.ref methods instead
    add_bar = function(self, name, def)
        apply_defaults(def, hud_bar_defaults)

        core._hud_bar_create(name, def)
    end,

    set_bar_value = function(self, name, value)
        core._hud_bar_set_value(name, value)
    end,

    set_bar_max_value = function(self, name, value)
        core._hud_bar_set_max_value(name, value)
    end,

    del_bar = function(self, name)
        core._hud_bar_remove(name)
    end,

    show_crafting_ui = function(self, category)
        core._show_crafting_ui(category or terrarium.default_crafting_category)
    end,

    open_inventory_ui = function(self, inventory, width, height)
        core._open_inventory_ui(inventory, width, height)
    end,
}

local function create_player(player_ref)
    return setmetatable({
        ref = player_ref,
    }, {__index = player_methods})
end

function terrarium.register_on_player_join(func)
    terrarium.on_player_join[#terrarium.on_player_join+1] = func
end

function terrarium.get_player(name)
    return terrarium.players[name]
end

function core._on_player_join(player_ref)
    -- should be replaced with something like "player_ref:get_player_name()"
    local player_name = "singleplayer"

    terrarium.players[player_name] = create_player(player_ref)

    local player = terrarium.players[player_name]

    for _, func in pairs(terrarium.on_player_join) do
        func(player)
    end
end
