local sound_defaults = {
    volume = 1.0,
    pitch = 1.0,
}

function terrarium.play_sound(sound)
    sound = deep_copy(sound)
    apply_defaults(sound, sound_defaults)

    return core._play_sound(sound)
end

function terrarium.stop_sound(handle)
    core._stop_sound(handle)
end
