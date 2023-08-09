terrarium.registered_chat_commands = {}

local function is_callable(f)
    if type(f) == "function" then return true end

    local mt = getmetatable(f)

    return mt and mt.__call ~= nil
end

function terrarium.register_command(name, def)
    if not is_callable(def.func) then
        error("field 'func' should be a callable object")
    else
        terrarium.registered_chat_commands[name] = def
    end
end

terrarium.send_message = core._send_message

core._event_handlers["ChatCmd"] = function(event)
    local user = core.get_user(event.chat_cmd.user)
    local cmd = event.chat_cmd.name
    local args = event.chat_cmd.args

    local def = terrarium.registered_chat_commands[cmd]

    if def then
        def.func(user, table.unpack(args))
    else
        core._send_message("Unknown chat command: /"..cmd, color.new(242, 120, 120))
    end
end
