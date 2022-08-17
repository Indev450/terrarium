local tool_defaults = {
    groups = {},
    uses_per_second = 4,
}

-- Checks block groups and tool groups.
-- Block groups should have minimum required levels to break them, and tool
-- groups are these levels. This function returns true if it finds block group
-- which level less than tool level
local function check_groups(block_groups, tool_groups)
    for group, level in pairs(tool_groups) do
        local block_level = block_groups[group]

        if block_level ~= nil and block_level <= level then
            return true
        end
    end

    return false
end

-- Register item that is used to dig blocks
function terrarium.register_tool(name, def)
    apply_defaults(def, tool_defaults)

    def.on_start_use = function(user, itemstack, position, use_ctx)
        use_ctx.timer = timer.new(1/def.uses_per_second)
    end

    def.on_start_alt_use = function(user, itemstack, position, use_ctx)
        use_ctx.timer = timer.new(1/def.uses_per_second)
    end

    def.on_use = function(user, itemstack, position, use_ctx)
        local x = math.floor(position.x)
        local y = math.floor(position.y)

        local block_name = terrarium.get_block(x, y)

        local block_def = terrarium.registered_blocks[block_name]
        local block_groups = {}

        if block_def ~= nil then
            block_groups = block_def.groups
        end

        if use_ctx.timer:tick(use_ctx.dtime)
           and check_groups(block_groups, def.groups)
           and terrarium.dig_block(x, y, user) then

           use_ctx.timer:restart()

            if user.ref.is_player then
                terrarium.give_block_drops(block_name, user)
            end
        end
    end

    def.on_alt_use = function(user, itemstack, position, use_ctx)
        local x = math.floor(position.x)
        local y = math.floor(position.y)

        local block_name = terrarium.get_wall(x, y)

        local block_def = terrarium.registered_blocks[block_name]
        local block_groups = {}

        if block_def ~= nil then
            block_groups = block_def.groups
        end

        if use_ctx.timer:tick(use_ctx.dtime)
           and check_groups(block_groups, def.groups)
           and terrarium.dig_wall(x, y, user) then

            use_ctx.timer:restart()

            if user.ref.is_player then
                terrarium.give_block_drops(block_name, user)
            end
        end
    end

    terrarium.register_item(name, def)
end
