local menu_sprite = terraclient.sprite("equipment_menu.png")

local menu_button = terraclient.sprite("equipment_menu_button.png")

local text = terraclient.text("", 16)
text:set_position({ x = 160, y = 60 })

-- TODO - add frame:get_visible or frame:toggle_visible
local menu_visible = false

local function clicked(button, position)
    if position.x < button.x or position.x > button.x + button.width
        or position.y < button.y or position.y > button.y + button.height then

        return false
    end

    return true
end

local menu = terraclient.add_hud_item("default:equipment_menu", {
    width = 256,
    height = 256,

    position = { x = -128, y = -128 },
    origin = "ScreenCenter",
    default_visible = menu_visible,

    slots = {
        { x = 24,  y = 24,  width = 32, height = 32, name = "battle"      },
        { x = 24,  y = 198, width = 32, height = 32, name = "energy"      },
        { x = 198, y = 24,  width = 32, height = 32, name = "protection"  },
        { x = 198, y = 198, width = 32, height = 32, name = "health"      },
        { x = 112, y = 56,  width = 32, height = 32, name = "helmet"      },
        { x = 112, y = 110, width = 32, height = 32, name = "breathplate" },
        { x = 112, y = 162, width = 32, height = 32, name = "leggins"     },
    },

    click = function(self, position)
        for _, slot in ipairs(self.slots) do
            if clicked(slot, position) then
                terraclient.send_cmd("default:unequip", slot.name)
                break
            end
        end

        return true
    end,

    render = function(self)
        self:draw(menu_sprite)

        for _, slot in ipairs(self.slots) do
            if slot.sprite then
                self:draw(slot.sprite)
            end
        end

        self:draw(text)
    end
})

local button = terraclient.add_hud_item("default:open_equipment_menu", {
    width = 45,
    height = 45,

    position = { x = -10 - 45, y = 100 },
    default_visible = true,

    click = function(self)
        menu:set_visible(not menu_visible)
        menu_visible = not menu_visible

        return true
    end,

    render = function(self)
        self:draw(menu_button)
    end,
})

terraclient.on_cmd("default:equip", function(args)
    local slot_name = args[1]
    local image = args[2]

    for _, slot in ipairs(menu.slots) do
        if slot.name == slot_name then
            slot.sprite = terraclient.sprite(image)
            slot.sprite:set_position({ x = slot.x, y = slot.y })
            break
        end
    end
end)

terraclient.on_cmd("default:unequip", function(args)
    local slot_name = args[1]

    for _, slot in ipairs(menu.slots) do
        if slot.name == slot_name then
            -- Probably i could preserve that sprite but eh, player don't equip
            -- items that often anyway, so no need for this optimization
            slot.sprite = nil
            break
        end
    end
end)

terraclient.on_cmd("default:stats", function(args)
    text:set_string(args[1])
end)
