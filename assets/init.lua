entity_prefab_id = core._new_entity_prefab({
    physics = {
        gravity = 20*16,

        enable_collision = 1, -- should trigger warning
    },

    size = {
        width = 16,
        height = 16,
    },

    image = "assets/grass.png",
})

entity = core._new_entity(entity_prefab_id)

core._update = function(dtime)
    if entity:get_collision_info().blockd then
        entity:set_speed({x = 10*16, y = -20*16})
    end
end
