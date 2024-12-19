musicplayer = {}

-- Not based on biome/dungeon, just pure random
local list = {
    "andand.ogg",
    "Chevo.ogg",
    "komplex.ogg",
    "Synthi.ogg",
    "Technob.ogg",
    "Tuntun.ogg",
}

local to_play_list = {}

local boss_music = {}

local function refill_music_list()
    to_play_list = {}

    for i, name in ipairs(list) do
        table.insert(to_play_list, name)
    end
end

local function pick_random_track()
    if #to_play_list == 0 then refill_music_list() end

    local i = math.random(1, #to_play_list)

    local name = to_play_list[i]

    table.remove(to_play_list, i)

    return name
end

function musicplayer.add_background_music(musname)
    table.insert(list, musname)
end

-- When will stop playing music when entity dies or when boss music is stopped explicitly
function musicplayer.play_boss_music(musname, entity)
    assert(entity)

    table.insert(boss_music, {entity = entity, musname = musname})

    terrarium.play_music(musname, true)
end

function musicplayer.stop_boss_music(entity)
    local update_music = false

    if #boss_music == 0 or boss_music[#boss_music].entity == entity then
        update_music = true
    end

    table.remove(boss_music)

    if update_music then
        if #boss_music > 0 then
            terrarium.play_music(boss_music[#boss_music].musname, true)
        else
            musicplayer.update_music()
        end
    end
end

function musicplayer.update_music()
    if not terrarium.is_music_playing() then
        terrarium.play_music(pick_random_track())
    end
end

terrarium.register_on_step(function()
    if #boss_music == 0 then return end

    if not boss_music[#boss_music].entity:is_valid() then
        musicplayer.stop_boss_music(boss_music[#boss_music].entity)
    end
end)

terrarium.register_on_step_timed(60, function()
    musicplayer.update_music()
end, true)
