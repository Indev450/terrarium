terrarium.register_item("default:copper_pickaxe",  {
    description = "",
    inventory_image = "copper_pickaxe.png",

    on_start_use = function(user, itemstack, position, use_ctx)
        use_ctx.timer = timer.new(0.25)
    end,

    on_start_alt_use = function(user, itemstack, position, use_ctx)
        use_ctx.timer = timer.new(0.25)
    end,

    on_use = function(user, itemstack, position, use_ctx)
        local x = math.floor(position.x)
        local y = math.floor(position.y)

        local block_name = terrarium.get_block(x, y)

        if use_ctx.timer:tick(use_ctx.dtime) and terrarium.dig_block(x, y, user) then
            use_ctx.timer:restart()

            -- Thanks to this if statement, non-player entities can use
            -- our pickaxe!
            if user.is_player then
                -- TODO - wrap this in some kind of function, like "ItemStack(name, count)"
                local itemstack = core._new_item_stack()

                itemstack:set(block_name, 1)

                -- Also, i think i need some kind of "overload" for this function, that takes item name and count.
                -- ...or maybe i need entire wrapper "class"?
                user:get_player_inventory():add_item(itemstack)
            end
        end
    end,

    on_alt_use = function(user, itemstack, position, use_ctx)
        local x = math.floor(position.x)
        local y = math.floor(position.y)

        local block_name = terrarium.get_wall(x, y)

        if use_ctx.timer:tick(use_ctx.dtime) and terrarium.dig_wall(x, y, user) then
            use_ctx.timer:restart()

            -- Thanks to this if statement, non-player entities can use
            -- our pickaxe!
            if user.is_player then
                -- TODO - wrap this in some kind of function, like "ItemStack(name, count)"
                local itemstack = core._new_item_stack()
                itemstack:set(block_name, 1)

                -- Also, i think i need some kind of "overload" for this function, that takes item name and count.
                -- ...or maybe i need entire wrapper "class"?
                user:get_player_inventory():add_item(itemstack)
            end
        end
    end,

})
