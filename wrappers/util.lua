-- Creates enum
function enum(values)
    local result = {}

    for i, name in pairs(values) do
        result[name] = i
    end

    return result
end

-- Creates enum that can combine its values with | operator
function binenum(values)
    local result = {
        NONE = 0,
    }

    local i = 1

    for _, name in pairs(values) do
        result[name] = i

        i = i * 2
    end

    return result
end

function fmt_table(t, spaces)
    if type(t) ~= 'table' then return string.format("%q", t) end

    spaces = spaces or 0

    local result = '{\n'

    for k, v in pairs(t) do
        result = result..string.format("%s[%s] = %s,\n",
            string.rep(' ', spaces+4),
            fmt_table(k, spaces+4),
            fmt_table(v, spaces+4))
    end

    return result..string.rep(' ', spaces).."}"
end

function print_table(t)
    print(fmt_table(t))
end

function copy(obj)
    if type(obj) ~= 'table' then return obj end

    local cobj = {}

    for k, v in pairs(obj) do
        cobj[k] = v
    end

    return cobj
end

function deep_copy(obj, seen)
    if type(obj) ~= 'table' then return obj end

    if seen and seen[obj] then return seen[obj] end

    local s = seen or {}

    local res = setmetatable({}, getmetatable(obj))

    s[obj] = res

    for k, v in pairs(obj) do
        res[deep_copy(k, s)] = deep_copy(v, s)
    end

    return res
end

function apply_defaults(table, defaults)
    for key, value in pairs(defaults) do
        table[key] = table[key] or deep_copy(defaults[key])
    end

    return table
end

timer = {}

function timer.new(time_to_wait, is_repeat)
    return setmetatable({
        timer = time_to_wait,
        is_repeat = is_repeat or false,
        time_to_wait = time_to_wait,
    }, {__index = timer})
end

function timer:tick()
    self.timer = self.timer - dtime

    if self.timer < 0 then
        if self.is_repeat then
            self.timer = self.time_to_wait
        end

        return true
    end

    return false
end

function timer:restart()
    self.timer = self.time_to_wait
end
