-- Builtin commands

terrarium.register_command("help", {
    help = "Usage: /help [command]. Print information about command or, without argument, print list of available commands",
    func = function(user, command)
        if not command then
            local commands = {}

            for cmd, _ in pairs(terrarium.registered_chat_commands) do
                table.insert(commands, cmd)
            end

            core._send_message("Available commands: "..table.concat(commands, ", "))
        elseif terrarium.registered_chat_commands[command] then
            if terrarium.registered_chat_commands[command].help then
                core._send_message(terrarium.registered_chat_commands[command].help)
            else
                core._send_message("No help for command /"..command.." is available", color.new(242, 120, 120))
            end
        else
            core._send_message("Command /"..command.." not found", color.new(242, 120, 120))
        end
    end,
})

-- Just a shortcut
local function usage(user, cmd)
    terrarium.registered_chat_commands["help"].func(user, cmd)
end

terrarium.register_command("spawnitem", {
    help = "Usage: /spawnitem <item> [amount]. Spawn item at cursor position",
    func = function(user, item, amount)
        if not item then return usage(user, "spawnitem") end

        local pos = user:get_controls().mouse_pos

        terrarium.drop_item(ItemStack(item, tonumber(amount)), pos)
    end,
})

terrarium.register_command("teleport", {
    help = "Usage: /teleport. Teleports player at cursor position",
    func = function(user)
        local pos = user:get_controls().mouse_pos - (user.ref:get_center() - user.ref:get_position())

        user.ref:set_position(pos)
    end,
})

terrarium.register_command("me", {
    help = "Usage: /me <message>. Prints \"* Player <msg>\"",
    func = function(user, ...)
        if not ... then return usage(user, "me") end

        terrarium.send_message("* signleplayer "..table.concat({...}, " "))
    end,
})
