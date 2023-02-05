terraclient = {}

dofile("wrappers/vec2.lua")

local update = {}
local frames = {}
local cmd_handlers = {}

-- API wrappers

function terraclient.register_on_update(fn)
    table.insert(update, fn)
end

function terraclient.add_hud_item(name, def)
    def.click = def.click or function() return false end
    def.scroll = def.scroll or function() return false end
    def.render = def.render or function() end
    def.update = def.update or function() end

    def.width = def.width or 100
    def.height = def.height or 50

    def.draw = function(self, drawable)
        core._draw(self._rtexture, drawable)
    end

    def.clear = function(self, color)
        core._clear(self._rtexture, color or { r = 0, g = 0, b = 0, })
    end

    def.set_visible = function(self, visible)
        core._set_hud_item_visible(name, visible)
    end

    def.set_position = function(self, position)
        core._set_hud_item_position(name, position)
    end

    def.set_origin = function(self, origin)
        core._set_hud_item_origin(name, origin)
    end

    frames[name] = def

    frames[name]._rtexture = core._frame_new(name, def.width, def.height)

    if def.position then def:set_position(def.position) end
    if def.origin then def:set_origin(def.origin) end
    if def.default_visible ~= nil then def:set_visible(def.default_visible) end

    return def
end

function terraclient.sprite(texture, texture_rect)
    return core._sprite_new(texture, texture_rect)
end

function terraclient.text(str, size, color)
    return core._text_new(str, size, color)
end

function terraclient.send_cmd(...)
    core._send_cmd(...)
end

function terraclient.on_cmd(name, fn)
    cmd_handlers[name] = fn
end

-- Hooks called from engine

function core._update(dtime)
    for _, fn in ipairs(update) do fn(dtime) end
    for _, frame in pairs(frames) do frame:update(dtime) end
end

function core._ui_click(id, position)
    return frames[id]:click(position)
end

function core._ui_scroll(id, position, delta)
    return frames[id]:scroll(position, delta)
end

function core._ui_render(id)
    frames[id]:render(target)
end

function core._mod_cmd(cmd)
    local handler = cmd_handlers[cmd.name]

    if handler ~= nil then
        handler(cmd.args)
    else
        print("Unknown command received: "..cmd.name)
    end
end
