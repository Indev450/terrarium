-- Basically, reimplementation of Datafile class but without any parsing
function datafile.new(data)
    data = data or {}
    data.content = data.content or {}
    data.children = data.children or {}
    data.children_ids = data.children_ids or {}

    local methods = {
        set_string = function(self, value, i)
            local data = rawget(self, "data")

            i = i or 1
            value = value or ""

            local content = data.content

            while #content < i do table.insert(content, "") end

            content[i] = tostring(value)
        end,

        get_string = function(self, i)
            local data = rawget(self, "data")

            i = i or 1

            local content = data.content

            while #content < i do table.insert(content, "") end

            return content[i]
        end,

        -- floats and integers don't differ in lua, so why create 2 same methods
        set_number = function(self, value, i)
            self:set_string(tostring(value), i)
        end,

        get_number = function(self, i)
            return tonumber(self:get_string(i)) or 0
        end,

        set_bool = function(self, value, i)
            self:set_string(value and "1" or "0", i)
        end,

        get_bool = function(self, i)
            return self:get_number(i) ~= 0
        end,

        get_value_count = function(self)
            return #rawget(self, "data").content
        end,

        iter_children = function(self)
            local i = 0
            local children = rawget(self, "data").children

            return function()
                i = i + 1

                if i <= #children then return children[i][1], self:get_child(children[i][1]) end
            end
        end,

        get_child = function(self, name)
            local data = rawget(self, "data")

            local children = data.children
            local children_ids = data.children_ids

            if children_ids[name] == nil then
                local child = datafile.new()
                table.insert(children, { name, rawget(child, "data"), child })
                children_ids[name] = #children
            end

            local tuple = children[children_ids[name]]

            -- In case our data was returned from C++. C++ doesn't know about
            -- our custom datafile type, so it will only return pair of name and
            -- datafile data, so we need to create datafile object ourselves
            if tuple[3] == nil then
                tuple[3] = datafile.new(tuple[2])
            end

            return children[children_ids[name]][3]
        end,

        child_exists = function(self, name)
            local data = rawget(self, "data")

            return data.children_ids[name] ~= nil
        end,

        to_file = function(self, filename, indent, list_sep)
            datafile._to_file(filename, rawget(self, "data"), indent, list_sep)
        end,

        to_string = function(self, indent, list_sep)
            return datafile._to_string(rawget(self, "data"), indent, list_sep)
        end,
    }

    return setmetatable({ data = data }, {
        __index = function(self, key)
            if methods[key] then
                return methods[key]
            elseif type(key) == "string" then
                return methods.get_child(self, key)
            end
        end,

        __newindex = function(self, key, value)
            error("attempt to set index for Datafile object")
        end,
    })
end

function datafile.from_file(filename, list_sep)
    return datafile.new(datafile._from_file(filename, list_sep))
end

function datafile.from_string(str, list_sep)
    return datafile.new(datafile._from_string(str, list_sep))
end
