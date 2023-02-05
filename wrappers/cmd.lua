core._cmd = {}

-- Currently ignores player argument. Will pass it too when multiplayer will be
-- there
function terrarium.send_cmd(player, ...)
    core._send_cmd(...)
end

function terrarium.on_cmd(name, fn)
    core._cmd[name] = fn
end

core._event_handlers["ModCmd"] = function(event)
    local user = core.get_user(event.mod_cmd.user)
    local cmd = event.mod_cmd.name
    local args = event.mod_cmd.args

    local handler = core._cmd[cmd]

    if handler ~= nil then
        handler(user, args)
    else
        print("Unknown command received: "..cmd)
    end
end
