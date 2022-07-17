terrarium.on_player_join = {}


function terrarium.register_on_player_join(func)
    terrarium.on_player_join[#terrarium.on_player_join+1] = func
end

function core._on_player_join(player)
    for _, func in pairs(terrarium.on_player_join) do
        func(player)
    end
end
