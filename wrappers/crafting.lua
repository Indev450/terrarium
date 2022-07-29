function terrarium.add_crafting_category(name)
    core._add_crafting_category(name)
end

function terrarium.set_default_crafting_category(name)
    core._set_default_crafting_category(name)
end

function terrarium.register_recipe(category, def)
    -- Set of unique items used in recipe requirements.
    -- Multiple use of same item type currently leads to problems.
    -- For example, if requirements are { "default:stone 10", "default:stone 5" },
    -- and player have, lets say, 11 items of type "default:stone", when trying to craft
    -- item, game will go like this:
    -- "player has at least 10 default:stone? yes."
    -- "player has at least 5 default:stone? yes. "
    -- "then we can craft item, take 15 default:stone from player inventory"
    -- This won't make item count negative, but it makes craft aviable
    -- even if it isn't meant to be.
    local items = {}

    for idx, item in pairs(def.requirements) do
        local istack = ItemStack(item)
        local name = istack:get_item_name()

        if items[name] then
            error("multiple items of same type in recipe requirements are not supported")
        end

        items[name] = istack
    end

    core._register_recipe(category, {
        requirements = items,
        result = ItemStack(def.result),
    })
end
