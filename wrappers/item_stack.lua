-- Convert item string, like "default:stone 5", into item stack object
local function string_to_itemstack(s)
    -- All symbols until whitespace are item name
    local item_type = s:match("[^%s]+")

    local item_count = tonumber(s:sub(s:find(item_type) + #item_type)) or 1

    local istack = core._new_item_stack()

    istack:set(item_type, item_count)

    return istack
end

-- Same, but for tables with fields "item_type" and "item_count"
local function table_to_itemstack(t)
    local istack = core._new_item_stack()

    istack:set(t.item_type, t.item_count)

    return istack
end

-- Kind of "copy contructor" for item stack
local function itemstack_copy(istack)
    local istack = core._new_item_stack()

    istack:set(istack:get_item_name(), istack:get_item_count())

    return istack
end

-- And this function combines all possible item stack "initializers"
-- into one "constructor with overloads"
-- Possible arguments:
-- ItemStack(ItemStack copy)
-- ItemStack(string item_string)
-- ItemStack(string item_type, unsigned int item_count)
-- ItemStack(ItemTable table) -- Means table with fields "item_type" and "item_count"
function ItemStack(arg1, arg2)
    if type(arg1) == 'table' then
        return table_to_itemstack(arg1)
    end

    if type(arg1) == 'string' and arg2 == nil then
        return string_to_itemstack(arg1)
    end

    if type(arg1) == 'string' and type(arg2) == 'number' then
        local istack = core._new_item_stack()

        istack:set(arg1, arg2)

        return istack
    end

    return itemstack_copy(arg1)
end
