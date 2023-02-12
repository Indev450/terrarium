local respawn_text = terraclient.text("You died :(\nClick here to respawn", 32)
respawn_text:set_position({ x = 100 - 32, y = 100 - 32 })

local respawn_bg_color = {
    r = 128,
    g = 128,
    b = 128,
    a = 128,
}

local respawn = terraclient.add_hud_item("default:respawn", {
    width = 400,
    height = 200,

    position = { x = -200, y = -100 },
    origin = "ScreenCenter",
    default_visible = false,

    click = function(self)
        terraclient.send_cmd("default:player_respawn")
        self:set_visible(false)

        return true
    end,

    render = function(self)
        self:clear(respawn_bg_color)

        self:draw(respawn_text)
    end
})

terraclient.on_cmd("default:player_died", function()
    respawn:set_visible(true)
end)
