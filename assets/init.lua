entity_prefab_id = core._new_entity_prefab({
    physics = {
        gravity = 20,

        enable_collision = 1, -- should trigger warning
    },

    size = {
        width = 1,
        height = 1,
    },

    image = "assets/grass.png",
})

entity = core._new_entity(entity_prefab_id)

core._update = function(dtime)
    if entity:get_collision_info().blockd then
        entity:set_speed({x = 10, y = -20})
    end
end

core._on_event = function(event)
    print("Event "..event.name.." happened!")

    if event.name == "ItemUseStart" then
        if event.item_event.item_stack:get_item_name() == "default:dirt" then
            local item_stack = event.item_event.item_stack
            local position = event.item_event.position

            if core._get_block(math.floor(position.x), math.floor(position.y)) == 0 then
                item_stack:add(-1)
                core._set_block(math.floor(position.x), math.floor(position.y), 1)
            end
        end
    end
end
